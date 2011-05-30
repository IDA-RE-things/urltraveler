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
//����: GetPlugInVersion
//����: ��ȡ��ǰ����İ汾��
//����: ����İ汾�ţ�ͨ��Ϊһ������
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
//����: GetBrowserName
//����: ��ȡ�ò����Ӧ������������ƺͰ汾
//----------------------------------------------------------------------------------------
const wchar_t* IEPlugIn::GetBrowserName() 
{
	SHFILEINFO stFileInfo = {0};
	HRESULT hr = ::SHGetFileInfo(GetInstallPath(), 0, &stFileInfo, sizeof(stFileInfo), SHGFI_DISPLAYNAME);

	if (SUCCEEDED(hr))
	{
		return stFileInfo.szDisplayName;
	}
	return NULL;
}

//----------------------------------------------------------------------------------------
//����: GetBrowserIcon
//����: ��ȡ�ò����Ӧ���������ͼ��
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
//����: GetInstallPath
//����: ��ȡ�����Ӧ��������İ�װĿ¼
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
		if (::PathRemoveFileSpec(szPath))
		{
			swprintf_s(szPath, MAX_PATH-1, L"%s\\%s", szPath, L"iexplorer.exe");
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
		if (::PathRemoveFileSpec(szPath))
		{
			swprintf_s(szPath, MAX_PATH-1, L"%s\\%s", szPath, L"iexplore.exe");
			return _wcsdup(szPath);
		}
	}


	return NULL;
}

//----------------------------------------------------------------------------------------
//����: GetFavoriteDataPath
//����: ��ȡ������ղؼж�Ӧ���ļ������ļ��е�·��
//----------------------------------------------------------------------------------------
wchar_t* IEPlugIn::GetFavoriteDataPath() 
{
	std::wstring strPath = PathHelper::GetHomeDir() + L"\\Favorites";

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©

	return wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//����: GetHistoryDataPath
//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
//----------------------------------------------------------------------------------------
wchar_t* IEPlugIn::GetHistoryDataPath() 
{
	std::wstring strPath = PathHelper::GetHomeDir() + L"\\AppData\\Local\\Microsoft\\Windows\\History";

	return wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//����: GetFavoriteCount
//����: ��ȡ��������ղ���ַ������ 
//����:
//      �ط��ղ���ַ����
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
//����: ExportFavoriteData
//����: ������ǰ����������е��ղؼ�����
//����: 
//		@param	pData			�������ղؼ���������
//		@param	nDataNum		�������ղؼ���Ŀ������
//----------------------------------------------------------------------------------------
BOOL IEPlugIn::ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum)
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
	TaverseFavoriteFolder(pFavFolder, 0, pData, nDataNum);
	
	m_pMalloc->Free(pItemList);
	m_pMalloc->Release();
	m_pMalloc = NULL;

	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: ImportFavoriteData
//����: ����ǰ�ļ�¼���뵽�������
//����: 
//		@param	pData			��Ҫ����ĵ��ղؼ���������
//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
//----------------------------------------------------------------------------------------
BOOL IEPlugIn::ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum)
{
	if ((pData == NULL) || (nDataNum == 0))
	{
		return FALSE;
	}

	//д��������֮ǰɾ��ԭ�������е�����
	wchar_t* pszFavDir = GetFavoriteDataPath();
	BOOL bRet = PathHelper::ForceRemoveDir(pszFavDir);
	free(pszFavDir);

	for (int i = 0; i < nDataNum; i++)
	{
		if (pData[i].bDelete == true)
		{//��ʱδ������
			continue;
		}
		
		wchar_t* pszCurrNodePath = GetNodeAbsolutePath(i, pData);

		if (pData[i].bFolder == true)
		{
			int nRet = ::SHCreateDirectory(NULL, pszCurrNodePath);
			free(pszCurrNodePath);
			if ((nRet != ERROR_SUCCESS) && (nRet != ERROR_ALREADY_EXISTS))
			{
				return FALSE;
			}
			
			continue;
		}
		else
		{
			wchar_t szFileDir[MAX_PATH] = {0};
			wcscpy_s(szFileDir, MAX_PATH - 1, pszCurrNodePath);

			PathRemoveFileSpec(szFileDir);
			PathHelper::CreateMultipleDirectory(szFileDir);
			wchar_t szCurrFileName[MAX_PATH] = {0};
			swprintf_s(szCurrFileName, MAX_PATH, L"%s%s", pszCurrNodePath, L".url");
			WritePrivateProfileStringW(L"InternetShortcut", L"URL", pData[i].szUrl, szCurrFileName);
		}

		free(pszCurrNodePath);
	}

	return TRUE;
}

