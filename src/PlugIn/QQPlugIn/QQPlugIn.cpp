#include "stdafx.h"
#include "PlugIn.h"
#include "QQPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include <algorithm>
#include "CRCHash.h"
#include "QQPlugInFactory.h"

#include <string>
#include <stack>
#include <fstream>
#include <algorithm>
#include "json/json.h"
#include "time.h"
#include "icu/icu_utf.h"

#pragma comment(lib, "shlwapi.lib")


QQPlugIn::QQPlugIn() : m_nMaxDepth(0), m_nIndex(0)
{
	m_mapDepthInfo.clear();
	m_mapPidInfo.clear();
	m_mapPidNodeInfo.clear();
}


QQPlugIn::~QQPlugIn()
{
	m_mapDepthInfo.clear();
	m_mapPidInfo.clear();
	m_mapPidNodeInfo.clear();
}

BOOL QQPlugIn::Load()
{
	return TRUE;
}

BOOL QQPlugIn::UnLoad()
{
	return TRUE;
}

int32 QQPlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* QQPlugIn::GetBrowserName()
{
	return L"QQ浏览器";
}

wchar_t* QQPlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\QQBrowser",
		L"DisplayIcon", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		return _wcsdup(szPath);
	}

	return NULL;
}

wchar_t* QQPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Tencent\\QQBrowser\\user_data\\0\\Bookmarks";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

wchar_t* QQPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Tencent\\QQBrowser\\user_data\\0\\history.db";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//名称: GetFavoriteCount
//描述: 获取浏览器中收藏网址的条数 
//返回:
//      回返收藏网址条数
//----------------------------------------------------------------------------------------
int32 QQPlugIn::GetFavoriteCount()
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
		//EnumNode(chrome_bookmarks["roots"]["other"]["children"], nFavoriteCount);
		//EnumNode(chrome_bookmarks["roots"]["synced"]["children"], nFavoriteCount);
	}

	infile.close();
	return nFavoriteCount;
}

//----------------------------------------------------------------------------------------
//名称: ExportFavoriteData
//描述: 导出当前浏览器的所有的收藏夹数据
//参数: 
//		@param	ppData			导出的收藏夹数据数组
//		@param	nDataNum		导出的收藏夹条目的条数
//----------------------------------------------------------------------------------------
BOOL QQPlugIn::ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
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
			ExportFolder(roots["bookmark_bar"], 0, ppData, nDataNum);
			//ExportFolder(roots["other"], 0, pData, nDataNum);
			//ExportFolder(roots["synced"], 0, pData, nDataNum);
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
//		@param	ppData			需要导入的的收藏夹数据数组
//		@param	nDataNum		需要导入的收藏夹条目的条数
//----------------------------------------------------------------------------------------
BOOL QQPlugIn::ImportFavoriteData(PFAVORITELINEDATA* ppData, int32 nDataNum)
{
	if ((ppData == NULL || *ppData == NULL) || (nDataNum == 0))
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
	//Json::Value other;
	//Json::Value synced;
	uint32 nIndex = 0;
	int32 nDepth = 0;

	//InitializeChecksum();

	MakeSpecialFolderNode(L"收藏夹", m_nIndex, bookmark_bar);
	SortByDepth(&ppData[0], nDataNum);
	for (int32 i = 0; i < nDataNum; ++i)
	{
		if (ppData[i]->bDelete == true)
		{
			continue;
		}

		MAP_PID_INFO::iterator it;
		it = m_mapPidInfo.find(ppData[i]->nPid);
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

		if (ppData[i]->bFolder)
		{
			m_mapPidInfo.insert(MAP_PID_INFO::value_type(ppData[i]->nId, nDepth));
		}

		m_mapDepthInfo.insert(MAP_DEPTH_INFO::value_type(nDepth, i));
	}

	TraverseNode(ppData, m_nMaxDepth);
	if (m_mapPidNodeInfo.size() == 1)
	{
		bookmark_bar["children"] = m_mapPidNodeInfo.begin()->second;
	}
	else
	{
		return FALSE;
	}

	MAP_ID_INDEX_INFO::iterator itIdIndex;
	for (int k = 0; k <= nDataNum; k++)
	{
		itIdIndex = m_mapIdIndexInfo.find(ppData[k]->nId);
		if (itIdIndex != m_mapIdIndexInfo.end())
		{
			int32 nIndex = (*itIdIndex).second; 
			if (ppData[k]->bFolder)
			{
				m_mapPidInfo.insert(MAP_PID_INFO::value_type(ppData[k]->nId, nDepth));
// 				char szId[256] = {0};
// 				sprintf_s(szId, 255, "%u", nIndex);
// 				UpdateChecksumWithFolderNode(&szId[0], pData[k].szTitle);
			}
			else
			{
// 				char szId[256] = {0};
// 				sprintf_s(szId, 255, "%u", nIndex);
// 				UpdateChecksumWithUrlNode(&szId[0], pData[k].szTitle, StringHelper::UnicodeToUtf8(pData[k].szUrl));
			}
		}
	}

	//MakeSpecialFolderNode(L"Other bookmarks", m_nIndex, other);
	//MakeSpecialFolderNode(L"Synced bookmarks", m_nIndex, synced);

	//FinalizeChecksum();

	//root["checksum"] = m_strCheckSum;
	root["favbar_folder_id"] = -1;
	root["roots"]["bookmark_bar"] = bookmark_bar;
	//root["roots"]["other"] = other;
	//root["roots"]["synced"] = synced;
	//root["version"] = 1;

	std::ofstream outfile(strTmpPath.c_str());
	Json::StyledStreamWriter writer;
	writer.write(outfile, root);	
	outfile.close();

	return TRUE;
}

