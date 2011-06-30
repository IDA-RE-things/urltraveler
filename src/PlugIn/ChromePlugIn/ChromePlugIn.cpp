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
#include "icu_utf.h"


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
//����: GetPlugInVersion
//����: ��ȡ��ǰ����İ汾��
//����: ����İ汾�ţ�ͨ��Ϊһ������
//----------------------------------------------------------------------------------------
int32 CChromePlugIn::GetPlugInVersion() 
{
	return 0;
}

//----------------------------------------------------------------------------------------
//����: GetBrowserName
//����: ��ȡ�ò����Ӧ������������ƺͰ汾
//----------------------------------------------------------------------------------------
const wchar_t* CChromePlugIn::GetBrowserName() 
{
	return L"Chrome";
}

//----------------------------------------------------------------------------------------
//����: GetInstallPath
//����: ��ȡ�����Ӧ��������İ�װĿ¼
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
//����: GetFavoriteDataPath
//����: ��ȡ������ղؼж�Ӧ���ļ������ļ��е�·��
//----------------------------------------------------------------------------------------
wchar_t* CChromePlugIn::GetFavoriteDataPath() 
{
	std::wstring strPath = PathHelper::GetLocalAppDataDir() + L"\\Google\\Chrome\\User Data\\Default\\Bookmarks";

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©
	return _wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//����: GetHistoryDataPath
//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
//----------------------------------------------------------------------------------------
wchar_t* CChromePlugIn::GetHistoryDataPath() 
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Local\\Google\\Chrome\\User Data\\Default\\History";

	return _wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//����: GetFavoriteCount
//����: ��ȡ��������ղ���ַ������ 
//����:
//      �ط��ղ���ַ����
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
		EnumNode(chrome_bookmarks["roots"]["synced"]["children"], nFavoriteCount);
	}

	infile.close();
	return nFavoriteCount;
}

//----------------------------------------------------------------------------------------
//����: ExportFavoriteData
//����: ������ǰ����������е��ղؼ�����
//����: 
//		@param	pData			�������ղؼ���������
//		@param	nDataNum		�������ղؼ���Ŀ������
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
			ExportFolder(roots["synced"], 0, pData, nDataNum);
		}
		
		bRetCode = TRUE;
	}

	infile.close();

	return bRetCode;
}

//----------------------------------------------------------------------------------------
//����: ImportFavoriteData
//����: ����ǰ�ļ�¼���뵽�������
//����: 
//		@param	pData			��Ҫ����ĵ��ղؼ���������
//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
//----------------------------------------------------------------------------------------
BOOL CChromePlugIn::ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum)
{
	if ((pData == NULL) || (nDataNum == 0))
	{
		return FALSE;
	}

	wchar_t* pszPath = GetFavoriteDataPath();
	//��ȡ�����ļ�·��
	std::string strTmpPath = StringHelper::UnicodeToUtf8(pszPath);
	//����ǰ��ɾ��֮ǰ���ղؼ��ļ�
	BOOL bResult = ::DeleteFileW(pszPath);
	free(pszPath);
	if (!bResult)
	{
		return FALSE;
	}

	Json::Value root;
	Json::Value bookmark_bar;
	Json::Value other;
	Json::Value synced;
	uint32 nIndex = 0;
	int32 nDepth = 0;

	InitializeChecksum();

	MakeSpecialFolderNode(L"Bookmarks bar", m_nIndex, bookmark_bar);
	SortByDepth(&pData[0], nDataNum);
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

	MAP_ID_INDEX_INFO::iterator itIdIndex;
	for (int k = 0; k <= nDataNum; k++)
	{
		itIdIndex = m_mapIdIndexInfo.find(pData[k].nId);
		if (itIdIndex != m_mapIdIndexInfo.end())
		{
			int32 nIndex = (*itIdIndex).second; 
			if (pData[k].bFolder)
			{
				m_mapPidInfo.insert(MAP_PID_INFO::value_type(pData[k].nId, nDepth));
				char szId[256] = {0};
				sprintf_s(szId, 255, "%u", nIndex);
				UpdateChecksumWithFolderNode(&szId[0], pData[k].szTitle);
			}
			else
			{
				char szId[256] = {0};
				sprintf_s(szId, 255, "%u", nIndex);
				UpdateChecksumWithUrlNode(&szId[0], pData[k].szTitle, StringHelper::UnicodeToUtf8(pData[k].szUrl));
			}
		}
	}

	MakeSpecialFolderNode(L"Other bookmarks", m_nIndex, other);
	MakeSpecialFolderNode(L"Synced bookmarks", m_nIndex, synced);
	
	FinalizeChecksum();

	root["checksum"] = m_strCheckSum;
	root["roots"]["bookmark_bar"] = bookmark_bar;
	root["roots"]["other"] = other;
	root["roots"]["synced"] = synced;
	root["version"] = 1;

	std::ofstream outfile(strTmpPath.c_str());
	Json::StyledStreamWriter writer;
	writer.write(outfile, root);	
	outfile.close();

	return TRUE;
}

