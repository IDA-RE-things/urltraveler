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
			return wcsdup(szPath);
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
	return wcsdup(strPath.c_str());
}

//----------------------------------------------------------------------------------------
//����: GetHistoryDataPath
//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
//----------------------------------------------------------------------------------------
wchar_t* CChromePlugIn::GetHistoryDataPath() 
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Local\\Google\\Chrome\\User Data\\Default\\History";

	return wcsdup(strPath.c_str());
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
	uint32 nIndex = 0;
	int32 nDepth = 0;

	MakeSpecialFolderNode(L"��ǩ��", nIndex, bookmark_bar);
	MakeSpecialFolderNode(L"������ǩ", nIndex, other);

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
	
	root["checksum"] = "9f25063887d5c6aa96b4d606b8dfdbcb";
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

	if (!wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"��ǩ��") 
		|| !wcscmp(StringHelper::Utf8ToUnicode(folder_obj["name"].asString()).c_str(), L"������ǩ"))
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

// 		TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(folder_obj["date_added"].asString()).c_str()),  \
// 			pData[nDataNum].nAddTimes);
// 		TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(folder_obj["date_modified"].asString()).c_str()),  \
// 			pData[nDataNum].nLastModifyTime);
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
// 	TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(url_obj["date_added"].asString()).c_str()),  \
// 		pData[nDataNum].nAddTimes);
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

BOOL CChromePlugIn::MakeUrlNode(FAVORITELINEDATA stData, Json::Value& url_obj, uint32& nIndex)
{
	wchar_t szFormat[] = L" {\
				 \"date_added\": \"%s\",\
				 \"id\": \"%u\",\
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

BOOL CChromePlugIn::MakeSpecialFolderNode(wchar_t *pszName, uint32& nIndex, Json::Value& folder_obj)
{
	wchar_t szFormat[] = L" { \"date_added\": \"0\",\
						  \"date_modified\": \"%s\",\
						  \"id\": \"%u\",\
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

std::string CChromePlugIn::MD5ToBase16(byte* pbyData)
{
	static char const szEncode[] = "0123456789abcdef";

	std::string strRet;
	strRet.resize(32);

	int j = 0;
	for (int i = 0; i < 16; i ++) 
	{
		int a = pbyData[i];
		strRet[j++] = szEncode[(a >> 4) & 0xf];
		strRet[j++] = szEncode[a & 0xf];
	}

	return strRet;
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