void QQPlugIn::SortByDepth(PFAVORITELINEDATA* ppData, int32 nDataNum)
{
	PFAVORITELINEDATA* ppSortLineData = new PFAVORITELINEDATA[nDataNum];
	memset(ppSortLineData, 0, sizeof(PFAVORITELINEDATA) * nDataNum);

	PFAVORITELINEDATA* ppSortLineDataPos = ppSortLineData;

	// 逐一找到合适的数据，并插入到pSortLineData中去
	SortNode(ppData, nDataNum, ppSortLineDataPos, 0);

	// 排序后的数据拷贝
	memcpy(ppData, ppSortLineData, nDataNum * sizeof(PFAVORITELINEDATA));
	delete[] ppSortLineData;
	ppSortLineData = NULL;
}

void QQPlugIn::SortNode(PFAVORITELINEDATA* ppData, int32 nDataNum, PFAVORITELINEDATA*& ppSortData, int32 nParentId)
{
	static int k = 0;

	for (int i = 0; i < nDataNum; i++)
	{
		if (ppData[i]->nPid == nParentId)
		{
			memcpy(ppSortData++, &ppData[i], sizeof(PFAVORITELINEDATA));
			if (++k >= nDataNum)
			{//排序完所有数据，则直接退出循环，以提高效率
				break;
			}

			if (ppData[i]->bFolder)
			{
				SortNode(ppData, nDataNum, ppSortData, ppData[i]->nId);
			}

		}
	}
}


