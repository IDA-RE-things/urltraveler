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

#define BOOKMARKS_MENU	2
#define BOOKMARKS_TOOLSBAR	3
#define BOOKMARKS_TAG	4
#define BOOKMARKS_UNFILED	5

// ��ǩ�˵�����Ҫ�ų��ļ�¼
#define BOOKMARKS_MENU_EXCLUED_SQL L" and type <> 3 and ( title <> '���ʹ�õ���ǩ' and title <> '���ʹ�õı�ǩ'  \
	and title <> 'Mozilla Firefox' and title <> '�������ĕ��`' and title<>'��������Ę˻`'  \
	and title <> '��ȡ��ǩ�������' and title <> 'ȡ�Õ��`���Ԫ��')"

#define BOOKMARKS_MENU_INCLUED_SQL L" and ( title = '���ʹ�õ���ǩ' or title = '���ʹ�õı�ǩ'  \
	or title = 'Mozilla Firefox' or title = '�������ĕ��`' or title = '��������Ę˻`'  \
	or title = '��ȡ��ǩ�������' or title = 'ȡ�Õ��`���Ԫ��'or type = 3)"

// ��ǩ��������Ҫ�ų��ļ�¼
#define BOOKMARKS_TOOLSBAR_EXCLUED_SQL	L" and type <> 3 and ( title <> '�������' and title <> '����ͷ��'  \
			and title <> '��g�[' and title <> '���r��' and title<> '������·')"

#define BOOKMARKS_TOOLSBAR_INCLUED_SQL	L" and ( title = '�������' or title = '����ͷ��'  \
			or title = '��g�[' or title = '���r��' or title = '������·' or type = 3)"


FireFox3PlugIn::FireFox3PlugIn()
{
	m_pSqliteDatabase = NULL;
}

FireFox3PlugIn::~FireFox3PlugIn()
{
	UnLoad();
}

BOOL FireFox3PlugIn::Load()
{
   	if( m_pSqliteDatabase == NULL)
		m_pSqliteDatabase = new CppSQLite3DB();

	if( m_pSqliteDatabase->IsOpen() == false)
		m_pSqliteDatabase->open(GetFavoriteDataPath(), "");

	if( m_pSqliteDatabase->IsOpen() == false)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL FireFox3PlugIn::UnLoad()
{
	if( m_pSqliteDatabase != NULL)
	{
		m_pSqliteDatabase->close();
		delete m_pSqliteDatabase;
		m_pSqliteDatabase = NULL;
	}

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

// ��ȡ���е�ϵͳ�ļ�¼ID
void FireFox3PlugIn::GetSystemFavoriteId(int nParentId,std::vector<int>& vIdData)
{
	wstring strSql = L"select id from moz_bookmarks where parent = " ;
	strSql += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(nParentId));
	if( nParentId == BOOKMARKS_MENU)
	{
		strSql += BOOKMARKS_MENU_INCLUED_SQL;
	}
	else if ( nParentId == BOOKMARKS_TOOLSBAR)
	{
		strSql += BOOKMARKS_TOOLSBAR_INCLUED_SQL;
	}

	CppSQLite3Query Query = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(strSql).c_str());
	while(!Query.eof())
	{
		int nId = Query.getIntField("id", 0);
		vIdData.push_back(nId);

		GetSystemFavoriteId(nId, vIdData);	

		Query.nextRow();
	}
}

void FireFox3PlugIn::GetSystemFavoriteId(std::vector<int>& vIdData)
{
	wchar_t* pDBPath = GetFavoriteDataPath();

	vIdData.push_back(1);
	GetSystemFavoriteId(1, vIdData);	
}

