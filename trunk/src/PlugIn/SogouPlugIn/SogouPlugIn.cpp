#include "stdafx.h"
#include "SogouPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include "Decoder.h"
#include "CppSQLite3.h"
#include <algorithm>
#include <boost/crc.hpp>

using namespace sogouplugin;

#pragma comment(lib, "shlwapi.lib")

namespace sogouplugin
{
	CSogouPlugIn*	g_SogouPlugIn	=	NULL;
}

// 导出借口实现
IPlugIn*	GetPlugIn()
{
	if( g_SogouPlugIn == NULL)
	{
		g_SogouPlugIn = new CSogouPlugIn();
		ASSERT( g_SogouPlugIn != NULL);
	}

	return g_SogouPlugIn;
}

void	ReleasePlugIn( IPlugIn* p)
{
	ASSERT( g_SogouPlugIn == p);
	if( g_SogouPlugIn  != NULL)
	{
		delete g_SogouPlugIn;
		g_SogouPlugIn = NULL;
	}
}


void Mereg(PFAVORITELINEDATA pData, int32 nLen, int nParentId)
{
	int nHash = 0;
	vector<PFAVORITELINEDATA> vec;

	//把所有相同父结点的节点放到vec中
	for (int i = 0; i < nLen; i++)
	{
		if (pData[i].nPid == nParentId)
		{
			vec.push_back(&pData[i]);
		}
	}

	int vSize = vec.size();

	//如果扫描出来的结点数少于1，merge结束, 即至少需要两个节点指到同一个父结点
	if (vSize > 1)
	{
		for (int i = 0; i < vSize; i++)
		{
			for(int j = i + 1; j < vSize; j++)
			{
				if (vec[i]->nHashId == vec[j]->nHashId)
				{
					//置上懒删除标记
					vec[j]->bDelete = true;
					//重新修正所有父节点为j节点
					for (int m = 0; m < nLen; m++)
					{
						if (pData[m].nPid == vec[j]->nId)
						{
							pData[m].nPid = vec[i]->nId;
						}
					}

					Mereg(pData, nLen, vec[i]->nId);
				}
			}
		}
	}
}

void Rearrange(PFAVORITELINEDATA pData, int nLen)
{
	for (int i = 0; i < nLen; i++)
	{
		//如果该结点的nId不是数组下标+1,则需要修正
		if ((pData[i].nId != i + 1))
		{
			//扫描所有以该结点为父结点，并修正他们的父结点id
			for (int j = 0; j < nLen + 7; j++)
			{
				if (pData[j].nPid == pData[i].nId)
				{
					pData[j].nPid = i + 1;
				}
			}

			pData[i].nId = i + 1;
		}
	}
}

CSogouPlugIn::CSogouPlugIn()
{
	m_pMemFavoriteDB = NULL;

	Load();
	GetFavoriteCount();

	vector<FAVORITELINEDATA> vec(245);

	int32 len = 245;

	FILE *pTemp = fopen("aa.bin", "rb");
	//FILE *pTemp = fopen("aa.bin", "wb");

	//ExportFavoriteData(&vec[0], len);

	fread(&vec[0], sizeof(FAVORITELINEDATA), 7, pTemp);
	//fwrite(&vec[0], sizeof(FAVORITELINEDATA), 7, pTemp);

	fclose(pTemp);
	//ImportFavoriteData(stFavorite, 100);
	ExportFavoriteData(&vec[7], len);

	Rearrange(&vec[0], len + 7);

	Mereg(&vec[0], len + 7, 0);

	ImportFavoriteData(&vec[0], len + 7);
	UnLoad();

}


CSogouPlugIn::~CSogouPlugIn()
{
	
}

BOOL CSogouPlugIn::Load()
{
	std::string strDecodeContent;

	if (decode(StringHelper::UnicodeToANSI(GetFavoriteDataPath()), strDecodeContent) == 0)
	{
		m_pMemFavoriteDB = (winFileMem *)malloc(sizeof(winFileMem));

		m_pMemFavoriteDB->ulMemSize = strDecodeContent.length();
		m_pMemFavoriteDB->pMemPointer = (unsigned char *)malloc(strDecodeContent.length());
		memcpy(m_pMemFavoriteDB->pMemPointer, strDecodeContent.c_str(), m_pMemFavoriteDB->ulMemSize);

		return TRUE;
	}

	return FALSE;
}

BOOL CSogouPlugIn::UnLoad()
{
	if (m_pMemFavoriteDB)
	{
		FILE *fOut = fopen("b.db", "wb");
		fwrite(m_pMemFavoriteDB->pMemPointer, 1, m_pMemFavoriteDB->ulMemSize, fOut);
		fclose(fOut);
		free(m_pMemFavoriteDB->pMemPointer);
		free(m_pMemFavoriteDB);
		m_pMemFavoriteDB = NULL;
		return TRUE;
	}

	return FALSE;
}

BOOL CSogouPlugIn::IsWorked(wchar_t *pszBrowserVersion, int32 &nLen)
{
	wchar_t szVersion[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szVersion); 
	int32   nVersion = 0;

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\SogouExplorer",
		L"DisplayVersion", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szVersion, 
		&dwSize))
	{
		if (nLen >= dwSize)
		{
			_tcscpy_s(pszBrowserVersion, nLen - 1, szVersion);
			pszBrowserVersion[nLen - 1] = 0;
		}

		return TRUE;
	}
}