BOOL QQPlugIn::ExportFolder(Json::Value& folder_obj, int32 nPid, PFAVORITELINEDATA* ppData, int32& nDataNum)
{
	if (folder_obj.empty() || (folder_obj["type"].asString() != std::string("folder")))
	{
		return FALSE;
	}

	if (!wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"收藏夹"))
		//|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"书签栏"))
		//|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"Other bookmarks")
		//|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"其他书签")
		//|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"Synced bookmarks"))
	{
		Json::Value children_nodes = folder_obj["children"];
		int32 nNodeCount = children_nodes.size();
		for (int32 i = 0; i < nNodeCount; ++i)
		{
			Json::Value new_val = children_nodes[i];
			if (new_val["type"].asString() == std::string("url"))
			{
				ExportUrl(new_val, nPid, ppData, nDataNum);
			}
			else if(new_val["type"].asString() == std::string("folder"))
			{
				ExportFolder(new_val, nPid, ppData, nDataNum);
			}
		}
	}
	else
	{
		ppData[nDataNum]->nId = nDataNum + ID_VALUE_CHROME_BEGIN;
		ppData[nDataNum]->bFolder = true;
		ppData[nDataNum]->bDelete = false;

		StringToInt64(folder_obj["date_added"].asString(), ppData[nDataNum]->nAddTimes);
		StringToInt64(folder_obj["date_added"].asString(), ppData[nDataNum]->nLastModifyTime);

		ppData[nDataNum]->nPid = nPid;

		wcscpy_s(ppData[nDataNum]->szTitle, MAX_PATH -1, StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str());
		ppData[nDataNum]->szUrl[0] = 0;
		ppData[nDataNum]->nCatId = 0;

		CCRCHash ojbCrcHash;
		ojbCrcHash.GetHash((BYTE *)ppData[nDataNum]->szTitle, wcslen(ppData[nDataNum]->szTitle) * sizeof(wchar_t),  \
			(BYTE *)&ppData[nDataNum]->nHashId, sizeof(int32));

		nDataNum++;

		Json::Value children_nodes = folder_obj["children"];
		int32 nNodeCount = children_nodes.size();
		int32 nCurrPid = nDataNum;
		for (int32 i = 0; i < nNodeCount; ++i)
		{
			Json::Value new_val = children_nodes[i];
			if (new_val["type"].asString() == std::string("url"))
			{
				ExportUrl(new_val, nCurrPid -1 + ID_VALUE_CHROME_BEGIN, ppData, nDataNum);
			}
			else if(new_val["type"].asString() == std::string("folder"))
			{
				ExportFolder(new_val, nCurrPid -1 + ID_VALUE_CHROME_BEGIN, ppData, nDataNum);
			}
		}
	}

	return TRUE;
}

BOOL QQPlugIn::ExportUrl(Json::Value& url_obj, int32 nPid, PFAVORITELINEDATA* ppData, int32& nDataNum)
{
	if (url_obj.empty() || url_obj["type"].asString() != std::string("url"))
	{
		return FALSE;
	}

	ppData[nDataNum]->nId = nDataNum + ID_VALUE_CHROME_BEGIN;
	ppData[nDataNum]->bFolder = false;
	ppData[nDataNum]->bDelete = false;
	StringToInt64(url_obj["date_added"].asString(), ppData[nDataNum]->nAddTimes);
	ppData[nDataNum]->nLastModifyTime =  0;
	ppData[nDataNum]->nPid = nPid;
	ppData[nDataNum]->nCatId = 0;

	wcscpy_s(ppData[nDataNum]->szTitle, MAX_PATH -1, StringHelper::Utf8ToUnicode(url_obj["name"].asString()).c_str());
	wcscpy_s(ppData[nDataNum]->szUrl, 1024 - 1, StringHelper::Utf8ToUnicode(url_obj["url"].asString()).c_str());
	ppData[nDataNum]->szUrl[1023] = 0;

	CCRCHash ojbCrcHash;
	ojbCrcHash.GetHash((BYTE *)ppData[nDataNum]->szTitle, wcslen(ppData[nDataNum]->szTitle) * sizeof(wchar_t),  \
		(BYTE *)&ppData[nDataNum]->nHashId, sizeof(int32));

	nDataNum++;

	return TRUE;
}


