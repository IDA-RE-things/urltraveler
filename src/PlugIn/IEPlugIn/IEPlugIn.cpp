#include "StdAfx.h"
#include "IEPlugIn.h"
#include <shlwapi.h>
#include "PathHelper.h"
#include "StringHelper.h"
#include <vector>
#include <intshcut.h>
#include "CRCHash.h"
#include "TimeHelper.h"
#include "IEPlugInFactory.h"
#include "XString.h"
#include "FilePath.h"


#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "ole32.lib")

using namespace ieplugin;


IEPlugIn::IEPlugIn(void)
{
}

IEPlugIn::~IEPlugIn(void)
{
	CoUninitialize();
}

BOOL IEPlugIn::Load()
{
	return TRUE;
}

BOOL IEPlugIn::UnLoad()
{
	return TRUE;
}


//----------------------------------------------------------------------------------------
//名称: GetPlugInVersion
//描述: 获取当前插件的版本号
//返回: 插件的版本号，通常为一整数。
//----------------------------------------------------------------------------------------
int32 IEPlugIn::GetPlugInVersion() 
{
	wchar_t szVersion[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szVersion); 
	int32   nVersion = 0;

	if (ERROR_SUCCESS == ::SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Internet Explorer",
		L"Version", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szVersion, 
		&dwSize))
	{
		std::string strVersion = StringHelper::UnicodeToANSI(szVersion);
		std::vector<std::string> vecResutl = StringHelper::Split(strVersion, '.');

		if (vecResutl.size() < 4)
		{
			return 0;
		}

		BYTE nMajaor = atol(vecResutl[0].c_str());
		BYTE nMinor  = atol(vecResutl[1].c_str());
		WORD nBuilderNum = atol(vecResutl[3].c_str());

		nVersion = MAKELONG(MAKEWORD(nMajaor, nMinor), nBuilderNum);

		return nVersion ;
	}

	return 0;
}

//----------------------------------------------------------------------------------------
//名称: GetBrowserName
//描述: 获取该插件对应的浏览器的名称和版本
//----------------------------------------------------------------------------------------
const wchar_t* IEPlugIn::GetBrowserName() 
{
/*
	SHFILEINFO stFileInfo = {0};
	HRESULT hr = ::SHGetFileInfo(GetInstallPath(), 0, &stFileInfo, sizeof(stFileInfo), SHGFI_DISPLAYNAME);

	if (SUCCEEDED(hr))
	{
		return stFileInfo.szDisplayName;
	}
	return NULL;
*/

	return L"IE浏览器";
}

//----------------------------------------------------------------------------------------
//名称: GetBrowserIcon
//描述: 获取该插件对应的浏览器的图标
//----------------------------------------------------------------------------------------
HICON IEPlugIn::GetBrowserIcon() 
{
	SHFILEINFO stFileInfo = {0};
	HRESULT hr = ::SHGetFileInfo(GetInstallPath(), 0, &stFileInfo, sizeof(stFileInfo), SHGFI_ICON);

	if (SUCCEEDED(hr))
	{
		return stFileInfo.hIcon;
	}
	return NULL;
}

//----------------------------------------------------------------------------------------
//名称: GetInstallPath
//描述: 获取插件对应的浏览器的安装目录
//----------------------------------------------------------------------------------------
wchar_t* IEPlugIn::GetInstallPath() 
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == ::SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORER.EXE",
		L"Path", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		String	strPath = szPath;
		strPath = strPath.TrimRight(L';');
		if (::PathRemoveFileSpec(szPath))
		{
			swprintf_s(szPath, MAX_PATH-1, L"%s\\%s", strPath.GetData(), L"iexplorer.exe");
			return _wcsdup(szPath);
		}
	}

	if (ERROR_SUCCESS == ::SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE",
		L"Path", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		String	strPath = szPath;

		if (-1 != strPath.Find(L";"))
		{
			strPath = strPath.TrimRight(L';');
			swprintf_s(szPath, MAX_PATH-1, L"%s\\%s", strPath.GetData(), L"iexplore.exe");
		}
		else
		{
			if (::PathRemoveFileSpec(szPath))
			{
				swprintf_s(szPath, MAX_PATH-1, L"%s\\%s", szPath, L"iexplore.exe");
			}
		}

		return _wcsdup(szPath);
	}

	return NULL;
}

