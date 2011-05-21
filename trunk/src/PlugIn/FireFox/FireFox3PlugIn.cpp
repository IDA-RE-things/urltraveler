#include "stdafx.h"
#include "PlugIn.h"
#include "FireFox3PlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include "CppSQLite3.h"
#include <algorithm>
#include "CRCHash.h"
#include "FireFoxPlugInFactory.h"
#include "Registry.h"
#include "XString.h"
#include "StringHelper.h"


using namespace firefox;

#pragma comment(lib, "shlwapi.lib")



FireFox3PlugIn::FireFox3PlugIn()
{
	m_pSqliteDatabase = NULL;
}

FireFox3PlugIn::~FireFox3PlugIn()
{
	if( m_pSqliteDatabase != NULL)
	{
		m_pSqliteDatabase->close();
		delete m_pSqliteDatabase;
	}
}

BOOL FireFox3PlugIn::Load()
{
	return TRUE;
}

BOOL FireFox3PlugIn::UnLoad()
{
	return TRUE;
}

BOOL FireFox3PlugIn::IsWorked()
{
	if( GetInstallPath() == NULL)
		return FALSE;

	return TRUE;
}

int32 FireFox3PlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* FireFox3PlugIn::GetBrowserName()
{
	return L"FireFox3.0";
}

wchar_t* FireFox3PlugIn::GetInstallPath()
{
	HKEY	hKey;
	if (ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\", 0, KEY_READ, &hKey))   
	{   
		return NULL;   
	}

	// ��ȡ����Ϣ(��Ҫ���Ӽ����ͼ�ֵ��, �����������)   
	// ע: Ҳ���Բ���������ȡ����, ����ö�ٺ���(RegEnumXXX)���� ERROR_NO_MORE_ITEMS ���ж�ö�ٽ���.   
	DWORD dwSubKeyCount = 0;
	DWORD dwValueCount = 0;
	if (ERROR_SUCCESS != ::RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwSubKeyCount, 
		NULL, NULL, &dwValueCount, NULL, NULL, NULL, NULL))   
	{   
		if (NULL != hKey)   
		{   
			::RegCloseKey(hKey);   
			hKey = NULL;   
		}   
		return NULL;   
	}

	const long MAX_KEY = 255;                       // ����󳤶�   
	wchar_t ptszSubKey[MAX_KEY] = _T("");             // �Ӽ��ַ���   
	DWORD cbSubKey = MAX_KEY; 

	// �����Ӽ�   
	if (0 != dwSubKeyCount)   
	{   
		wstring	wstrSubKey;
		for (DWORD i = 0; i < dwSubKeyCount; i++)   
		{   
			cbSubKey = MAX_KEY;   
			if (ERROR_SUCCESS == ::RegEnumKeyExW(hKey, i, ptszSubKey, &cbSubKey, NULL, NULL, NULL, NULL))   
			{   
				wstrSubKey = ptszSubKey;
				size_t n = wstrSubKey.find(L"Mozilla Firefox (3");
				if( 0xffffffff != wstrSubKey.find(L"Mozilla Firefox (3"))
				{
					wchar_t szInstallPath[MAX_PATH] = {0};
					DWORD   dwSize = sizeof(szInstallPath); 
					int32   nVersion = 0;

					wchar_t szFullPath[MAX_PATH];
					swprintf(szFullPath, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s", ptszSubKey);

					if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
						szFullPath,
						L"DisplayIcon", 
						SRRF_RT_REG_SZ, 
						NULL,			
						szInstallPath, 
						&dwSize))
					{
						// firefox3.0��·����ʽΪ C:\Program Files\Mozilla Firefox\firefox.exe,0
						wstring wstrPath = szInstallPath;
						int nIndex = wstrPath.find_last_of(L",");
						if( nIndex != -1)
							wstrPath = wstrPath.substr(0, nIndex);

						return const_cast<wchar_t*>(wstrPath.c_str());
					}

				}
			}   
		}   
	}

	return NULL;
}