// �������и����ΪnParentId�Ľ����ղ�����
// nDataNumΪ�������ղ�����
BOOL FireFox3PlugIn::ExportFavoriteData(int nParentId, PFAVORITELINEDATA pData, int32& nCurrentIndex)
{
	wstring strSql = L"select * from moz_bookmarks where parent = " ;
	strSql += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(nParentId));
	if( nParentId == BOOKMARKS_MENU)
	{
		strSql += BOOKMARKS_MENU_EXCLUED_SQL;
	}
	else if ( nParentId == BOOKMARKS_TOOLSBAR)
	{
		strSql += BOOKMARKS_TOOLSBAR_EXCLUED_SQL;
	}

	CCRCHash ojbCrcHash;

	CppSQLite3Query Query = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(strSql).c_str());
	while(!Query.eof())
	{
		int nId = Query.getIntField("id", 0);

		if( nParentId == 2 || nParentId == 3 || nParentId == 4 || nParentId == 5)
		{
			pData[nCurrentIndex].nPid = 0;
		}
		else
		{
			pData[nCurrentIndex].nPid = Query.getIntField("parent", 0);
		}

		pData[nCurrentIndex].nId = nId + ID_VALUE_FIREFOX_BEGIN;
		if (pData[nCurrentIndex].nPid != 0)
		{
			pData[nCurrentIndex].nPid += ID_VALUE_FIREFOX_BEGIN;
		}

		pData[nCurrentIndex].bFolder = Query.getIntField("type", 1) == 2 ? true : false;

		wcscpy_s(pData[nCurrentIndex].szTitle, MAX_PATH - 1, StringHelper::Utf8ToUnicode(Query.getStringField("title", 0)).c_str());
		pData[nCurrentIndex].szTitle[MAX_PATH - 1] = 0;

		pData[nCurrentIndex].nOrder = Query.getIntField("position", 0);
		pData[nCurrentIndex].nAddTimes = Query.getInt64Field("dateAdded", 0);
		pData[nCurrentIndex].nLastModifyTime = Query.getInt64Field("lastModified",0);

		ojbCrcHash.GetHash((BYTE *)pData[nCurrentIndex].szTitle, 
			wcslen(pData[nCurrentIndex].szTitle) * sizeof(wchar_t),
			(BYTE *)&pData[nCurrentIndex].nHashId, sizeof(uint32));
		pData[nCurrentIndex].nCatId = 0;
		pData[nCurrentIndex].bDelete = false;

		// ���Ҷ�Ӧ��URL
		string strUrlQuerySql = "select place.url from moz_bookmarks as marks,\
					moz_places as place where marks.id = " ;
		strUrlQuerySql += StringHelper::ConvertFromInt(nId);
		strUrlQuerySql += " and marks.fk = place.id";
		CppSQLite3Query urlQuery = m_pSqliteDatabase->execQuery(strUrlQuerySql.c_str());
		if(!urlQuery.eof())
		{
			wcscpy_s(pData[nCurrentIndex].szUrl, 1023, StringHelper::Utf8ToUnicode(urlQuery.getStringField("url", 0)).c_str());
			pData[nCurrentIndex].szUrl[1023] = 0;
		}

		nCurrentIndex++;

		ExportFavoriteData(nId, pData, nCurrentIndex);
		
		Query.nextRow();
	}

	return TRUE;
}

BOOL FireFox3PlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	memset(pData,0x0, nDataNum*sizeof(FAVORITELINEDATA));

	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	string strSql = "select marks.* from moz_bookmarks as marks where marks.id in (2,3,4,5)";
	CppSQLite3Query Query = m_pSqliteDatabase->execQuery(strSql.c_str());

	// ��ǰ�����λ��
	int nCurrentIndex = 0;
	while(!Query.eof())
	{
		int nId = Query.getIntField("id", 0);
		ExportFavoriteData(nId,pData,nCurrentIndex);

		Query.nextRow();
	}

	nDataNum = nCurrentIndex;

	return TRUE;
}

BOOL FireFox3PlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