BOOL QQPlugIn::MakeFolderNode(FAVORITELINEDATA* pstData, Json::Value& folder_obj, uint32& nIndex)
{
	wchar_t szFormat[] = L" { \"children\": [ ],\
						  \"date_added\": \"%s\",\
						  \"date_modified\": \"%s\",\
						  \"id\": \"%u\",\
						  \"name\": \"%s\",\
						  \"type\": \"folder\" }";

	std::string strAddTime;
	std::string strModifyTime;
	Int64ToString(pstData->nAddTimes, strAddTime);
	Int64ToString(pstData->nLastModifyTime, strModifyTime);

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 - 1, szFormat, StringHelper::Utf8ToUnicode(strAddTime).c_str(), 
		StringHelper::Utf8ToUnicode(strModifyTime).c_str(), ++nIndex, pstData->szTitle);

	Json::Reader reader;
	folder_obj.clear();
	reader.parse(StringHelper::UnicodeToUtf8(szTmp), folder_obj);

	return TRUE;
}

BOOL QQPlugIn::MakeUrlNode(FAVORITELINEDATA* pstData, Json::Value& url_obj, uint32& nIndex)
{
	wchar_t szFormat[] = L" {\
						  \"date_added\": \"%s\",\
						  \"id\": \"%u\",\
						  \"name\": \"%s\",\
						  \"type\": \"url\",\
						  \"url\": \"%s\"}";

	std::string strAddTime;
	Int64ToString(pstData->nAddTimes, strAddTime);

	wchar_t szTmp[2048];
	swprintf_s(szTmp, 2048 -1, szFormat, StringHelper::Utf8ToUnicode(strAddTime).c_str(), ++nIndex, pstData->szTitle, pstData->szUrl);

	Json::Reader reader;
	url_obj.clear();
	reader.parse(StringHelper::UnicodeToUtf8(szTmp), url_obj);	

	return TRUE;
}


BOOL QQPlugIn::MakeSpecialFolderNode(wchar_t *pszName, uint32& nIndex, Json::Value& folder_obj)
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
	swprintf_s(szTmp, 2048 - 1, szFormat, StringHelper::Utf8ToUnicode(strTime).c_str(), 
		StringHelper::Utf8ToUnicode(strTime).c_str(), ++nIndex, pszName);


	char szId[256] = {0};
	sprintf_s(szId, 255, "%u", nIndex);
	UpdateChecksumWithFolderNode(&szId[0], pszName);

	Json::Reader reader;
	folder_obj.clear();
	reader.parse(StringHelper::UnicodeToUtf8(szTmp), folder_obj);

	return TRUE;
}


BOOL QQPlugIn::EnumNode(Json::Value& folder_obj, int32& nCount)
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


BOOL QQPlugIn::TraverseNode(PFAVORITELINEDATA* ppData, int32 nDepth)
{
	if (nDepth >= 0)
	{
		std::vector<int32> vecPidList;
		std::multimap<int32, NODEINFO> mapPidObj;
		for (MAP_DEPTH_INFO ::iterator it = m_mapDepthInfo.lower_bound(nDepth); it != m_mapDepthInfo.upper_bound(nDepth); ++it)
		{
			NODEINFO stNodeInfo = {0};	
			if (ppData[(*it).second]->bFolder == true)
			{
				Json::Value folder_obj;
				MakeFolderNode(ppData[(*it).second], folder_obj, m_nIndex);
				stNodeInfo.nIndex = (*it).second;
				stNodeInfo.node_obj = folder_obj;
				mapPidObj.insert(std::multimap<int32, NODEINFO>::value_type(ppData[(*it).second]->nPid, stNodeInfo));
			}
			else
			{
				Json::Value url_obj;
				MakeUrlNode(ppData[(*it).second], url_obj, m_nIndex);
				stNodeInfo.nIndex = (*it).second;
				stNodeInfo.node_obj = url_obj;
				mapPidObj.insert(std::multimap<int32, NODEINFO>::value_type(ppData[(*it).second]->nPid, stNodeInfo));
			}

			std::vector<int32>::iterator itPidList = std::find(vecPidList.begin(), vecPidList.end(), ppData[(*it).second]->nPid);
			if (itPidList == vecPidList.end())
			{
				vecPidList.push_back(ppData[(*it).second]->nPid);
			}	

			m_mapIdIndexInfo.insert(MAP_ID_INDEX_INFO::value_type(ppData[(*it).second]->nId, m_nIndex));
		}

		std::vector<int32>::iterator itrPid;
		for (itrPid = vecPidList.begin(); itrPid != vecPidList.end(); ++itrPid)
		{
			Json::Value array_obj;
			for (std::multimap<int32, NODEINFO>::iterator itr = mapPidObj.lower_bound(*itrPid); itr != mapPidObj.upper_bound(*itrPid); ++itr)
			{
				if (ppData[(*itr).second.nIndex]->bFolder)
				{
					MAP_PID_NODE_INFO::iterator it = m_mapPidNodeInfo.find(ppData[(*itr).second.nIndex]->nId);
					if (it != m_mapPidNodeInfo.end())
					{
						(*itr).second.node_obj["children"] = (*it).second;
						m_mapPidNodeInfo.erase(ppData[(*itr).second.nIndex]->nId);
					}

				}
				array_obj.append((*itr).second.node_obj);
			}

			m_mapPidNodeInfo.insert(MAP_PID_NODE_INFO::value_type(*itrPid, array_obj));
		}

		TraverseNode(ppData, --nDepth);
	}

	return TRUE;
}