//----------------------------------------------------------------------------------------
//名称: GetFavoriteDataPath
//描述: 获取浏览器收藏夹对应的文件或者文件夹的路径
//----------------------------------------------------------------------------------------
wchar_t* IEPlugIn::GetFavoriteDataPath() 
{
	std::wstring strPath = PathHelper::GetHomeDir() + wstring(L"\\Favorites");

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//名称: GetHistoryDataPath
//描述: 获取浏览器收藏夹的历史数据对应的文件或者文件夹
//----------------------------------------------------------------------------------------
wchar_t* IEPlugIn::GetHistoryDataPath() 
{
	std::wstring strPath = PathHelper::GetHomeDir() + wstring(L"\\AppData\\Local\\Microsoft\\Windows\\History");

	return wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//名称: GetFavoriteCount
//描述: 获取浏览器中收藏网址的条数 
//返回:
//      回返收藏网址条数
//----------------------------------------------------------------------------------------
int32 IEPlugIn::GetFavoriteCount()
{
	LPITEMIDLIST pItemList = NULL;

	HRESULT hr = ::SHGetMalloc(&m_pMalloc);
	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = ::SHGetSpecialFolderLocation(NULL, CSIDL_FAVORITES, &pItemList);

	if (NOERROR != hr)
	{
		m_pMalloc->Release();
		return FALSE;
	}

	IShellFolder *pShellFolder = NULL;
	hr = ::SHGetDesktopFolder(&pShellFolder);
	if (FAILED(hr))
	{
		m_pMalloc->Free(pItemList);
		m_pMalloc->Release();
		return hr;
	}

	IShellFolder *pFavFolder = NULL;
	hr = pShellFolder->BindToObject(pItemList, NULL, IID_IShellFolder, reinterpret_cast<void **>(&pFavFolder));

	int32 nDataNum = 0;
	BOOL bRet = TaverseFavoriteFolder(pFavFolder, 0, NULL, nDataNum, TRUE);

	m_pMalloc->Free(pItemList);
	m_pMalloc->Release();
	m_pMalloc = NULL;

	return nDataNum;
}

//----------------------------------------------------------------------------------------
//名称: ExportFavoriteData
//描述: 导出当前浏览器的所有的收藏夹数据
//参数: 
//		@param	pData			导出的收藏夹数据数组
//		@param	nDataNum		导出的收藏夹条目的条数
//----------------------------------------------------------------------------------------
BOOL IEPlugIn::ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
{
	LPITEMIDLIST pItemList = NULL;

	HRESULT hr = ::SHGetMalloc(&m_pMalloc);
	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = ::SHGetSpecialFolderLocation(NULL, CSIDL_FAVORITES, &pItemList);

	if (NOERROR != hr)
	{
		m_pMalloc->Release();
		return FALSE;
	}

	IShellFolder *pShellFolder = NULL;
	hr = ::SHGetDesktopFolder(&pShellFolder);
	if (FAILED(hr))
	{
		m_pMalloc->Free(pItemList);
		m_pMalloc->Release();
		return hr;
	}

	IShellFolder *pFavFolder = NULL;
	hr = pShellFolder->BindToObject(pItemList, NULL, IID_IShellFolder, reinterpret_cast<void **>(&pFavFolder));

	nDataNum = 0;
	TaverseFavoriteFolder(pFavFolder, 0, ppData, nDataNum);
	
	m_pMalloc->Free(pItemList);
	m_pMalloc->Release();
	m_pMalloc = NULL;

	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: ImportFavoriteData
//描述: 将当前的记录导入到浏览器中
//参数: 
//		@param	pData			需要导入的的收藏夹数据数组
//		@param	nDataNum		需要导入的收藏夹条目的条数
//----------------------------------------------------------------------------------------
int IEPlugIn::ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
{
	if (ppData == NULL || nDataNum == 0)
	{
		return ERROR_INVALID_PARAM;
	}

	//写入新数据之前删除原来的所有的数据
	wchar_t* pszFavDir = GetFavoriteDataPath();
	BOOL bRet = PathHelper::ForceRemoveDir(pszFavDir);

	if( PathHelper::IsDirExist(pszFavDir) == FALSE)
	{
		PathHelper::CreateMultipleDirectory(pszFavDir);
	}
	free(pszFavDir);

	int nRealNum  = 0;
	for (int i = 0; i < nDataNum; i++)
	{
		if( ppData[i] == NULL)
			continue;

		if (ppData[i]->bDelete == true)
			continue;

		nRealNum ++;
		
		wchar_t* pszCurrNodePath = GetNodeAbsolutePath(i, ppData);

		if (ppData[i]->bFolder == true)
		{
			int nRet = ::SHCreateDirectory(NULL, pszCurrNodePath);
			//free(pszCurrNodePath);
			if ((nRet != ERROR_SUCCESS) && (nRet != ERROR_ALREADY_EXISTS))
			{
				return ERROR_CREATE_FAVORITE_FOLD;
			}
			
			continue;
		}
		else
		{
#define MAX_LENGTH 1024

			wchar_t szFileDir[MAX_LENGTH] = {0};
			wcscpy_s(szFileDir, MAX_LENGTH - 1, pszCurrNodePath);

			PathRemoveFileSpec(szFileDir);
			PathHelper::CreateMultipleDirectory(szFileDir);
			wchar_t szCurrFileName[MAX_LENGTH] = {0};
			swprintf_s(szCurrFileName, MAX_LENGTH, L"%s%s", pszCurrNodePath, L".url");

			// 检查szCurrentFileName 中的不能在路径中出现的字符去掉
			CFilePath path(szCurrFileName);

			const wchar_t* pszFileName = path.GetFileName();
			String strFileName = pszFileName;
			free((void*)pszFileName);

			strFileName.Replace(L":", L"");
			strFileName.Replace(L"\\", L"");
			strFileName.Replace(L"/", L"");
			strFileName.Replace(L"*", L"");
			strFileName.Replace(L"?", L"");
			strFileName.Replace(L"<", L"");
			strFileName.Replace(L">", L"");
			strFileName.Replace(L"|", L"");

			const wchar_t* pszFilePath = path.GetFilePath();
			String strName = String(pszFilePath) + strFileName;
			free((void*)pszFilePath);
			WritePrivateProfileStringW(L"InternetShortcut", L"URL", ppData[i]->szUrl, strName.GetData());
		}

		free(pszCurrNodePath);
	}

	nDataNum = nRealNum;

	return ERROR_OK;
}

BOOL IEPlugIn::ImportFavoriteData(FAVORITELINEDATA stData)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: GetNodeAbsolutePath
//描述: 获取节点路径
//参数: 
//		@param	nIndex			当前节点索引
//      @param	pData           节点数据链表
//----------------------------------------------------------------------------------------
wchar_t* IEPlugIn::GetNodeAbsolutePath(int32 nIndex, PFAVORITELINEDATA* ppData)
{
	int nCurrNodeIndex = nIndex;
	std::vector<wchar_t *> vecNodeTitle;

	std::wstring strNodePath = GetFavoriteDataPath();
	while (ppData[nCurrNodeIndex]->nPid != 0)
	{
		//保存当前节点的路径
		vecNodeTitle.push_back(ppData[nCurrNodeIndex]->szTitle);
		nCurrNodeIndex = ppData[nCurrNodeIndex]->nPid - 1;
	}

	vecNodeTitle.push_back(ppData[nCurrNodeIndex]->szTitle);

	std::vector<wchar_t *>::reverse_iterator it;
	for (it = vecNodeTitle.rbegin(); it != vecNodeTitle.rend(); ++it)
	{
		//拼接当前节点的全路径
		strNodePath.append(L"\\");
		strNodePath.append(*it);
	}

	//复制一份，由调用者负责释放
	return wcsdup(strNodePath.c_str());
}

HRESULT IEPlugIn::ResolveInternetShortcut(LPCWSTR lpszLinkFile, LPWSTR* lpszURL)
{
	IUniformResourceLocator* pUrlLink = NULL;

	HRESULT hr = CoCreateInstance(CLSID_InternetShortcut, 
		NULL, CLSCTX_INPROC_SERVER,
		IID_IUniformResourceLocator, 
		(LPVOID*)&pUrlLink);

	if (SUCCEEDED(hr))
	{
		IPersistFile* pPersistFile = NULL;

		hr = pUrlLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPersistFile);
		if (SUCCEEDED(hr))
		{
			// 加载internet快捷方式数据
			hr = pPersistFile->Load(lpszLinkFile, STGM_READ);
			if (SUCCEEDED(hr))
			{
				hr = pUrlLink->GetURL(lpszURL);
			}

			pPersistFile->Release();
		}

		pUrlLink->Release();
	}

	return hr;
} 

BOOL IEPlugIn::GetFileTimeInfo(void *pFileData, FILETIMEINFO *pstFileTimeInfo)
{
	SYSTEMTIME sysTime;
	LPWIN32_FILE_ATTRIBUTE_DATA pFileAttrData = (LPWIN32_FILE_ATTRIBUTE_DATA)pFileData;

	::FileTimeToSystemTime (&pFileAttrData->ftCreationTime, &sysTime);
	TimeHelper::SysTime2Time(sysTime, pstFileTimeInfo->tCreateTime);

	::FileTimeToSystemTime (&pFileAttrData->ftLastWriteTime, &sysTime);
	TimeHelper::SysTime2Time(sysTime, pstFileTimeInfo->tLastWriteTime);

	::FileTimeToSystemTime (&pFileAttrData->ftLastAccessTime, &sysTime);
	TimeHelper::SysTime2Time(sysTime, pstFileTimeInfo->tLastAccessTime);

	return TRUE;
}


BOOL IEPlugIn::TaverseFavoriteFolder(IShellFolder* pFolder, int32 nPid, 
				     PFAVORITELINEDATA* ppData, int32& nDataNum, BOOL bStat)
{
	IEnumIDList  *pItems = NULL;        //对象遍历接口
	LPITEMIDLIST pidlNext = NULL;
	STRRET StrRetName;
	STRRET StrRetFile;
	LPWSTR lpszName = NULL;
	LPWSTR lpszFileName = NULL;
	LPWSTR lpszURL = NULL;
	CCRCHash ojbCrcHash;

	if (nPid == 0)
	{
		//根节点需要特殊处理,测试时需要，不然会导致利用到导出的时候缺少根节点的情况，导致节点错乱
		SHFILEINFO fileInfo;
		WIN32_FILE_ATTRIBUTE_DATA fileAttrData;
		FILETIMEINFO stFileTimeInfo = {0};
		wchar_t *pszFavoritePath = GetFavoriteDataPath();

		if (::GetFileAttributesEx(pszFavoritePath, GetFileExInfoStandard, &fileAttrData))
		{
			GetFileTimeInfo(&fileAttrData, &stFileTimeInfo);
		}

		free(pszFavoritePath);
	}
	
	HRESULT hr = pFolder->EnumObjects(NULL, SHCONTF_FOLDERS|SHCONTF_NONFOLDERS, &pItems);
	while (NOERROR == hr)
	{
		hr = pItems->Next(1, &pidlNext, NULL);
		if (NOERROR == hr)
		{
			pFolder->GetDisplayNameOf(pidlNext, SHGDN_NORMAL, &StrRetName);
			pFolder->GetDisplayNameOf(pidlNext, SHGDN_FORPARSING, &StrRetFile);

			::StrRetToStr(&StrRetName, pidlNext, &lpszName);
			::StrRetToStr(&StrRetFile, pidlNext, &lpszFileName);

			SHFILEINFO fileInfo;
			WIN32_FILE_ATTRIBUTE_DATA fileAttrData;
			FILETIMEINFO stFileTimeInfo = {0};

			if (::GetFileAttributesEx(lpszFileName, GetFileExInfoStandard, &fileAttrData))
			{
				GetFileTimeInfo(&fileAttrData, &stFileTimeInfo);
			}

			int nRet = ::SHGetFileInfo(lpszFileName, NULL, &fileInfo, sizeof(fileInfo), SHGFI_ATTRIBUTES|SHGFI_TYPENAME);
			if( wcscmp(fileInfo.szTypeName, L"") == 0)
				continue;

			if ((fileInfo.dwAttributes & SFGAO_FOLDER) && wcscmp(fileInfo.szTypeName, L"Channel Shortcut") != 0)
			{
				// 如果是目录需要递归遍历
				HRESULT hrSub = S_OK;
				IShellFolder* pSubFolder;
				hrSub = pFolder->BindToObject(pidlNext, NULL, IID_IShellFolder, (LPVOID*)&pSubFolder);
				if (FAILED(hrSub))
				{
					continue;
				}
	
				if (!bStat)
				{
					//如果只是统计节点数量，则不需要导出数据
					ppData[nDataNum]->nId = nDataNum + ID_VALUE_IE9_BEGIN;
					ppData[nDataNum]->bFolder = true;
					ppData[nDataNum]->bDelete = false;
					ppData[nDataNum]->nAddTimes = stFileTimeInfo.tCreateTime;
					ppData[nDataNum]->nLastModifyTime = stFileTimeInfo.tLastWriteTime;
					ppData[nDataNum]->nPid = nPid;

					wcscpy_s(ppData[nDataNum]->szTitle, MAX_LENGTH -1, lpszName);
					ppData[nDataNum]->szUrl[0] = 0;
					ojbCrcHash.GetHash((BYTE *)ppData[nDataNum]->szTitle, 
						wcslen(ppData[nDataNum]->szTitle) * sizeof(wchar_t), 
						(BYTE *)&ppData[nDataNum]->nHashId, sizeof(int32));
				}

				nDataNum++;
				TaverseFavoriteFolder(pSubFolder, nDataNum - 1 + ID_VALUE_IE9_BEGIN, ppData, nDataNum, bStat);
				pSubFolder->Release();
			}
			else
			{
				if  (wcscmp(fileInfo.szTypeName, L"Internet 快捷方式") != 0 &&
					wcscmp(fileInfo.szTypeName, L"Internet Shortcut") != 0)
				{
					continue;
				}

				ResolveInternetShortcut(lpszFileName, &lpszURL);

				if (NULL != lpszURL)
				{
					if (!bStat)
					{
						ppData[nDataNum]->nId = nDataNum + ID_VALUE_IE9_BEGIN;
						ppData[nDataNum]->bFolder = false;
						ppData[nDataNum]->bDelete = false;
						ppData[nDataNum]->nAddTimes = stFileTimeInfo.tCreateTime;
						ppData[nDataNum]->nLastModifyTime = stFileTimeInfo.tLastWriteTime;
						ppData[nDataNum]->nPid = nPid;
						wcscpy_s(ppData[nDataNum]->szTitle, MAX_LENGTH -1, lpszName);
						wcscpy_s(ppData[nDataNum]->szUrl, MAX_LENGTH - 1, lpszURL);
						ppData[nDataNum]->szUrl[MAX_LENGTH-1] = 0;

						ojbCrcHash.GetHash((BYTE *)ppData[nDataNum]->szTitle, 
							wcslen(ppData[nDataNum]->szTitle) * sizeof(wchar_t),
							(BYTE *)&ppData[nDataNum]->nHashId, sizeof(int32));
						wcscpy_s(ppData[nDataNum]->szUrl, MAX_LENGTH - 1, lpszURL);
					}
					
					nDataNum++;
					m_pMalloc->Free(lpszURL);
				}
			}

			if (lpszName)
			{
				//释放StrRetToStr中分配的资源
				::CoTaskMemFree(lpszName);
			}

			if (lpszFileName)
			{
				//释放StrRetToStr中分配的资源
				::CoTaskMemFree(lpszFileName);
			}

			if (pidlNext)
			{
				m_pMalloc->Free(pidlNext);
			}
		}
	}

	if (NULL != pItems)
	{
		pItems->Release();
	}

	return TRUE;
}
