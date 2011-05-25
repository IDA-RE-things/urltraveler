// ChromePlugIn.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ChromePlugIn.h"
#include <shlwapi.h>
#include "PathHelper.h"
#include "StringHelper.h"
#include "TimeHelper.h"
#include "CRCHash.h"
#include "ChromePlugInFactory.h"
#include <string>
#include <fstream>
#include "json/json.h"

#pragma comment(lib, "shlwapi.lib")

using namespace chromeplugin;


CChromePlugIn::CChromePlugIn(void)
{
}

CChromePlugIn::~CChromePlugIn(void)
{
}

BOOL CChromePlugIn::Load()
{
	return TRUE;
}

BOOL CChromePlugIn::UnLoad()
{
	return TRUE;
}


//----------------------------------------------------------------------------------------
//名称: IsWorked
//描述: 检查该插件是否能够正常工作。如果没有安装该PlugIn对应的浏览器，则该插件不需要
//	处理，只需要返回FALSE即可。否则它将会被调用。
//返回: 插件的版本号，通常为一整数。
//----------------------------------------------------------------------------------------
BOOL CChromePlugIn::IsWorked()
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: GetPlugInVersion
//描述: 获取当前插件的版本号
//返回: 插件的版本号，通常为一整数。
//----------------------------------------------------------------------------------------
int32 CChromePlugIn::GetPlugInVersion() 
{
	return 0;
}

//----------------------------------------------------------------------------------------
//名称: GetBrowserName
//描述: 获取该插件对应的浏览器的名称和版本
//----------------------------------------------------------------------------------------
const wchar_t* CChromePlugIn::GetBrowserName() 
{
	return L"Chrome";
}

//----------------------------------------------------------------------------------------
//名称: GetBrowserIcon
//描述: 获取该插件对应的浏览器的图标
//----------------------------------------------------------------------------------------
HICON CChromePlugIn::GetBrowserIcon() 
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
wchar_t* CChromePlugIn::GetInstallPath() 
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == ::SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.EXE",
		L"Path", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		if (::PathRemoveFileSpec(szPath))
		{
			::MessageBox(NULL, szPath, szPath, NULL);
			return szPath;
		}
	}

	return NULL;
}

//----------------------------------------------------------------------------------------
//名称: GetFavoriteDataPath
//描述: 获取浏览器收藏夹对应的文件或者文件夹的路径
//----------------------------------------------------------------------------------------
wchar_t* CChromePlugIn::GetFavoriteDataPath() 
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Local\\Google\\Chrome\\User Data\\Default\\Bookmarks";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//名称: GetHistoryDataPath
//描述: 获取浏览器收藏夹的历史数据对应的文件或者文件夹
//----------------------------------------------------------------------------------------
wchar_t* CChromePlugIn::GetHistoryDataPath() 
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Local\\Google\\Chrome\\User Data\\Default\\History";

	return wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//名称: GetFavoriteCount
//描述: 获取浏览器中收藏网址的条数 
//返回:
//      回返收藏网址条数
//----------------------------------------------------------------------------------------
int32 CChromePlugIn::GetFavoriteCount()
{

	return 0;
}

//----------------------------------------------------------------------------------------
//名称: ExportFavoriteData
//描述: 导出当前浏览器的所有的收藏夹数据
//参数: 
//		@param	pData			导出的收藏夹数据数组
//		@param	nDataNum		导出的收藏夹条目的条数
//----------------------------------------------------------------------------------------
BOOL CChromePlugIn::ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum)
{
	BOOL bRetCode = FALSE;
	Json::Value chrome_bookmarks;
	std::wstring strPath = GetFavoriteDataPath();
	std::string strTmpPath = StringHelper::UnicodeToUtf8(strPath);
	
	free((void *)strPath.c_str());
	chrome_bookmarks.clear();

	std::ifstream infile(strTmpPath.c_str());
	Json::Reader reader;
	if (reader.parse(infile, chrome_bookmarks))
	{
		if (!chrome_bookmarks.empty())
		{
			Json::Value roots = chrome_bookmarks["roots"];
			ExportFolder(roots["bookmark_bar"], 0, pData, nDataNum);
			ExportFolder(roots["other"], 0, pData, nDataNum);
		}
		
		bRetCode = TRUE;
	}

	infile.close();

	return bRetCode;
}