// ��ΪFirefox��Ŀ¼�Ƚ����⣬��λ��C:\Users\zhangzq\AppData\Roaming\Mozilla\Firefox\Profiles\*.default
// Ŀ¼�£�*Ϊһ�漴������˱����ȼ������Ŀ¼
wchar_t*	FireFox3PlugIn::GetAppBasePath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Mozilla\\Firefox\\Profiles\\";

	WIN32_FIND_DATA fd;
	ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

	HANDLE hFile = FindFirstFile(wstring(strPath + L"*").c_str(), &fd);
	if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		wstring	wstrFileName = fd.cFileName;
		if( wstrFileName.rfind(L".default") != wstring::npos)
		{
			strPath += wstrFileName;
			return _wcsdup(strPath.c_str());
		}
	}

	int nRet = FindNextFileW(hFile, &fd);
	while( nRet != FALSE)
	{
		if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			wstring	wstrFileName = fd.cFileName;
			if( wstrFileName.rfind(L".default") != wstring::npos)
			{
				strPath += wstrFileName;
				return _wcsdup(strPath.c_str());
			}
		}

		nRet = FindNextFileW(hFile, &fd);
	}


	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©
	return NULL;
}

wchar_t* FireFox3PlugIn::GetFavoriteDataPath()
{
	wchar_t* pBaseAppPath =GetAppBasePath();
	if( pBaseAppPath == NULL)
		return NULL;

	wstring strPath = pBaseAppPath;
	strPath += L"\\";
	strPath += L"places.sqlite";

	free(pBaseAppPath);
	return _wcsdup(strPath.c_str());
}

wchar_t* FireFox3PlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\data\\history.dat";

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©
	return _wcsdup(strPath.c_str());
}

BOOL FireFox3PlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	memset(pData,0x0, nDataNum*sizeof(FAVORITELINEDATA));

	CCRCHash ojbCrcHash;

	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	CppSQLite3DB m_SqliteDatabase;
	m_SqliteDatabase.open(GetFavoriteDataPath(), "");


	CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from moz_bookmarks where id in (2,3,4,5)");
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

BOOL FireFox3PlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
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

// ��ȡ�����ΪnParentId�����н�����Ŀ
int FireFox3PlugIn::GetFavoriteCount(int nParentId)
{
	if( m_pSqliteDatabase == NULL)
		m_pSqliteDatabase = new CppSQLite3DB();

	if( m_pSqliteDatabase->IsOpen() == false)
		m_pSqliteDatabase->open(GetFavoriteDataPath(), "");

	string strSql = "select count(*) from moz_bookmarks where parent = " ;
	strSql += StringHelper::ConvertFromInt(nParentId);

	CppSQLite3Query Query = m_pSqliteDatabase->execQuery(strSql.c_str());
	return StringHelper::ConvertToInt(Query.fieldValue(0));
}


int32 FireFox3PlugIn::GetFavoriteCount()
{
	/*
	static int nTotalNumber = 0;

	if( m_pSqliteDatabase == NULL)
		m_pSqliteDatabase = new CppSQLite3DB();


	wchar_t* pDBPath = GetFavoriteDataPath();

	if( m_pSqliteDatabase->IsOpen() == false)
		m_pSqliteDatabase->open(pDBPath, "");


	// 0 ��ʾ open�ɹ�
	if( m_pSqliteDatabase->IsOpen() == true)
	{
		CppSQLite3Query Query = m_pSqliteDatabase->execQuery("select * from moz_bookmarks where id in (2,3,4,5)");
		while(!Query.eof() )
		{
			int nId = Query.getIntField("id", 0);
			nTotalNumber += GetFavoriteCount(nId);

			Query.nextRow();
		}
	}
	*/
	wchar_t* pDBPath = GetFavoriteDataPath();
																						
	CppSQLite3DB  m_SqliteDatabase;
	m_SqliteDatabase.open(GetFavoriteDataPath(), "");

	bool b = m_SqliteDatabase.tableExists("moz_bookmarks");
	
	CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from moz_bookmarks");
	int n = Query.getIntField(0);


	return TRUE;
}

BOOL FireFox3PlugIn::SaveDatabase()
{
	return TRUE;
}

void FireFox3PlugIn::ReplaceSingleQuoteToDoubleQuote(wchar_t *pszOri)
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



