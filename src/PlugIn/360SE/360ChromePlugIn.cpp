#include "stdafx.h"
#include "PlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include <algorithm>
#include "CRCHash.h"
#include "PathHelper.h"
#include "FileHelper.h"

#include "360ChromePlugIn.h"
#include "360SEPlugInFactory.h"

#include <string>
#include <stack>
#include <fstream>
#include "json/json.h"
#include "time.h"
#include "icu/icu_utf.h"


using namespace n360seplugin;

#pragma comment(lib, "shlwapi.lib")


C360ChromePlugIn::C360ChromePlugIn() : m_nMaxDepth(0), m_nIndex(0)
{
	m_mapDepthInfo.clear();
	m_mapPidInfo.clear();
	m_mapPidNodeInfo.clear();
}


C360ChromePlugIn::~C360ChromePlugIn()
{
	m_mapDepthInfo.clear();
	m_mapPidInfo.clear();
	m_mapPidNodeInfo.clear();
}

BOOL C360ChromePlugIn::Load()
{
	return TRUE;
}

BOOL C360ChromePlugIn::UnLoad()
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: GetPlugInVersion
//����: ��ȡ��ǰ����İ汾��
//����: ����İ汾�ţ�ͨ��Ϊһ������
//----------------------------------------------------------------------------------------
int32 C360ChromePlugIn::GetPlugInVersion()
{
	return 1;
}

//----------------------------------------------------------------------------------------
//����: GetBrowserName
//����: ��ȡ�ò����Ӧ������������ƺͰ汾
//----------------------------------------------------------------------------------------
const wchar_t* C360ChromePlugIn::GetBrowserName()
{
	return L"360Chrome";
}

//----------------------------------------------------------------------------------------
//����: GetInstallPath
//����: ��ȡ�����Ӧ��������İ�װĿ¼
//----------------------------------------------------------------------------------------
wchar_t* C360ChromePlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\360chrome.exe",
		L"Path", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		swprintf_s(szPath,MAX_PATH-1, L"%s%s", szPath, L"\\360chrome.exe"); 
		return _wcsdup(szPath);
	}

	return NULL;
}

//----------------------------------------------------------------------------------------
//����: GetFavoriteDataPath
//����: ��ȡ������ղؼж�Ӧ���ļ������ļ��е�·��
//----------------------------------------------------------------------------------------
wchar_t* C360ChromePlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetLocalAppDataDir() + wstring(L"\\360Chrome\\Chrome\\User Data\\Default\\Bookmarks");

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©
	return _wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//����: GetHistoryDataPath
//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
//----------------------------------------------------------------------------------------
wchar_t* C360ChromePlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetLocalAppDataDir() + wstring(L"\\360Chrome\\Chrome\\User Data\\Default\\History");

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©
	return _wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//����: GetFavoriteCount
