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
	return TRUE;
}

BOOL C360SE3PlugIn::UnLoad()
{
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

	wchar_t* pszPath = GetInstallPath();
	if( pszPath == NULL)
		return NULL;

	String strPath = pszPath;
	int nIndex = strPath.ReverseFind(L"\\");
	if( nIndex == String::NPOS)
		return NULL;

	strPath = strPath.SubStr(0, nIndex);
	nIndex = strPath.ReverseFind(L"\\");
	if( nIndex == String::NPOS)
		return NULL;

	strPath = strPath.SubStr(0, nIndex);
	strPath += L"\\data\\360sefav.db";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.GetData());
}

wchar_t* C360SE3PlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\data\\history.dat";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

BOOL C360SE3PlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	memset(pData,0x0, nDataNum*sizeof(FAVORITELINEDATA));

	CCRCHash ojbCrcHash;

	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	CppSQLite3DB m_SqliteDatabase;

	m_SqliteDatabase.open(GetFavoriteDataPath(), "");
	CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from tb_fav");
	int i = 0;

	while(!Query.eof() && i < nDataNum)
	{
		 pData[i].nId = Query.getIntField("id", 0) + ID_VALUE_360_BEGIN;
	     pData[i].nPid = Query.getIntField("parent_id", 0);

		 if (pData[i].nPid != 0)
		 {
			 pData[i].nPid += ID_VALUE_360_BEGIN;
		 }

		 pData[i].bFolder = Query.getIntField("is_folder", 0);
		 wcscpy_s(pData[i].szTitle, MAX_PATH - 1, StringHelper::Utf8ToUnicode(Query.getStringField("title", 0)).c_str());
		 pData[i].szTitle[MAX_PATH - 1] = 0;
		 wcscpy_s(pData[i].szUrl, 1023, StringHelper::Utf8ToUnicode(Query.getStringField("url", 0)).c_str());
		 pData[i].szUrl[1023] = 0;
		 pData[i].nOrder = Query.getIntField("pos", 0);
		 pData[i].nAddTimes = Query.getInt64Field("create_time", 0);
		 pData[i].nLastModifyTime = Query.getInt64Field("last_modify_time",0);

		 ojbCrcHash.GetHash((BYTE *)pData[i].szTitle, wcslen(pData[i].szTitle) * sizeof(wchar_t), (BYTE *)&pData[i].nHashId, sizeof(uint32));
		 pData[i].nCatId = 0;
		 pData[i].bDelete = false;

		 Query.nextRow();
		 i++;
	}

	nDataNum = i;

	return TRUE;
}

BOOL C360SE3PlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}


#define MAX_BUFFER_LEN	4096

	CppSQLite3DB  m_SqliteDatabase;
	wchar_t szInsert[MAX_BUFFER_LEN] = {0};
	wchar_t szDelete[MAX_BUFFER_LEN] = {0};

	m_SqliteDatabase.open(GetFavoriteDataPath(), "");
	int i = 0;

	m_SqliteDatabase.execDML(StringHelper::UnicodeToUtf8(L"delete from tb_fav").c_str());

	for (int i = 0; i < nDataNum; i++)
	{
		if (pData[i].bDelete == true)
		{
			continue;
		}

		ReplaceSingleQuoteToDoubleQuote(pData[i].szTitle);
		ReplaceSingleQuoteToDoubleQuote(pData[i].szUrl);

		swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into tb_fav"
			L"(id,parent_id,is_folder,title,url,pos,create_time,last_modify_time,is_best,reserved)"
			L" values(%d,%d,%d,'%s','%s',%d,'%s',"
			L"'%s',%d,0)", 
			pData[i].nId,
			pData[i].nPid,
			pData[i].bFolder,
			pData[i].szTitle,
			pData[i].szUrl,
			pData[i].nOrder,
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
	CppSQLite3DB  m_SqliteDatabase;
	m_SqliteDatabase.open(GetFavoriteDataPath(), "");
	
	CppSQLite3Query Query = m_SqliteDatabase.execQuery("select count(*) as Total from tb_fav");
	return  Query.getIntField("Total");
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



