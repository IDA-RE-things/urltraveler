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
#include <algorithm>
#include "json/json.h"
#include "time.h"


#pragma comment(lib, "shlwapi.lib")

using namespace chromeplugin;


CChromePlugIn::CChromePlugIn(void) : m_nMaxDepth(0), m_nIndex(0)
{
	m_mapDepthInfo.clear();
	m_mapPidInfo.clear();
	m_mapPidNodeInfo.clear();
}

CChromePlugIn::~CChromePlugIn(void)
{
	m_mapDepthInfo.clear();
	m_mapPidInfo.clear();
	m_mapPidNodeInfo.clear();
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
			wstring wstrPath = wstring(szPath) + L"\\Application\\chrome.exe";
			return _wcsdup(wstrPath.c_str());
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
	return _wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//名称: GetHistoryDataPath
//描述: 获取浏览器收藏夹的历史数据对应的文件或者文件夹
//----------------------------------------------------------------------------------------
wchar_t* CChromePlugIn::GetHistoryDataPath() 
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Local\\Google\\Chrome\\User Data\\Default\\History";

	return _wcsdup(strPath.c_str());
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
 		EnumNode(chrome_bookmarks["roots"]["bookmark_bar"]["children"], nFavoriteCount);
 		EnumNode(chrome_bookmarks["roots"]["other"]["children"], nFavoriteCount);
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

	Json::Value root;
	Json::Value bookmark_bar;
	Json::Value other;
	uint32 nIndex = 0;
	int32 nDepth = 0;

	InitializeChecksum();

	MakeSpecialFolderNode(L"书签栏", nIndex, bookmark_bar);
	MakeSpecialFolderNode(L"其他书签", nIndex, other);

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

		if (pData[i].bFolder)
		{
			m_mapPidInfo.insert(MAP_PID_INFO::value_type(pData[i].nId, nDepth));
		}
		
		m_mapDepthInfo.insert(MAP_DEPTH_INFO::value_type(nDepth, i));
	}

	TraverseNode(pData, m_nMaxDepth);
	if (m_mapPidNodeInfo.size() == 1)
	{
		bookmark_bar["children"] = m_mapPidNodeInfo.begin()->second;
	}
	else
	{
		return FALSE;
	}
	
	FinalizeChecksum();

	root["checksum"] = m_strCheckSum;
	root["roots"]["bookmark_bar"] = bookmark_bar;
	root["roots"]["other"] = other;
	root["version"] = "1";

	std::ofstream outfile(strTmpPath.c_str());
	Json::StyledStreamWriter writer;
	writer.write(outfile, root);
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

	if (!wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"书签栏") 
		|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"其他书签"))
	{
		Json::Value children_nodes = folder_obj["children"];
		int32 nNodeCount = children_nodes.size();
		for (int32 i = 0; i < nNodeCount; ++i)
		{
			Json::Value new_val = children_nodes[i];
			if (new_val["type"].asString() == std::string("url"))
			{
				ExportUrl(new_val, nPid, pData, nDataNum);
			}
			else if(new_val["type"].asString() == std::string("folder"))
			{
				ExportFolder(new_val, nPid, pData, nDataNum);
			}
		}
	}
	else
	{
		pData[nDataNum].nId = nDataNum + ID_VALUE_CHROME_BEGIN;
		pData[nDataNum].bFolder = true;
		pData[nDataNum].bDelete = false;

		StringToInt64(folder_obj["date_added"].asString(), pData[nDataNum].nAddTimes);
		StringToInt64(folder_obj["date_added"].asString(), pData[nDataNum].nLastModifyTime);

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
		int32 nCurrPid = nDataNum;
		for (int32 i = 0; i < nNodeCount; ++i)
		{
			Json::Value new_val = children_nodes[i];
			if (new_val["type"].asString() == std::string("url"))
			{
				ExportUrl(new_val, nCurrPid -1 + ID_VALUE_CHROME_BEGIN, pData, nDataNum);
			}
			else if(new_val["type"].asString() == std::string("folder"))
			{
				ExportFolder(new_val, nCurrPid -1 + ID_VALUE_CHROME_BEGIN, pData, nDataNum);
			}
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
	StringToInt64(url_obj["date_added"].asString(), pData[nDataNum].nAddTimes);
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


BOOL CChromePlugIn::MakeFolderNode(FAVORITELINEDATA stData, Json::Value& folder_obj, uint32& nIndex)
{
	wchar_t szFormat[] = L" { \"children\": [ ],\
				  \"date_added\": \"%s\",\
				  \"date_modified\": \"%s\",\
				  \"id\": \"%u\",\
				  \"name\": \"%s\",\
				  \"type\": \"folder\" }";

	std::string strAddTime;
	std::string strModifyTime;
	Int64ToString(stData.nAddTimes, strAddTime);
	Int64ToString(stData.nLastModifyTime, strModifyTime);

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 - 1, szFormat, StringHelper::ANSIToUnicode(strAddTime).c_str(), \
		StringHelper::ANSIToUnicode(strModifyTime).c_str(), ++nIndex, stData.szTitle);
	std::string strTmp = StringHelper::UnicodeToUtf8(szTmp);

	char szId[256] = {0};
	sprintf_s(szId, 255, "%u", nIndex);
	UpdateChecksumWithFolderNode(&szId[0], stData.szTitle);

	Json::Reader reader;
	folder_obj.clear();
	reader.parse(strTmp.c_str(), folder_obj);

	return TRUE;
}

BOOL CChromePlugIn::MakeUrlNode(FAVORITELINEDATA stData, Json::Value& url_obj, uint32& nIndex)
{
	wchar_t szFormat[] = L" {\
				 \"date_added\": \"%s\",\
				 \"id\": \"%u\",\
				 \"name\": \"%s\",\
				 \"type\": \"url\",\
				 \"url\": \"%s\"}";
	
	std::string strAddTime;
	Int64ToString(stData.nAddTimes, strAddTime);

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 -1, szFormat, StringHelper::ANSIToUnicode(strAddTime).c_str(), \
		++nIndex, stData.szTitle, stData.szUrl);
	std::string strTmp = StringHelper::UnicodeToUtf8(szTmp);

	char szId[256] = {0};
	sprintf_s(szId, 255, "%u", nIndex);
	UpdateChecksumWithUrlNode(&szId[0],stData.szTitle, StringHelper::UnicodeToUtf8(stData.szUrl));

	Json::Reader reader;
	url_obj.clear();
	reader.parse(strTmp.c_str(),url_obj);	

	return TRUE;
}