//����: ��ȡ��������ղ���ַ������ 
//����:
//      �ط��ղ���ַ����
//----------------------------------------------------------------------------------------
int32 C360ChromePlugIn::GetFavoriteCount()
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
		// ����Ƿ����bookmark_bar
		if( chrome_bookmarks["roots"].isMember("bookmark_bar") == true)
		{
			EnumNode(chrome_bookmarks["roots"]["bookmark_bar"]["children"], nFavoriteCount);
			nFavoriteCount ++;
		}

		if( chrome_bookmarks["roots"].isMember("other") == true)
		{
			EnumNode(chrome_bookmarks["roots"]["other"]["children"], nFavoriteCount);
			nFavoriteCount ++;
		}

		if( chrome_bookmarks["roots"].isMember("other") == true)
		{
			EnumNode(chrome_bookmarks["roots"]["synced"]["children"], nFavoriteCount);
			nFavoriteCount ++;
		}
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
BOOL C360ChromePlugIn::ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
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
			int nRealDataNum = 0;
			Json::Value roots = chrome_bookmarks["roots"];
			ExportFolder(roots["bookmark_bar"], 0, ppData, nDataNum, nRealDataNum);
			ExportFolder(roots["other"], 0, ppData, nDataNum, nRealDataNum);
			ExportFolder(roots["synced"], 0, ppData, nDataNum, nRealDataNum);
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
//		@param	ppData			��Ҫ����ĵ��ղؼ���������
//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
//----------------------------------------------------------------------------------------
int C360ChromePlugIn::ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
{
	if (ppData == NULL || nDataNum == 0)
	{
		return ERROR_INVALID_PARAM;
	}

	m_mapPidInfo.clear();
	m_mapDepthInfo.clear();
	m_mapPidNodeInfo.clear();
	m_mapIdIndexInfo.clear();

	PFAVORITELINEDATA* ppInnerData = new PFAVORITELINEDATA[nDataNum];
	memcpy(ppInnerData, ppData, sizeof(PFAVORITELINEDATA)*nDataNum);

	wchar_t* pszPath = GetFavoriteDataPath();

	//��ȡ�����ļ�·��
	std::wstring strTmpPath = StringHelper::ANSIToUnicode(StringHelper::UnicodeToUtf8(pszPath));
	if( FileHelper::IsFileExist(strTmpPath.c_str()) == TRUE)
	{
		BOOL bResult = ::DeleteFileW(pszPath);
		if( bResult == FALSE)
		{
			//����ǰ��ɾ��֮ǰ���ղؼ��ļ�
			bResult = FileHelper::DeleteFile(pszPath);
			free(pszPath);
			if (!bResult)
			{
				delete ppInnerData;
				return ERROR_CLEAR_FAVORITE_DATA;
			}
		}
	}

	Json::Value root;
	Json::Value bookmark_bar;
	Json::Value other;
	Json::Value synced;
	uint32 nIndex = 0;
	int32 nDepth = 0;

	InitializeChecksum();

	MakeSpecialFolderNode(L"Bookmarks bar", m_nIndex, bookmark_bar);
	SortByDepth(&ppInnerData[0], nDataNum);
	for (int32 i = 0; i < nDataNum; ++i)
	{
		if (ppInnerData[i] == NULL || ppInnerData[i]->bDelete == true)
		{
			continue;
		}

		MAP_PID_INFO::iterator it;
		it = m_mapPidInfo.find(ppInnerData[i]->nPid);
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

		if (ppInnerData[i]->bFolder)
		{
			m_mapPidInfo.insert(MAP_PID_INFO::value_type(ppInnerData[i]->nId, nDepth));
		}
		
		m_mapDepthInfo.insert(MAP_DEPTH_INFO::value_type(nDepth, i));
	}

	TraverseNode(ppInnerData, m_nMaxDepth);
	if (m_mapPidNodeInfo.size() == 1)
	{
		bookmark_bar["children"] = m_mapPidNodeInfo.begin()->second;
	}
	else
	{
		delete ppInnerData;
		return ERROR_INTERNAL_ERROR;
	}

	MAP_ID_INDEX_INFO::iterator itIdIndex;

	int nRealNum  = 0;
	for (int k = 0; k < nDataNum; k++)
	{
		if( ppInnerData[k] == NULL || ppInnerData[k]->bDelete == true)
			continue;

		nRealNum ++;

		itIdIndex = m_mapIdIndexInfo.find(ppInnerData[k]->nId);
		if (itIdIndex != m_mapIdIndexInfo.end())
		{
			int32 nIndex = (*itIdIndex).second; 
			if (ppInnerData[k]->bFolder)
			{
				m_mapPidInfo.insert(MAP_PID_INFO::value_type(ppInnerData[k]->nId, nDepth));
				char szId[256] = {0};
				sprintf_s(szId, 255, "%u", nIndex);
				UpdateChecksumWithFolderNode(&szId[0], ppInnerData[k]->szTitle);
			}
			else
			{
				char szId[256] = {0};
				sprintf_s(szId, 255, "%u", nIndex);
				UpdateChecksumWithUrlNode(&szId[0], ppInnerData[k]->szTitle, StringHelper::UnicodeToUtf8(ppInnerData[k]->szUrl));
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

	delete[] ppInnerData;

	nDataNum = nRealNum;

	return ERROR_OK;
}

void C360ChromePlugIn::SortByDepth(PFAVORITELINEDATA* ppData, int32 nDataNum)
{
	PFAVORITELINEDATA* ppSortLineData = new PFAVORITELINEDATA[nDataNum];
	memset(ppSortLineData, 0, sizeof(PFAVORITELINEDATA) * nDataNum);

	PFAVORITELINEDATA* ppSortLineDataPos = ppSortLineData;

	// ��һ�ҵ����ʵ����ݣ������뵽ppSortLineData��ȥ
	int k = 0;
	SortNode(ppData, nDataNum, ppSortLineDataPos, 0, k);

	// ���������ݿ���
	memcpy(ppData, ppSortLineData, nDataNum * sizeof(PFAVORITELINEDATA));
	delete[] ppSortLineData;
	ppSortLineData = NULL;
}

void C360ChromePlugIn::SortNode(PFAVORITELINEDATA* ppData, int32 nDataNum, 
				PFAVORITELINEDATA*& ppSortData, int32 nParentId, int& k)
{
	//static int k = 0;

	for (int i = 0; i < nDataNum; i++)
	{
		if (ppData[i]->nPid == nParentId)
		{
			memcpy(ppSortData++, &ppData[i], sizeof(PFAVORITELINEDATA));
			if (++k >= nDataNum)
			{
				//�������������ݣ���ֱ���˳�ѭ���������Ч��
				break;
			}

			if (ppData[i]->bFolder)
			{
				SortNode(ppData, nDataNum, ppSortData, ppData[i]->nId, k);
			}

		}
	}
}

BOOL C360ChromePlugIn::ExportFolder(Json::Value& folder_obj, int32 nPid,
								 PFAVORITELINEDATA* ppData, int nDataNum,int32& nRealDataNum)
{
	if (folder_obj.empty() || (folder_obj["type"].asString() != std::string("folder")))
	{
		return FALSE;
	}

	string strName = folder_obj["name"].asString();
	wstring wstrName = StringHelper::Utf8ToUnicode(strName) ;

	if (wstrName == L"Bookmarks bar" 
		|| wstrName == L"��ǩ��"
		|| wstrName == L"�ղ���"
		|| wstrName == L"Other bookmarks"
		|| wstrName == L"������ǩ"
		|| wstrName == L"������ǩ"
		|| wstrName == L"�����ղ�"
		|| wstrName == L"�����ղ�"
		|| wstrName == L"Synced bookmarks"
		|| wstrName == L"��ͬ����ǩ"
		|| wstrName == L"��ͬ���ղ�"
		)
	{
		Json::Value children_nodes = folder_obj["children"];
		int32 nNodeCount = children_nodes.size();
		for (int32 i = 0; i < nNodeCount; ++i)
		{
			Json::Value new_val = children_nodes[i];
			if (new_val["type"].asString() == std::string("url"))
			{
				ExportUrl(new_val, nPid, ppData, nDataNum, nRealDataNum);
			}
			else if(new_val["type"].asString() == std::string("folder"))
			{
				ExportFolder(new_val, nPid, ppData, nDataNum, nRealDataNum);
			}
		}
	}
	else
	{
		// ��ǰ������������Χ
		if( nRealDataNum == nDataNum)
			return TRUE;

		ppData[nRealDataNum]->nId = nRealDataNum + ID_VALUE_360EXTREME_BEGIN;
		ppData[nRealDataNum]->bFolder = true;
		ppData[nRealDataNum]->bDelete = false;

		StringToInt64(folder_obj["date_added"].asString(), ppData[nRealDataNum]->nAddTimes);
		StringToInt64(folder_obj["date_added"].asString(), ppData[nRealDataNum]->nLastModifyTime);

		ppData[nRealDataNum]->nPid = nPid;

		wcscpy_s(ppData[nRealDataNum]->szTitle, MAX_LENGTH -1, StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str());
		ppData[nRealDataNum]->szUrl[0] = 0;

		CCRCHash ojbCrcHash;
		ojbCrcHash.GetHash((BYTE *)ppData[nRealDataNum]->szTitle, wcslen(ppData[nRealDataNum]->szTitle) * sizeof(wchar_t),  \
			(BYTE *)&ppData[nRealDataNum]->nHashId, sizeof(int32));

		nRealDataNum++;

		Json::Value children_nodes = folder_obj["children"];
		int32 nNodeCount = children_nodes.size();
		int32 nCurrPid = nRealDataNum;
		for (int32 i = 0; i < nNodeCount; ++i)
		{
			Json::Value new_val = children_nodes[i];
			if (new_val["type"].asString() == std::string("url"))
			{
				ExportUrl(new_val, nCurrPid -1 + ID_VALUE_360EXTREME_BEGIN, ppData, nDataNum,nRealDataNum);
			}
			else if(new_val["type"].asString() == std::string("folder"))
			{
				ExportFolder(new_val, nCurrPid -1 + ID_VALUE_360EXTREME_BEGIN, ppData, nDataNum, nRealDataNum);
			}
		}
	}

	return TRUE;
}

BOOL C360ChromePlugIn::ExportUrl(Json::Value& url_obj, int32 nPid, PFAVORITELINEDATA* ppData, int32 nDataNum, int32& nRealDataNum)
{
	if (url_obj.empty() || url_obj["type"].asString() != std::string("url"))
	{
		return FALSE;
	}

	if( nDataNum == nRealDataNum)
		return FALSE;

	ppData[nRealDataNum]->nId = nRealDataNum + ID_VALUE_360EXTREME_BEGIN;
	ppData[nRealDataNum]->bFolder = false;
	ppData[nRealDataNum]->bDelete = false;
	StringToInt64(url_obj["date_added"].asString(), ppData[nRealDataNum]->nAddTimes);
	ppData[nRealDataNum]->nLastModifyTime =  0;
	ppData[nRealDataNum]->nPid = nPid;

	wcscpy_s(ppData[nRealDataNum]->szTitle, MAX_LENGTH -1, StringHelper::Utf8ToUnicode(url_obj["name"].asString()).c_str());
	wcscpy_s(ppData[nRealDataNum]->szUrl, MAX_LENGTH - 1, StringHelper::Utf8ToUnicode(url_obj["url"].asString()).c_str());
	ppData[nRealDataNum]->szUrl[MAX_LENGTH-1] = 0;

	CCRCHash ojbCrcHash;
	ojbCrcHash.GetHash((BYTE *)ppData[nRealDataNum]->szTitle, wcslen(ppData[nRealDataNum]->szTitle) * sizeof(wchar_t),  \
		(BYTE *)&ppData[nRealDataNum]->nHashId, sizeof(int32));

	nRealDataNum++;

    return TRUE;
}


BOOL C360ChromePlugIn::MakeFolderNode(PFAVORITELINEDATA pstData, Json::Value& folder_obj, uint32& nIndex)
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

BOOL C360ChromePlugIn::MakeUrlNode(PFAVORITELINEDATA pstData, Json::Value& url_obj, uint32& nIndex)
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


BOOL C360ChromePlugIn::MakeSpecialFolderNode(wchar_t *pszName, uint32& nIndex, Json::Value& folder_obj)
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

BOOL C360ChromePlugIn::EnumNode(Json::Value& folder_obj, int32& nCount)
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


BOOL C360ChromePlugIn::TraverseNode(PFAVORITELINEDATA* ppData, int32 nDepth)
{
	if (nDepth >= 0)
	{
		std::vector<int32> vecPidList;
		std::multimap<int32, NODEINFO> mapPidObj;
		for (MAP_DEPTH_INFO ::iterator it = m_mapDepthInfo.lower_bound(nDepth);
			it != m_mapDepthInfo.upper_bound(nDepth); ++it)
		{
			NODEINFO stNodeInfo = {0};	

			if( ppData[(*it).second] == NULL )
				continue;

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

BOOL C360ChromePlugIn::StringToInt64(std::string strTime, int64& nTime)
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

BOOL C360ChromePlugIn::Int64ToString(int64 nTime, std::string& strTime)
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

void C360ChromePlugIn::UpdateChecksum(const std::string& str) 
{
	MD5Update(&m_szMd5Context, str.data(), str.length() * sizeof(char));
}

void C360ChromePlugIn::UpdateChecksum(const std::wstring& str) 
{
	MD5Update(&m_szMd5Context, str.data(), str.length() * sizeof(uint16));
}

void C360ChromePlugIn::UpdateChecksumWithUrlNode(const std::string& id, const std::wstring& title, const std::string& url) 
{
	if (IsStringUTF8(url))
	{
		UpdateChecksum(id);
		UpdateChecksum(title);
		UpdateChecksum("url");
		UpdateChecksum(url);
	}
}

void C360ChromePlugIn::UpdateChecksumWithFolderNode(const std::string& id, const std::wstring& title) 
{
	UpdateChecksum(id);
	UpdateChecksum(title);
	UpdateChecksum("folder");
}

void C360ChromePlugIn::InitializeChecksum() 
{
	MD5Init(&m_szMd5Context);
}

void C360ChromePlugIn::FinalizeChecksum() 
{
	MD5Digest digest;
	MD5Final(&digest, &m_szMd5Context);
	m_strCheckSum = MD5DigestToBase16(digest);
}

inline bool C360ChromePlugIn::IsValidCharacter(uint32 code_point) 
{
	// Excludes non-characters (U+FDD0..U+FDEF, and all codepoints ending in
	// 0xFFFE or 0xFFFF) from the set of valid code points.
	return code_point < 0xD800u || (code_point >= 0xE000u &&
		code_point < 0xFDD0u) || (code_point > 0xFDEFu &&
		code_point <= 0x10FFFFu && (code_point & 0xFFFEu) != 0xFFFEu);
}

bool C360ChromePlugIn::IsStringUTF8(const std::string& str) 
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