int32 CSogouPlugIn::GetPluginVersion()
{
	return 1;
}

const wchar_t* CSogouPlugIn::GetBrowserName()
{
	return L"SogouExplorer";
}

wchar_t* CSogouPlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\SogouExplorer",
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

wchar_t* CSogouPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\SogouExplorer\\Favorite2.dat";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _wcsdup(strPath.c_str());
}

wchar_t* CSogouPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\SogouExplorer\\HistoryUrl.db";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _wcsdup(strPath.c_str());
}

BOOL CSogouPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	boost::crc_32_type objCrc32;

	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from FavorTable");
		int i = 0;

		while(!Query.eof() && i < nDataNum)
		{
			 pData[i].nId = Query.getIntField("id", 0) + PLUGIN_SOGOU_ID_BASE;
		     pData[i].nPid = Query.getIntField("pid", 0);

			 if (pData[i].nPid != 0)
			 {
				 pData[i].nPid += PLUGIN_SOGOU_ID_BASE;
			 }

			 pData[i].bFolder = Query.getIntField("folder", 0);
			 _tcscpy_s(pData[i].szTitle, MAX_PATH - 1, StringHelper::Utf8ToUnicode(Query.getStringField("title", 0)).c_str());
			 pData[i].szTitle[MAX_PATH - 1] = 0;
			 _tcscpy_s(pData[i].szUrl, 1023, StringHelper::Utf8ToUnicode(Query.getStringField("url", 0)).c_str());
			 pData[i].szUrl[1023] = 0;
			 pData[i].nOrder = Query.getIntField("sequenceNO", 0);

			 TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(Query.getStringField("addTime", "2011-05-11 21:00:00")).c_str()), pData[i].nAddTimes);
			 TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(Query.getStringField("lastmodify", "2011-05-11 21:00:00")).c_str()), pData[i].nLastModifyTime);
			 objCrc32.process_bytes(pData[i].szTitle, wcslen(pData[i].szTitle) * sizeof(wchar_t));
			 pData[i].nHashId = objCrc32.checksum();
			 pData[i].nCatId = Query.getIntField("category", 0);

			 pData[i].bDelete = false;

			 Query.nextRow();
			 i++;
		}

		nDataNum = i;

		return TRUE;
	}

	return FALSE;
}

BOOL CSogouPlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	if (m_pMemFavoriteDB != NULL)
	{
#define MAX_BUFFER_LEN	4096

		CppSQLite3DB  m_SqliteDatabase;
		wchar_t szInsert[MAX_BUFFER_LEN] = {0};
		wchar_t szDelete[MAX_BUFFER_LEN] = {0};

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		int i = 0;

		m_SqliteDatabase.execDML(StringHelper::UnicodeToUtf8(L"delete from favorTable").c_str());

		for (int i = 0; i < nDataNum; i++)
		{
			if (pData[i].bDelete == true)
			{
				continue;
			}

			ReplaceSingleQuoteToDoubleQuote(pData[i].szTitle);
			ReplaceSingleQuoteToDoubleQuote(pData[i].szUrl);

			swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into favorTable"
				L"(id,pid,folder,title,url,sequenceNo,addtime,lastmodify,hashid,category,reserved)"
				L"values(%d,%d,%d,'%s','%s',%d,'%s',"
				L"'%s',%u,%d,0)", 
				pData[i].nId,
				pData[i].nPid,
				pData[i].bFolder,
				pData[i].szTitle,
				pData[i].szUrl,
				pData[i].nOrder,
				L"2011-05-11 12:00:00", 
				L"2011-05-11 12:00:00",
				pData[i].nHashId,
				pData[i].nCatId);
			m_SqliteDatabase.execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());
		}

		SaveDatabase();

		return TRUE;
	}

	return FALSE;
}

int32 CSogouPlugIn::GetFavoriteCount()
{
	int32 nCount = 0;
	
	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select count(*) as Total from FavorTable");

		nCount = Query.getIntField("Total");
	}

	return nCount;
}

BOOL CSogouPlugIn::SaveDatabase()
{
	std::string strEncode;
	wchar_t *pszFavoriteDataPath = GetFavoriteDataPath();
	int nRet = 0;

	if (m_pMemFavoriteDB == NULL)
	{
		return FALSE;
	}

	strEncode.assign((char *)m_pMemFavoriteDB->pMemPointer, m_pMemFavoriteDB->ulMemSize);

	nRet = encode(strEncode, StringHelper::UnicodeToANSI(pszFavoriteDataPath));

	free(pszFavoriteDataPath);

	return nRet == 0;
}

void CSogouPlugIn::ReplaceSingleQuoteToDoubleQuote(wchar_t *pszOri)
{
	int32 nLen = _tcslen(pszOri);

	if (pszOri == NULL || nLen == 0)
	{
		return;
	}

	for (int i = 0; i < nLen; i++)
	{
		if (pszOri[i] == '\'')
		{
			pszOri[i] = '\"';
		}
	}

}