BOOL CChromePlugIn::MakeSpecialFolderNode(wchar_t *pszName, uint32& nIndex, Json::Value& folder_obj)
{
	wchar_t szFormat[] = L" { \"children\": [ ],\
						  \"date_added\": \"%s\",\
						  \"date_modified\": \"%s\",\
						  \"id\": \"%u\",\
						  \"name\": \"%s\",\
						  \"type\": \"folder\" }";

	wchar_t szTmp[2048];
	std::string strTime;
	Int64ToString(time(NULL), strTime);
	swprintf_s(szTmp, 2048 - 1, szFormat, StringHelper::ANSIToUnicode(strTime).c_str(), StringHelper::ANSIToUnicode(strTime).c_str(), ++nIndex, pszName);
	std::string strTmp = StringHelper::UnicodeToUtf8(szTmp);

	
// 	char szId[256] = {0};
// 	sprintf_s(szId, 255, "%u", nIndex);
// 	UpdateChecksumWithFolderNode(&szId[0], pszName);

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


BOOL CChromePlugIn::TraverseNode(PFAVORITELINEDATA pData, int32 nDepth)
{
	if (nDepth >= 0)
	{
		std::vector<int32> vecPidList;
		std::multimap<int32, NODEINFO> mapPidObj;
		for (MAP_DEPTH_INFO ::iterator it = m_mapDepthInfo.lower_bound(nDepth); it != m_mapDepthInfo.upper_bound(nDepth); ++it)
		{
			NODEINFO stNodeInfo = {0};	
			if (pData[(*it).second].bFolder == true)
			{
				Json::Value folder_obj;
				MakeFolderNode(pData[(*it).second], folder_obj, m_nIndex);
				stNodeInfo.nIndex = (*it).second;
				stNodeInfo.node_obj = folder_obj;
				mapPidObj.insert(std::multimap<int32, NODEINFO>::value_type(pData[(*it).second].nPid, stNodeInfo));
			}
			else
			{
				Json::Value url_obj;
				MakeUrlNode(pData[(*it).second], url_obj, m_nIndex);
				stNodeInfo.nIndex = (*it).second;
				stNodeInfo.node_obj = url_obj;
				mapPidObj.insert(std::multimap<int32, NODEINFO>::value_type(pData[(*it).second].nPid, stNodeInfo));
			}

			std::vector<int32>::iterator itPidList = std::find(vecPidList.begin(), vecPidList.end(), pData[(*it).second].nPid);
			if (itPidList == vecPidList.end())
			{
				vecPidList.push_back(pData[(*it).second].nPid);
			}	
		}
		
		std::vector<int32>::iterator itrPid;
		for (itrPid = vecPidList.begin(); itrPid != vecPidList.end(); ++itrPid)
		{
			Json::Value array_obj;
			for (std::multimap<int32, NODEINFO>::iterator itr = mapPidObj.lower_bound(*itrPid); itr != mapPidObj.upper_bound(*itrPid); ++itr)
			{
				if (pData[(*itr).second.nIndex].bFolder)
				{
					MAP_PID_NODE_INFO::iterator it = m_mapPidNodeInfo.find(pData[(*itr).second.nIndex].nId);
					if (it != m_mapPidNodeInfo.end())
					{
						(*itr).second.node_obj["children"] = (*it).second;
						m_mapPidNodeInfo.erase(pData[(*itr).second.nIndex].nId);
					}
					
				}
				array_obj.append((*itr).second.node_obj);
			}

			m_mapPidNodeInfo.insert(MAP_PID_NODE_INFO::value_type(*itrPid, array_obj));
		}

		TraverseNode(pData, --nDepth);
	}

	return TRUE;
}

BOOL CChromePlugIn::StringToInt64(std::string strTime, int64& nTime)
{
	for (std::string::iterator it = strTime.begin(); it != strTime.end(); ++it)
	{
		uint8 nNum = 0;
		if ((*it >= '0') && (*it <= '9'))
		{
			nNum = *it - '0';
		}
		else
		{
			return FALSE;
		}

		if (it != strTime.begin())
		{
			nTime *= 10;
		}

		nTime += nNum;
	}

	return TRUE;
}

BOOL CChromePlugIn::Int64ToString(int64 nTime, std::string& strTime)
{
	int64 nMod = 0;
	int64 nQuot = nTime;
	char szMod[3] = {0};
	while (nQuot != 0)
	{
		nMod = nQuot % 10;	
		_i64toa(nMod, &szMod[0], 10);
		strTime.append(std::string(&szMod[0]));
		nQuot = nQuot / 10;
	}
	
	std::reverse(strTime.begin(), strTime.end());
	return TRUE;
}

void CChromePlugIn::UpdateChecksum(const std::string& str) 
{
	MD5Update(&m_szMd5Context, str.data(), str.length() * sizeof(char));
}

void CChromePlugIn::UpdateChecksum(const std::wstring& str) 
{
	MD5Update(&m_szMd5Context, str.data(), str.length() * sizeof(uint16));
}

void CChromePlugIn::UpdateChecksumWithUrlNode(const std::string& id, const std::wstring& title, const std::string& url) 
{
	UpdateChecksum(id);
	UpdateChecksum(title);
	UpdateChecksum("url");
	UpdateChecksum(url);
}

void CChromePlugIn::UpdateChecksumWithFolderNode(const std::string& id, const std::wstring& title) 
{
	 UpdateChecksum(id);
	 UpdateChecksum(title);
	 UpdateChecksum("folder");
}

void CChromePlugIn::InitializeChecksum() 
{
	MD5Init(&m_szMd5Context);
}

void CChromePlugIn::FinalizeChecksum() 
{
	MD5Digest digest;
	MD5Final(&digest, &m_szMd5Context);
	m_strCheckSum = MD5DigestToBase16(digest);
}