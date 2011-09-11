#include "stdafx.h"
#include "PlugIn.h"
#include "360SE3PlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include "CppSQLite3.h"
#include <algorithm>
#include "CRCHash.h"
#include "XString.h"
#include "FileHelper.h"

#pragma comment(lib, "shlwapi.lib")


C360SE3PlugIn::C360SE3PlugIn()
{
	m_strFavoritePath = L"";
}


C360SE3PlugIn::~C360SE3PlugIn()
{
	
}

BOOL C360SE3PlugIn::Load()
{
	return FALSE;
}

BOOL C360SE3PlugIn::UnLoad()
{
	if( m_SqliteDatabase.IsOpen() == TRUE)
		m_SqliteDatabase.close();

	return TRUE;
}

int32 C360SE3PlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* C360SE3PlugIn::GetBrowserName()
{
	return L"360SE";
}

wchar_t* C360SE3PlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{23F3F476-BE34-4f48-9C77-2806A8393EC4}",
		L"UninstallString", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		if( PathFileExists(szPath) == FALSE)
			return NULL;

		return _wcsdup(szPath);
	}

	return NULL;
}

wchar_t* C360SE3PlugIn::GetFavoriteDataPath()
{
	if( m_strFavoritePath != L"")
		return (wchar_t*)m_strFavoritePath.c_str();

	const wchar_t* pszPath = PathHelper::GetAppDataDir();
	if( pszPath == NULL)
		return NULL;

	String strPath = pszPath;
	strPath += L"\\360se\\data\\360sefav.db";
	free((void*)pszPath);

	if( FileHelper::IsFileExist(strPath.GetData()) == TRUE)
	{
		//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
		m_strFavoritePath = strPath.GetData();
		return _wcsdup(strPath.GetData());
	}

	return NULL;
}

wchar_t* C360SE3PlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + std::wstring(L"\\data\\history.dat");

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

BOOL C360SE3PlugIn::ExportFavoriteData( PFAVORITELINEDATA* ppData, int32& nDataNum )
{
	CCRCHash ojbCrcHash;

	if (ppData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	const wchar_t* pszPath = GetFavoriteDataPath();
	if(pszPath == NULL)
		return FALSE;

	if( m_SqliteDatabase.IsOpen() == FALSE)
		m_SqliteDatabase.open(pszPath, "");
	ASSERT(m_SqliteDatabase.IsOpen() == TRUE);

	m_SqliteDatabase.execDML("delete from tb_fav");

	CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from tb_fav");
	int i = 0;

	while(!Query.eof() && i < nDataNum)
	{
		 ppData[i]->nId = Query.getIntField("id", 0) + ID_VALUE_360_BEGIN;
		 ppData[i]->nPid = Query.getIntField("parent_id", 0);

		 if (ppData[i]->nPid != 0)
		 {
			 ppData[i]->nPid += ID_VALUE_360_BEGIN;
		 }

		 ppData[i]->bFolder = Query.getIntField("is_folder", 0);
		 wcscpy_s(ppData[i]->szTitle, MAX_LENGTH - 1, StringHelper::Utf8ToUnicode(Query.getStringField("title", 0)).c_str());
		 ppData[i]->szTitle[MAX_LENGTH - 1] = 0;
		 wcscpy_s(ppData[i]->szUrl, MAX_LENGTH-1, StringHelper::Utf8ToUnicode(Query.getStringField("url", 0)).c_str());
		 ppData[i]->szUrl[MAX_LENGTH-1] = 0;
		 ppData[i]->nOrder = Query.getIntField("pos", 0);
		 ppData[i]->nAddTimes = Query.getInt64Field("create_time", 0);
		 ppData[i]->nLastModifyTime = Query.getInt64Field("last_modify_time",0);

		 ojbCrcHash.GetHash((BYTE *)ppData[i]->szTitle, wcslen(ppData[i]->szTitle) * sizeof(wchar_t), (BYTE *)&ppData[i]->nHashId, sizeof(uint32));
		 ppData[i]->bDelete = false;

		 Query.nextRow();
		 i++;
	}

	nDataNum = i;

	return TRUE;
}

BOOL C360SE3PlugIn::ImportFavoriteData( PFAVORITELINEDATA* ppData, int32& nDataNum )
{
	if (ppData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	const wchar_t* pszPath = GetFavoriteDataPath();
	if( pszPath == NULL)
		return FALSE;

	if( m_SqliteDatabase.IsOpen() == FALSE)
		m_SqliteDatabase.open(pszPath, "");
	ASSERT(m_SqliteDatabase.IsOpen() == TRUE);

	m_SqliteDatabase.execDML("delete from tb_fav");

#define MAX_BUFFER_LEN	4096
	wchar_t szInsert[MAX_BUFFER_LEN] = {0};
	wchar_t szDelete[MAX_BUFFER_LEN] = {0};

	for (int i = 0; i < nDataNum; i++)
	{
		if (ppData[i] == NULL || ppData[i]->bDelete == true)
		{
			continue;
		}

		ReplaceSingleQuoteToDoubleQuote(ppData[i]->szTitle);
		ReplaceSingleQuoteToDoubleQuote(ppData[i]->szUrl);

		swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into tb_fav"
			L"(id,parent_id,is_folder,title,url,pos,create_time,last_modify_time,is_best,reserved)"
			L" values(%d,%d,%d,'%s','%s',%d,'%s',"
			L"'%s',%d,0)", 
			ppData[i]->nId,
			ppData[i]->nPid,
			ppData[i]->bFolder,
			ppData[i]->szTitle,
			ppData[i]->szUrl,
			ppData[i]->nOrder,
			L"2011-05-11 12:00:00", 
			L"2011-05-11 12:00:00",
			0);
		m_SqliteDatabase.execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());
	}

	SaveDatabase();

	return TRUE;
}

int32 C360SE3PlugIn::GetFavoriteCount()
{
	const wchar_t* pszPath = GetFavoriteDataPath();
	if( pszPath == NULL)
		return 0;

	if( m_SqliteDatabase.IsOpen() == FALSE)
		m_SqliteDatabase.open(pszPath, "");
	ASSERT(m_SqliteDatabase.IsOpen() == TRUE);
	
	CppSQLite3Query Query = m_SqliteDatabase.execQuery("select count(*) as Total from tb_fav");
	int nTotal =   Query.getIntField("Total");
	return nTotal;
}

BOOL C360SE3PlugIn::SaveDatabase()
{
	return TRUE;
}

void C360SE3PlugIn::ReplaceSingleQuoteToDoubleQuote(wchar_t *pszOri)
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



