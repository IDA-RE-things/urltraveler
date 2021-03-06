#include "stdafx.h"
#include "SogouPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include "Decoder.h"
#include "CppSQLite3.h"
#include "CRCHash.h"
#include "SogouPlugInFactory.h"

using namespace sogouplugin;

#pragma comment(lib, "shlwapi.lib")


SogouPlugIn::SogouPlugIn()
{
	m_pMemFavoriteDB = NULL;
}


SogouPlugIn::~SogouPlugIn()
{
	
}

BOOL SogouPlugIn::Load()
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

BOOL SogouPlugIn::UnLoad()
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

BOOL SogouPlugIn::IsWorked()
{
	BOOL bRet = FALSE;

	if (m_pMemFavoriteDB)
	{
		CppSQLite3DB  objSqliteDatabase;
		objSqliteDatabase.openmem(m_pMemFavoriteDB, "");
		bRet = objSqliteDatabase.tableExists("favorTable");
	}

	if( bRet == FALSE)
	{
		ASSERT(0);
	}

	return bRet;
}

int32 SogouPlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* SogouPlugIn::GetBrowserName()
{
	return L"搜狗浏览器";
}

wchar_t* SogouPlugIn::GetInstallPath()
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

wchar_t* SogouPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + wstring(L"\\SogouExplorer\\Favorite2.dat");

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

wchar_t* SogouPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + std::wstring(L"\\SogouExplorer\\HistoryUrl.db");

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

BOOL SogouPlugIn::ExportFavoriteData( PFAVORITELINEDATA* ppData, int32& nDataNum )
{
	CCRCHash ojbCrcHash;

	if (ppData == NULL || *ppData == NULL || nDataNum == 0)
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
			 ppData[i]->nId = Query.getIntField("id", 0) + ID_VALUE_SOGOU_BEGIN;
			 ppData[i]->nPid = Query.getIntField("pid", 0);

			 if (ppData[i]->nPid != 0)
			 {
				 ppData[i]->nPid += ID_VALUE_SOGOU_BEGIN;
			 }

			  ppData[i]->bFolder = Query.getIntField("folder", 0);
			 _tcsncpy_s( ppData[i]->szTitle, MAX_LENGTH - 1, StringHelper::Utf8ToUnicode(Query.getStringField("title", 0)).c_str(), MAX_LENGTH - 2);
			  ppData[i]->szTitle[MAX_LENGTH - 1] = 0;
			 _tcsncpy_s( ppData[i]->szUrl, MAX_LENGTH-1, StringHelper::Utf8ToUnicode(Query.getStringField("url", 0)).c_str(), MAX_LENGTH-2);
			  ppData[i]->szUrl[MAX_LENGTH-1] = 0;
			  ppData[i]->nOrder = Query.getIntField("sequenceNO", 0);

			 TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(
				 StringHelper::Utf8ToUnicode(
				 Query.getStringField("addTime", "2011-05-11 21:00:00")).c_str()),  ppData[i]->nAddTimes);

			 TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(
				 StringHelper::Utf8ToUnicode(
				 Query.getStringField("lastmodify", "2011-05-11 21:00:00")).c_str()),  ppData[i]->nLastModifyTime);

			 ojbCrcHash.GetHash((BYTE *) ppData[i]->szTitle, wcslen( ppData[i]->szTitle) * sizeof(wchar_t), (BYTE *)& ppData[i]->nHashId, sizeof(uint32));
			  //ppData[i]->nCatId = Query.getIntField("category", 0);

			  ppData[i]->bDelete = false;

			 Query.nextRow();
			 i++;
		}

		nDataNum = i;

		return TRUE;
	}

	return FALSE;
}

int SogouPlugIn::ImportFavoriteData( PFAVORITELINEDATA* ppData, int32& nDataNum )
{
	if (ppData == NULL || nDataNum == 0)
	{
		return ERROR_INVALID_PARAM;
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
			if (ppData[i]->bDelete == true)
			{
				continue;
			}

			ReplaceSingleQuoteToDoubleQuote(ppData[i]->szTitle);
			ReplaceSingleQuoteToDoubleQuote(ppData[i]->szUrl);

			swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into favorTable"
				L"(id,pid,folder,title,url,sequenceNo,addtime,lastmodify,hashid,category,reserved)"
				L"values(%d,%d,%d,'%s','%s',%d,'%s',"
				L"'%s',%u,%d,0)", 
				ppData[i]->nId,
				ppData[i]->nPid,
				ppData[i]->bFolder,
				ppData[i]->szTitle,
				ppData[i]->szUrl,
				ppData[i]->nOrder,
				L"2011-05-11 12:00:00", 
				L"2011-05-11 12:00:00",
				ppData[i]->nHashId,
				0);

			try
			{
				m_SqliteDatabase.execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());
			}
			catch(CppSQLite3Exception&e )
			{
				int i=0;
				i++;
			}
		}

		SaveDatabase();

		return ERROR_OK;
	}

	return ERROR_SQLITE_ERROR;
}

int32 SogouPlugIn::GetFavoriteCount()
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

BOOL SogouPlugIn::SaveDatabase()
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