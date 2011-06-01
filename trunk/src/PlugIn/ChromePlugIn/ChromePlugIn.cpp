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
#include <stack>
#include <fstream>
#include "json/json.h"

#pragma comment(lib, "shlwapi.lib")

using namespace chromeplugin;


CChromePlugIn::CChromePlugIn(void) : m_nMaxDepth(0)
{
	m_mapDepthInfo.clear();
	m_mapPidInfo.clear();
}

CChromePlugIn::~CChromePlugIn(void)
{
	m_mapDepthInfo.clear();
	m_mapPidInfo.clear();
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
			return wcsdup(szPath);
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
	std::wstring strPath = PathHelper::GetLocalAppDataDir() + L"\\Google\\Chrome\\User Data\\Default\\Bookmarks";

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
	int32 nFavoriteCount = 0;
	Json::Value chrome_bookmarks;
	chrome_bookmarks.clear();

	wchar_t* pszPath = GetFavoriteDataPath();
	std::string strTmpPath = StringHelper::UnicodeToUtf8(pszPath);
	free(pszPath);

	std::ifstream infile(strTmpPath.c_str());
	Json::Reader reader;
	if (reader.parse(infile, chrome_bookmarks))
	{
 		EnumNode(chrome_bookmarks["roots"]["bookmark_bar"], nFavoriteCount);
 		EnumNode(chrome_bookmarks["roots"]["other"], nFavoriteCount);
	}

	infile.close();
	return nFavoriteCount;
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

	wchar_t* pszPath = GetFavoriteDataPath();
	std::string strTmpPath = StringHelper::UnicodeToUtf8(pszPath);
	free(pszPath);

	chrome_bookmarks.clear();

	std::ifstream infile(strTmpPath.c_str());
	Json::Reader reader;
	if (reader.parse(infile, chrome_bookmarks))
	{
		if (!chrome_bookmarks.empty())
		{
			nDataNum = 0;
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

	wchar_t* pszPath = GetFavoriteDataPath();
	//获取导入文件路径
	std::string strTmpPath = StringHelper::UnicodeToUtf8(pszPath);
	//导入前先删除之前的收藏夹文件
	BOOL bResult = ::DeleteFileW(pszPath);
	free(pszPath);
	if (!bResult)
	{
		return FALSE;
	}

	Json::Value roots;
	Json::Value bookmark_bar;
	Json::Value other;
	Json::Value curr_node;
	int32 nIndex = 0;
	int32 nDepth = 0;

	MakeSpecialFolderNode(L"roots", nIndex, roots);
	MakeSpecialFolderNode(L"bookmark_bar", nIndex, bookmark_bar);
	MakeSpecialFolderNode(L"other", nIndex, other);
	curr_node = roots.append(bookmark_bar);

	for (int32 i = 0; i < nDataNum; ++i)
	{
		if (pData[i].bDelete == true)
		{
			continue;
		}

		MAP_PID_INFO::iterator it;
		it = m_mapPidInfo.find(pData[i].nPid);
		if (it != m_mapPidInfo.end())
		{
			nDepth = (*it).second + 1;
		}
		else
		{
			nDepth = 0;
		}

		if (nDepth > m_nMaxDepth)
		{
			m_nMaxDepth = nDepth;
		}

		m_mapPidInfo.insert(MAP_PID_INFO::value_type(pData[i].nPid, nDepth));
		m_mapDepthInfo.insert(MAP_DEPTH_INFO::value_type(nDepth, i));

		if (pData[i].bFolder == true)
		{
			
// 			Json::Value folder_obj;
// 			MakeFolderNode(pData[i], folder_obj, nIndex);
		}
		else
		{
// 			Json::Value url_obj;
// 			MakeUrlNode(pData[i], url_obj, nIndex);
		}
	}

	roots.append(other);
	std::ofstream outfile(strTmpPath.c_str());
	Json::StyledStreamWriter writer;
	writer.write(outfile, roots);
	outfile.close();

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
	TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(folder_obj["date_added"].asString()).c_str()),  \
		pData[nDataNum].nAddTimes);
	TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(folder_obj["date_modified"].asString()).c_str()),  \
		pData[nDataNum].nLastModifyTime);
	pData[nDataNum].nPid = nPid;

	wcscpy_s(pData[nDataNum].szTitle, MAX_PATH -1, StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str());
	pData[nDataNum].szUrl[0] = 0;
	pData[nDataNum].nCatId = 0;
	
	CCRCHash ojbCrcHash;
	ojbCrcHash.GetHash((BYTE *)pData[nDataNum].szTitle, wcslen(pData[nDataNum].szTitle) * sizeof(wchar_t),  \
		(BYTE *)&pData[nDataNum].nHashId, sizeof(int32));
	
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
	TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(url_obj["date_added"].asString()).c_str()),  \
		pData[nDataNum].nAddTimes);
	pData[nDataNum].nLastModifyTime =  0;
	pData[nDataNum].nPid = nPid;
	pData[nDataNum].nCatId = 0;

	wcscpy_s(pData[nDataNum].szTitle, MAX_PATH -1, StringHelper::Utf8ToUnicode(url_obj["name"].asString()).c_str());
	wcscpy_s(pData[nDataNum].szUrl, 1024 - 1, StringHelper::Utf8ToUnicode(url_obj["url"].asString()).c_str());
	pData[nDataNum].szUrl[1023] = 0;

	CCRCHash ojbCrcHash;
	ojbCrcHash.GetHash((BYTE *)pData[nDataNum].szTitle, wcslen(pData[nDataNum].szTitle) * sizeof(wchar_t),  \
		(BYTE *)&pData[nDataNum].nHashId, sizeof(int32));

	nDataNum++;

    return TRUE;
}