BOOL IEPlugIn::ImportFavoriteData(FAVORITELINEDATA stData)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: GetNodeAbsolutePath
//����: ��ȡ�ڵ�·��
//����: 
//		@param	nIndex			��ǰ�ڵ�����
//      @param	pData           �ڵ���������
//----------------------------------------------------------------------------------------
wchar_t* IEPlugIn::GetNodeAbsolutePath(int32 nIndex, PFAVORITELINEDATA pData)
{
	int nCurrNodeIndex = nIndex;
	std::vector<wchar_t *> vecNodeTitle;

	std::wstring strNodePath = GetFavoriteDataPath();
	while (pData[nCurrNodeIndex].nPid != 0)
	{
		//���浱ǰ�ڵ��·��
		vecNodeTitle.push_back(pData[nCurrNodeIndex].szTitle);
		nCurrNodeIndex = pData[nCurrNodeIndex].nPid - 1;
	}

	vecNodeTitle.push_back(pData[nCurrNodeIndex].szTitle);

	std::vector<wchar_t *>::reverse_iterator it;
	for (it = vecNodeTitle.rbegin(); it != vecNodeTitle.rend(); ++it)
	{
		//ƴ�ӵ�ǰ�ڵ��ȫ·��
		strNodePath.append(L"\\");
		strNodePath.append(*it);
	}

	//����һ�ݣ��ɵ����߸����ͷ�
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
			// ����internet��ݷ�ʽ����
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


BOOL IEPlugIn::TaverseFavoriteFolder(IShellFolder* pFolder, int32 nPid, PFAVORITELINEDATA pData, int32& nDataNum, BOOL bStat)
{
	IEnumIDList  *pItems = NULL;        //��������ӿ�
	LPITEMIDLIST pidlNext = NULL;
	STRRET StrRetName;
	STRRET StrRetFile;
	LPWSTR lpszName = NULL;
	LPWSTR lpszFileName = NULL;
	LPWSTR lpszURL = NULL;
	CCRCHash ojbCrcHash;

	if (nPid == 0)
	{//���ڵ���Ҫ���⴦��,����ʱ��Ҫ����Ȼ�ᵼ�����õ�������ʱ��ȱ�ٸ��ڵ����������½ڵ����
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

			::SHGetFileInfo(lpszFileName, NULL, &fileInfo, sizeof(fileInfo), SHGFI_ATTRIBUTES|SHGFI_TYPENAME);
			if ((fileInfo.dwAttributes & SFGAO_FOLDER) && wcscmp(fileInfo.szTypeName, L"Channel Shortcut") != 0)
			{
				// �����Ŀ¼��Ҫ�ݹ����
				HRESULT hrSub = S_OK;
				IShellFolder* pSubFolder;
				hrSub = pFolder->BindToObject(pidlNext, NULL, IID_IShellFolder, (LPVOID*)&pSubFolder);
				if (FAILED(hrSub))
				{
					continue;
				}
	
				if (!bStat)
				{//���ֻ��ͳ�ƽڵ�����������Ҫ��������
					pData[nDataNum].nId = nDataNum + ID_VALUE_IE9_BEGIN;
					pData[nDataNum].bFolder = true;
					pData[nDataNum].bDelete = false;
					pData[nDataNum].nAddTimes = stFileTimeInfo.tCreateTime;
					pData[nDataNum].nLastModifyTime = stFileTimeInfo.tLastWriteTime;
					pData[nDataNum].nPid = nPid;

					wcscpy_s(pData[nDataNum].szTitle, MAX_PATH -1, lpszName);
					pData[nDataNum].szUrl[0] = 0;
					ojbCrcHash.GetHash((BYTE *)pData[nDataNum].szTitle, wcslen(pData[nDataNum].szTitle) * sizeof(wchar_t), (BYTE *)&pData[nDataNum].nHashId, sizeof(int32));
					pData[nDataNum].nCatId = 0;
				}

				nDataNum++;
				TaverseFavoriteFolder(pSubFolder, nDataNum - 1 + ID_VALUE_IE9_BEGIN, pData, nDataNum, bStat);
				pSubFolder->Release();
			}
			else
			{
				if  (!wcscmp(fileInfo.szTypeName, L"Internet ��ݷ�ʽ"))
				{
					ResolveInternetShortcut(lpszFileName, &lpszURL);
				}

				if (NULL != lpszURL)
				{
					if (!bStat)
					{
						pData[nDataNum].nId = nDataNum + ID_VALUE_IE9_BEGIN;
						pData[nDataNum].bFolder = false;
						pData[nDataNum].bDelete = false;
						pData[nDataNum].nAddTimes = stFileTimeInfo.tCreateTime;
						pData[nDataNum].nLastModifyTime = stFileTimeInfo.tLastWriteTime;
						pData[nDataNum].nPid = nPid;
						wcscpy_s(pData[nDataNum].szTitle, MAX_PATH -1, lpszName);
						wcscpy_s(pData[nDataNum].szUrl, 1024 - 1, lpszURL);
						pData[nDataNum].szUrl[1023] = 0;

						ojbCrcHash.GetHash((BYTE *)pData[nDataNum].szTitle, wcslen(pData[nDataNum].szTitle) * sizeof(wchar_t), (BYTE *)&pData[nDataNum].nHashId, sizeof(int32));
						pData[nDataNum].nCatId = 0;
						wcscpy_s(pData[nDataNum].szUrl, 1024 - 1, lpszURL);
					}
					
					nDataNum++;
					m_pMalloc->Free(lpszURL);
				}
			}

			if (lpszName)
			{
				//�ͷ�StrRetToStr�з������Դ
				::CoTaskMemFree(lpszName);
			}

			if (lpszFileName)
			{
				//�ͷ�StrRetToStr�з������Դ
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