//----------------------------------------------------------------------------------------
//名称: ImportFavoriteData
//描述: 将当前的记录导入到浏览器中
//参数: 
//		@param	pData			需要导入的的收藏夹数据数组
//		@param	nDataNum		需要导入的收藏夹条目的条数
//----------------------------------------------------------------------------------------
BOOL CChromePlugIn::ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum)
{
	if ((pData == NULL) || (nDataNum == 0))
	{
		return FALSE;
	}

	Json::Value roots;
	Json::Value bookmark_bar;
	Json::Value other;
	Json::Value children;
	Json::StyledStreamWriter writer;
	int32 nIndex = 0;

	for (int32 i = 0; i < nDataNum; ++i)
	{
		if (pData[i].bDelete == true)
		{
			continue;
		}

		if (pData[i].bFolder == true)
		{
			CHROMEFOLDERNODE stFolderNode = {0};
			stFolderNode.nAddedTime = pData[i].nAddTimes;
			stFolderNode.nModifiedTime = pData[i].nLastModifyTime;
			stFolderNode.nId = nIndex;
			wcscpy_s(stFolderNode.szName, MAX_PATH - 1, pData[i].szTitle);
			wcscpy_s(stFolderNode.szType, MAX_PATH - 1, L"folder");

			Json::Value folder_obj;
			MakeFolderNode(stFolderNode, folder_obj);

			if (pData[i].nPid == 0)
			{
				bookmark_bar.append(folder_obj);
			}
		}
		else
		{
			CHROMEURLNODE stUrlNode = {0};
			stUrlNode.nAddedTime = pData[i].nAddTimes;
			stUrlNode.nId = nIndex;
			wcscpy_s(stUrlNode.szName, MAX_PATH - 1, pData[i].szTitle);
			wcscpy_s(stUrlNode.szType, MAX_PATH - 1, L"url");
			wcscpy_s(stUrlNode.szUrl, MAX_URL_LEN - 1, pData[i].szUrl);

			Json::Value url_obj;
			MakeUrlNode(stUrlNode, url_obj);

			if (pData[i].nPid == 0)
			{
				other.append(url_obj);
			}
		}
	}


	std::wstring strPath = GetFavoriteDataPath();
	std::string strTmpPath = StringHelper::UnicodeToUtf8(strPath);
	free((void *)strPath.c_str());
	std::ofstream outfile(strTmpPath.c_str());

	roots.append(bookmark_bar);
	roots.append(other);
	writer.write(outfile, roots);

	return TRUE;
}

BOOL CChromePlugIn::ImportFavoriteData(FAVORITELINEDATA stData)
{
	return TRUE;
}

BOOL CChromePlugIn::ExportFolder(Json::Value& folder_obj, int32 nPid, PFAVORITELINEDATA pData, int32& nDataNum)
{
	if (folder_obj.empty() || (folder_obj["type"].asString() != std::string("folder")))
	{
		return FALSE;
	}

	pData[nDataNum].nId = nDataNum + ID_VALUE_CHROME_BEGIN;
	pData[nDataNum].bFolder = true;
	pData[nDataNum].bDelete = false;
	TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(folder_obj["date_added"].asString()).c_str()), pData[nDataNum].nAddTimes);
	TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(folder_obj["date_modified"].asString()).c_str()), pData[nDataNum].nLastModifyTime);
	pData[nDataNum].nPid = nPid;

	wcscpy_s(pData[nDataNum].szTitle, MAX_PATH -1, StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str());
	pData[nDataNum].szUrl[0] = 0;
	pData[nDataNum].nCatId = 0;
	
	CCRCHash ojbCrcHash;
	ojbCrcHash.GetHash((BYTE *)pData[nDataNum].szTitle, wcslen(pData[nDataNum].szTitle) * sizeof(wchar_t), (BYTE *)&pData[nDataNum].nHashId, sizeof(int32));
	
	nDataNum++;

	Json::Value children_nodes = folder_obj["children"];
	int32 nNodeCount = children_nodes.size();
	for (int32 i = 0; i < nNodeCount; ++i)
	{
		Json::Value new_val = children_nodes[i];
		if (new_val["type"].asString() == std::string("url"))
		{
			ExportUrl(new_val, nDataNum -1 + ID_VALUE_CHROME_BEGIN, pData, nDataNum);
		}
		else if(new_val["type"].asString() == std::string("folder"))
		{
			ExportFolder(new_val, nDataNum -1 + ID_VALUE_CHROME_BEGIN, pData, nDataNum);
		}
	}

	return TRUE;
}