BOOL CChromePlugIn(FAVORITELINEDATA stData, Json::Value& folder_obj, int32& nIndex)
{
	wchar_t szFormat[] = L" { \"children\": [ ],\
				  \"date_added\": \"%s\",\
				  \"date_modified\": \"%s\",\
				  \"id\": \"%d\",\
				  \"name\": \"%s\",\
				  \"type\": \"folder\" }";

	SYSTEMTIME sysTime1;
	SYSTEMTIME sysTime2;
	TimeHelper::Time2SysTime(stData.nAddTimes, sysTime1);
	TimeHelper::Time2SysTime(stData.nLastModifyTime, sysTime2);

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 - 1, szFormat, TimeHelper::GetStrTime2(sysTime1), TimeHelper::GetStrTime2(sysTime2), ++nIndex, stData.szTitle);
	std::string strTmp = StringHelper::UnicodeToUtf8(szTmp);

	Json::Reader reader;
	folder_obj.clear();
	reader.parse(strTmp.c_str(), folder_obj);

	return TRUE;
}

BOOL CChromePlugIn::MakeUrlNode(FAVORITELINEDATA stData, Json::Value& url_obj, int32& nIndex)
{
	wchar_t szFormat[] = L" {\
				 \"date_added\": \"%s\",\
				 \"id\": \"%d\",\
				 \"name\": \"%s\",\
				 \"type\": \"url\",\
				 \"url\": \"%s\"}";
	
	SYSTEMTIME sysTime;
	TimeHelper::Time2SysTime(stData.nAddTimes, sysTime);

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 -1, szFormat, TimeHelper::GetStrTime2(sysTime), ++nIndex, stData.szTitle, stData.szUrl);
	std::string strTmp = StringHelper::UnicodeToUtf8(szTmp);

	Json::Reader reader;
	url_obj.clear();
	reader.parse(strTmp.c_str(),url_obj);	

	return TRUE;
}

BOOL CChromePlugIn::MakeSpecialFolderNode(wchar_t *pszName, int32& nIndex, Json::Value& folder_obj)
{
	wchar_t szFormat[] = L" { \"date_added\": \"0\",\
						  \"date_modified\": \"%s\",\
						  \"id\": \"%d\",\
						  \"name\": \"%s\",\
						  \"type\": \"folder\" }";

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 - 1, szFormat, TimeHelper::GetCurrentStrTime2(), ++nIndex, pszName);
	std::string strTmp = StringHelper::UnicodeToUtf8(szTmp);

	Json::Reader reader;
	folder_obj.clear();
	reader.parse(strTmp.c_str(), folder_obj);

	return TRUE;
}

BOOL CChromePlugIn::EnumNode(Json::Value& folder_obj, int32& nCount)
{
	switch (folder_obj.type())
	{
	case Json::nullValue:
	case Json::intValue:
	case Json::uintValue:
	case Json::realValue:
	case Json::stringValue:
	case Json::booleanValue:
		break;
	case Json::arrayValue:
		{
			int32 nSize = folder_obj.size();
			//nCount += nSize;
			for (int32 index = 0; index < nSize; ++index)
			{
				EnumNode(folder_obj[index], nCount);
			}
		}
		break;
	case Json::objectValue:
		{
			nCount++;
			Json::Value::Members members(folder_obj.getMemberNames());
			for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
			{
				const std::string &name = *it;
				EnumNode(folder_obj[name], nCount);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL CChromePlugIn::MergeNode(PFAVORITELINEDATA pData, Json::Value& node_obj)
{
	Json::Value array_obj;
	for (int32 i = m_nMaxDepth; i >= 0; --i)
	{
		MAP_DEPTH_INFO ::iterator it = m_mapDepthInfo.lower_bound(i);
		while (it != m_mapDepthInfo.upper_bound(i))
		{
			++it;
		}
	}

	return TRUE;
}