#define MAX_BUFFER_LEN	4096

	std::vector<int> v;
	GetSystemFavoriteId(v);

	wstring strId = L"";
	for( int i=0; i<v.size(); i++)
	{
		strId += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(v.at(i)));
		strId += L",";
	}
	strId.erase( strId.find_last_not_of(L",") + 1, strId.length()-1);


	wchar_t szInsert[MAX_BUFFER_LEN] = {0};
	wchar_t szDelete[MAX_BUFFER_LEN] = {0};

	int i = 0;

	wstring wstrDeleteSql =	 L"delete from moz_places where id in (select fk from moz_bookmarks where id not in (";
	wstrDeleteSql += strId + L"))";
	m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(wstrDeleteSql).c_str());

	wstrDeleteSql = L"delete from moz_bookmarks where id not in (";
	wstrDeleteSql += strId + L")";
	m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(wstrDeleteSql).c_str());

	// �ҵ��������ʼid
	int nBeginId = 0;
	swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"select max(id) from moz_bookmarks ");
	CppSQLite3Query selectQuery = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(szInsert).c_str());
	if( selectQuery.eof() == false)
	{
		nBeginId = selectQuery.getIntField(0);
	}

	for (int i = 0; i < nDataNum; i++)
	{
		if (pData[i].bDelete == true)
		{
			nBeginId--;
			continue;
		}

		ReplaceSingleQuoteToDoubleQuote(pData[i].szTitle);
		ReplaceSingleQuoteToDoubleQuote(pData[i].szUrl);

		int nFk = -1;
		if( wcscspn(pData[i].szUrl,L"") != 0)
		{
			// ���url�Ƿ����
			swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"select * from moz_places where url ='%s'",
				pData[i].szUrl);
			CppSQLite3Query urlQuery = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(szInsert).c_str());

			// ��URL�����ڣ������һ���µļ�¼
			if( urlQuery.eof() == true)
			{
				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_places "
					L"(url,title) "
					L" values('%s','%s')",
					pData[i].szUrl,
					pData[i].szTitle);
				m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());

				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"select max(id) from moz_places ");
				CppSQLite3Query selectQuery = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(szInsert).c_str());
				if( selectQuery.eof() == false)
				{
					nFk = selectQuery.getIntField(0);
				}
			}
			else
			{
				nFk = urlQuery.getIntField(0);
			}
		}

		if( pData[i].nPid == 0)
		{
			// ��ǩ�˵���Ŀ
			swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_bookmarks "
				L"(parent,type,title,position,dateAdded,lastModified,fk) "
				L" values(%d,%d,'%s',%d,%d,%d, %d)",
				3,
				pData[i].bFolder == true ? 2 : 1,
				pData[i].szTitle,
				pData[i].nOrder,
				L"2011-05-11 12:00:00", 
				L"2011-05-11 12:00:00",
				nFk
				);
		}
		else
		{
			if( nFk != -1)
			{
				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_bookmarks "
					L"(parent,type,title,position,dateAdded,lastModified,fk) "
					L" values(%d,%d,'%s',%d,%d,%d, %d)",
					pData[i].nPid + nBeginId,
					pData[i].bFolder == true ? 2 : 1,
					pData[i].szTitle,
					pData[i].nOrder,
					L"2011-05-11 12:00:00", 
					L"2011-05-11 12:00:00",
					nFk
					);
			}
			else
			{
				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_bookmarks "
					L"(parent,type,title,position,dateAdded,lastModified) "
					L" values(%d,%d,'%s',%d,%d,%d)",
					pData[i].nPid + nBeginId,
					pData[i].bFolder == true ? 2 : 1,
					pData[i].szTitle,
					pData[i].nOrder,
					L"2011-05-11 12:00:00", 
					L"2011-05-11 12:00:00"
					);
			}
		}
		m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());

		/*
		// ��ǩ������
		pData[i].nPid = 3;
		swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_bookmarks "
		L"(parent,type,title,position,dateAdded,lastModified, fk) "
		L" values(%d,%d,'%s',%d,%d,%d, %d)",
		pData[i].nPid,
		pData[i].bFolder == true ? 2 : 1,
		pData[i].szTitle,
		pData[i].nOrder,
		L"2011-05-11 12:00:00", 
		L"2011-05-11 12:00:00",
		nFk);
		m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());
		*/

	}

	return TRUE;
}

// ��ȡ�����ΪnParentId�����н�����Ŀ
int FireFox3PlugIn::GetFavoriteCount(int nParentId)
{
	int nCount = 0;

	wstring strSql = L"select count(*) as total from moz_bookmarks where parent = " ;
	strSql += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(nParentId));

	if( nParentId == BOOKMARKS_MENU)
	{
		strSql += BOOKMARKS_MENU_EXCLUED_SQL;
	}
	else if ( nParentId == BOOKMARKS_TOOLSBAR)
	{
		strSql += BOOKMARKS_TOOLSBAR_EXCLUED_SQL;
	}


	CppSQLite3Query Query = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(strSql).c_str());
	const char* pTotal = Query.fieldValue("total");
	nCount =  StringHelper::ConvertToInt(Query.fieldValue("total"));

	strSql = L"select * from moz_bookmarks where parent = " ;
	strSql += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(nParentId));
	if( nParentId == BOOKMARKS_MENU)
	{
		strSql += BOOKMARKS_MENU_EXCLUED_SQL;
	}
	else if ( nParentId == BOOKMARKS_TOOLSBAR)
	{
		strSql += BOOKMARKS_TOOLSBAR_EXCLUED_SQL;
	}

	Query = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(strSql).c_str());
	while(!Query.eof())
	{
		int nId = Query.getIntField("id", 0);
		nCount += GetFavoriteCount(nId);

		Query.nextRow();
	}

	return nCount;
}

int32 FireFox3PlugIn::GetFavoriteCount()
{
	int nTotalNumber = 0;

	CppSQLite3Query Query = m_pSqliteDatabase->execQuery("select * from moz_bookmarks where id in(2,3,4,5)");
	while(!Query.eof() )
	{
		int nId = Query.getIntField("id", 0);
		nTotalNumber += GetFavoriteCount(nId);

		Query.nextRow();
	}

	return nTotalNumber;
}