void CChromePlugIn::SortByDepth(PFAVORITELINEDATA pData, int32 nDataNum)
{
	FAVORITELINEDATA* pSortLineData = new FAVORITELINEDATA[nDataNum];
	memset(pSortLineData, 0, sizeof(FAVORITELINEDATA) * nDataNum);

	FAVORITELINEDATA* pSortLineDataPos = pSortLineData;

	// ��һ�ҵ����ʵ����ݣ������뵽pSortLineData��ȥ
	SortNode(pData, nDataNum, pSortLineDataPos, 0);

	// ���������ݿ���
	memcpy(pData, pSortLineData, nDataNum * sizeof(FAVORITELINEDATA));
	delete[] pSortLineData;
	pSortLineData = NULL;
}

void CChromePlugIn::SortNode(PFAVORITELINEDATA pData, int32 nDataNum, PFAVORITELINEDATA& pSortData, int32 nParentId)
{
	static int k = 0;

	for (int i = 0; i < nDataNum; i++)
	{
		if (pData[i].nPid == nParentId)
		{
			memcpy(pSortData++, &pData[i], sizeof(FAVORITELINEDATA));
			if (++k >= nDataNum)
			{//�������������ݣ���ֱ���˳�ѭ���������Ч��
				break;
			}

			if (pData[i].bFolder)
			{
				SortNode(pData, nDataNum, pSortData, pData[i].nId);
			}

		}
	}
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

	if (!wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"Bookmarks bar") 
		|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"��ǩ��")
		|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"Other bookmarks")
		|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"������ǩ")
		|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"Synced bookmarks"))
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
	swprintf_s(szTmp, 2048 - 1, szFormat, StringHelper::Utf8ToUnicode(strAddTime).c_str(), StringHelper::Utf8ToUnicode(strModifyTime).c_str(), ++nIndex, stData.szTitle);

	Json::Reader reader;
	folder_obj.clear();
	reader.parse(StringHelper::UnicodeToUtf8(szTmp), folder_obj);

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
	swprintf_s(szTmp, 2048 -1, szFormat, StringHelper::Utf8ToUnicode(strAddTime).c_str(), ++nIndex, stData.szTitle, stData.szUrl);

	Json::Reader reader;
	url_obj.clear();
	reader.parse(StringHelper::UnicodeToUtf8(szTmp), url_obj);	

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
	swprintf_s(szTmp, 2048 - 1, szFormat, StringHelper::Utf8ToUnicode(strTime).c_str(), StringHelper::Utf8ToUnicode(strTime).c_str(), ++nIndex, pszName);


	char szId[256] = {0};
	sprintf_s(szId, 255, "%u", nIndex);
	UpdateChecksumWithFolderNode(&szId[0], pszName);

	Json::Reader reader;
	folder_obj.clear();
	reader.parse(StringHelper::UnicodeToUtf8(szTmp), folder_obj);

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

			m_mapIdIndexInfo.insert(MAP_ID_INDEX_INFO::value_type(pData[(*it).second].nId, m_nIndex));
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
	if (IsStringUTF8(url))
	{
		UpdateChecksum(id);
		UpdateChecksum(title);
		UpdateChecksum("url");
		UpdateChecksum(url);
	}
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

inline bool CChromePlugIn::IsValidCharacter(uint32 code_point) 
{
	// Excludes non-characters (U+FDD0..U+FDEF, and all codepoints ending in
	// 0xFFFE or 0xFFFF) from the set of valid code points.
	return code_point < 0xD800u || (code_point >= 0xE000u &&
		code_point < 0xFDD0u) || (code_point > 0xFDEFu &&
		code_point <= 0x10FFFFu && (code_point & 0xFFFEu) != 0xFFFEu);
}

bool CChromePlugIn::IsStringUTF8(const std::string& str) 
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