BOOL CChromePlugIn::ExportUrl(Json::Value& url_obj, int32 nPid, PFAVORITELINEDATA pData, int32& nDataNum)
{
	if (url_obj.empty() || url_obj["type"].asString() != std::string("url"))
	{
		return FALSE;
	}

	pData[nDataNum].nId = nDataNum + ID_VALUE_CHROME_BEGIN;
	pData[nDataNum].bFolder = false;
	pData[nDataNum].bDelete = false;
	TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(url_obj["date_added"].asString()).c_str()), pData[nDataNum].nAddTimes);
	pData[nDataNum].nLastModifyTime =  0;
	pData[nDataNum].nPid = nPid;
	pData[nDataNum].nCatId = 0;

	wcscpy_s(pData[nDataNum].szTitle, MAX_PATH -1, StringHelper::Utf8ToUnicode(url_obj["name"].asString()).c_str());
	wcscpy_s(pData[nDataNum].szUrl, 1024 - 1, StringHelper::Utf8ToUnicode(url_obj["url"].asString()).c_str());
	pData[nDataNum].szUrl[1023] = 0;

	CCRCHash ojbCrcHash;
	ojbCrcHash.GetHash((BYTE *)pData[nDataNum].szTitle, wcslen(pData[nDataNum].szTitle) * sizeof(wchar_t), (BYTE *)&pData[nDataNum].nHashId, sizeof(int32));

	nDataNum++;

    return TRUE;
}


BOOL CChromePlugIn::MakeFolderNode(CHROMEFOLDERNODE stFolderNode, Json::Value& folder_obj)
{
	wchar_t szFormat[] = L" { \"children\": [ ],\
				  \"date_added\": \"%s\",\
				  \"date_modified\": \"%s\",\
				  \"id\": \"%d\",\
				  \"name\": \"%s\",\
				  \"type\": \"folder\" }";

	SYSTEMTIME sysTime1;
	SYSTEMTIME sysTime2;
	TimeHelper::Time2SysTime(stFolderNode.nAddedTime, sysTime1);
	TimeHelper::Time2SysTime(stFolderNode.nModifiedTime, sysTime2);

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 - 1, szFormat, TimeHelper::GetStrTime2(sysTime1), TimeHelper::GetStrTime2(sysTime2), stFolderNode.nId, stFolderNode.szName);
	std::string strTmp = StringHelper::UnicodeToUtf8(szTmp);

	Json::Reader reader;
	folder_obj.clear();
	reader.parse(strTmp.c_str(), folder_obj);

	return TRUE;
}

BOOL CChromePlugIn::MakeUrlNode(CHROMEURLNODE stUrlNode, Json::Value& url_obj)
{
	wchar_t szFormat[] = L" {\
				 \"date_added\": \"%s\",\
				 \"id\": \"%d\",\
				 \"name\": \"%s\",\
				 \"type\": \"url\",\
				 \"url\": \"%s\"}";
	
	SYSTEMTIME sysTime;
	TimeHelper::Time2SysTime(stUrlNode.nAddedTime, sysTime);

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 -1, szFormat, TimeHelper::GetStrTime2(sysTime), stUrlNode.nId, stUrlNode.szName, stUrlNode.szUrl);

	std::string strTmp = StringHelper::UnicodeToUtf8(szTmp);
	Json::Reader reader;
	url_obj.clear();
	reader.parse(strTmp.c_str(),url_obj);	

	return TRUE;
}