BOOL QQPlugIn::StringToInt64(std::string strTime, int64& nTime)
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

BOOL QQPlugIn::Int64ToString(int64 nTime, std::string& strTime)
{
	int64 nMod = 0;
	int64 nQuot = nTime;
	char szMod[3] = {0};
	while (nQuot != 0)
	{
		nMod = nQuot % 10;	
		_i64toa_s(nMod, &szMod[0], sizeof(szMod), 10);
		strTime.append(std::string(&szMod[0]));
		nQuot = nQuot / 10;
	}

	std::reverse(strTime.begin(), strTime.end());
	return TRUE;
}

void QQPlugIn::UpdateChecksum(const std::string& str) 
{
	MD5Update(&m_szMd5Context, str.data(), str.length() * sizeof(char));
}

void QQPlugIn::UpdateChecksum(const std::wstring& str) 
{
	MD5Update(&m_szMd5Context, str.data(), str.length() * sizeof(uint16));
}

void QQPlugIn::UpdateChecksumWithUrlNode(const std::string& id, const std::wstring& title, const std::string& url) 
{
	if (IsStringUTF8(url))
	{
		UpdateChecksum(id);
		UpdateChecksum(title);
		UpdateChecksum("url");
		UpdateChecksum(url);
	}
}

void QQPlugIn::UpdateChecksumWithFolderNode(const std::string& id, const std::wstring& title) 
{
	UpdateChecksum(id);
	UpdateChecksum(title);
	UpdateChecksum("folder");
}

void QQPlugIn::InitializeChecksum() 
{
	MD5Init(&m_szMd5Context);
}

void QQPlugIn::FinalizeChecksum() 
{
	MD5Digest digest;
	MD5Final(&digest, &m_szMd5Context);
	m_strCheckSum = MD5DigestToBase16(digest);
}

inline bool QQPlugIn::IsValidCharacter(uint32 code_point) 
{
	// Excludes non-characters (U+FDD0..U+FDEF, and all codepoints ending in
	// 0xFFFE or 0xFFFF) from the set of valid code points.
	return code_point < 0xD800u || (code_point >= 0xE000u &&
		code_point < 0xFDD0u) || (code_point > 0xFDEFu &&
		code_point <= 0x10FFFFu && (code_point & 0xFFFEu) != 0xFFFEu);
}

bool QQPlugIn::IsStringUTF8(const std::string& str) 
{
	const char *src = str.data();
	int32 src_len = static_cast<int32>(str.length());
	int32 char_index = 0;

	while (char_index < src_len) 
	{
		int32 code_point;
		CBU8_NEXT(src, char_index, src_len, code_point);
		if (!IsValidCharacter(code_point))
		{
			return false;
		}
	}
	return true;
}