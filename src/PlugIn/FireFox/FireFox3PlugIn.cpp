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
#include "time.h"


using namespace firefox;

#pragma comment(lib, "shlwapi.lib")

#define BOOKMARKS_MENU	2
#define BOOKMARKS_TOOLSBAR	3
#define BOOKMARKS_TAG	4
#define BOOKMARKS_UNFILED	5

//==============================================================
// ��ǩ�˵�����Ҫ�ų��ļ�¼
// ��Щ��ϵͳ�Ĺؼ��֣�Ŀǰ�������������ͷ����
//==============================================================
#define BOOKMARKS_MENU_EXCLUED_SQL L" and type <> 3 and ( title <> '���ʹ�õ���ǩ' and title <> '���ʹ�õı�ǩ'  \
	and title <> 'Mozilla Firefox' and title <> '�������ĕ��`' and title<>'��������Ę˻`'  \
	and title <> '��ȡ��ǩ�������' and title <> 'ȡ�Õ��`���Ԫ��' and title <> 'Recently Bookmarked' \
	and title <> 'Recent Tags')"

#define BOOKMARKS_MENU_INCLUED_SQL L" and ( title = '���ʹ�õ���ǩ' or title = '���ʹ�õı�ǩ'  \
	or title = 'Mozilla Firefox' or title = '�������ĕ��`' or title = '��������Ę˻`'  \
	or title = '��ȡ��ǩ�������' or title = 'ȡ�Õ��`���Ԫ��'or type = 3 or title = 'Recently Bookmarked' \
	or title = 'Recent Tags')"

// ��ǩ��������Ҫ�ų��ļ�¼
#define BOOKMARKS_TOOLSBAR_EXCLUED_SQL	L" and type <> 3 and ( title <> '�������' and title <> '����ͷ��'  \
	and title <> '��g�[' and title <> '���r��' and title<> '������·' and title <> 'Most Visited' and title <> 'Latest Headlines')"

#define BOOKMARKS_TOOLSBAR_INCLUED_SQL	L" and ( title = '�������' or title = '����ͷ��'  \
	or title = '��g�[' or title = '���r��' or title = '������·' or type = 3 or title = 'Most Visited' or title = 'Latest Headlines')"


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
	wchar_t* pszFavoritePath = GetFavoriteDataPath();
	if( pszFavoritePath == NULL)
		return FALSE;

	if( PathFileExists(pszFavoritePath) == FALSE)
	{
		free(pszFavoritePath);
		return FALSE;
	}

   	if( m_pSqliteDatabase == NULL)
		m_pSqliteDatabase = new CppSQLite3DB();

	if( m_pSqliteDatabase->IsOpen() == false)
		m_pSqliteDatabase->open(pszFavoritePath, "");

	free(pszFavoritePath);

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
				size_t nIndex3 = wstrSubKey.find(L"Mozilla Firefox (3");
				size_t nIndex4 = wstrSubKey.find(L"Mozilla Firefox 4");
				size_t nIndex5 = wstrSubKey.find(L"Mozilla Firefox 5");
				if( 0xffffffff != nIndex3 || 0xffffffff != nIndex4 || 0xffffffff != nIndex5)
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

						return const_cast<wchar_t*>(_wcsdup(wstrPath.c_str()));
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
BOOL FireFox3PlugIn::ExportFavoriteData(int nParentId, PFAVORITELINEDATA* ppData, int32& nCurrentIndex)
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
			ppData[nCurrentIndex]->nPid = 0;
		}
		else
		{
			ppData[nCurrentIndex]->nPid = Query.getIntField("parent", 0);
		}

		ppData[nCurrentIndex]->nId = nId + ID_VALUE_FIREFOX_BEGIN;
		if (ppData[nCurrentIndex]->nPid != 0)
		{
			ppData[nCurrentIndex]->nPid += ID_VALUE_FIREFOX_BEGIN;
		}

		ppData[nCurrentIndex]->bFolder = Query.getIntField("type", 1) == 2 ? true : false;

		wcscpy_s(ppData[nCurrentIndex]->szTitle, MAX_PATH - 1, StringHelper::Utf8ToUnicode(Query.getStringField("title", 0)).c_str());
		ppData[nCurrentIndex]->szTitle[MAX_PATH - 1] = 0;

		ppData[nCurrentIndex]->nOrder = Query.getIntField("position", 0);
		ppData[nCurrentIndex]->nAddTimes = Query.getInt64Field("dateAdded", 0);
		ppData[nCurrentIndex]->nLastModifyTime = Query.getInt64Field("lastModified",0);

		ojbCrcHash.GetHash((BYTE *)ppData[nCurrentIndex]->szTitle, 
			wcslen(ppData[nCurrentIndex]->szTitle) * sizeof(wchar_t),
			(BYTE *)&ppData[nCurrentIndex]->nHashId, sizeof(uint32));
		ppData[nCurrentIndex]->nCatId = 0;
		ppData[nCurrentIndex]->bDelete = false;

		// ���Ҷ�Ӧ��URL
		string strUrlQuerySql = "select place.url from moz_bookmarks as marks,\
					moz_places as place where marks.id = " ;
		strUrlQuerySql += StringHelper::ConvertFromInt(nId);
		strUrlQuerySql += " and marks.fk = place.id";
		CppSQLite3Query urlQuery = m_pSqliteDatabase->execQuery(strUrlQuerySql.c_str());
		if(!urlQuery.eof())
		{
			wcscpy_s(ppData[nCurrentIndex]->szUrl, 1023, StringHelper::Utf8ToUnicode(urlQuery.getStringField("url", 0)).c_str());
			ppData[nCurrentIndex]->szUrl[1023] = 0;
		}

		nCurrentIndex++;

		ExportFavoriteData(nId, ppData, nCurrentIndex);
		
		Query.nextRow();
	}

	return TRUE;
}

BOOL FireFox3PlugIn::ExportFavoriteData( PFAVORITELINEDATA* ppData, int32& nDataNum )
{
	memset(ppData,0x0, nDataNum*sizeof(PFAVORITELINEDATA));

	if (ppData == NULL || *ppData == NULL || nDataNum == 0)
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
		ExportFavoriteData(nId,ppData,nCurrentIndex);

		Query.nextRow();
	}

	nDataNum = nCurrentIndex;

	return TRUE;
}

void FireFox3PlugIn::InsertIntoDB(int nRootId, PFAVORITELINEDATA* ppData, int32 nDataNum)
{
	if (ppData == NULL || *ppData == NULL || nDataNum == 0)
	{
		return ;
	}

#define MAX_BUFFER_LEN	4096

	wchar_t szInsert[MAX_BUFFER_LEN] = {0};

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
		if (ppData[i]->bDelete == true)
		{
			nBeginId--;
			continue;
		}

		ReplaceSingleQuoteToDoubleQuote(ppData[i]->szTitle);
		ReplaceSingleQuoteToDoubleQuote(ppData[i]->szUrl);

		int nFk = -1;
		if( wcscspn(ppData[i]->szUrl,L"") != 0)
		{
			// ���url�Ƿ����
			swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"select * from moz_places where url ='%s'",
				ppData[i]->szUrl);
			CppSQLite3Query urlQuery = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(szInsert).c_str());

			// ��URL�����ڣ������һ���µļ�¼
			if( urlQuery.eof() == true)
			{
				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_places "
					L"(url,title) "
					L" values('%s','%s')",
					ppData[i]->szUrl,
					ppData[i]->szTitle);
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

		time_t nowTime;
		time(&nowTime);

		if( ppData[i]->nPid == 0)
		{
			if( nFk != -1)
			{
				// ��ǩ�˵���Ŀ
				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_bookmarks "
					L"(parent,type,title,position,dateAdded,lastModified,fk) "
					L" values(%d,%d,'%s',%d,%d,%d,%d)",
					nRootId,
					ppData[i]->bFolder == true ? 2 : 1,
					ppData[i]->szTitle,
					ppData[i]->nOrder,
					(int)nowTime, 
					(int)nowTime,
					nFk
				);
			}
			else
			{
				// ��ǩ�˵���Ŀ
				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_bookmarks " \
					L" (parent,type,title,position,lastModified,dateAdded) " \
					L" values(%d,%d,'%s',%d,%d,%d)",
					nRootId,
					ppData[i]->bFolder == true ? 2 : 1,
					ppData[i]->szTitle,
					ppData[i]->nOrder,
					(int)nowTime,
					(int)nowTime);
			}
			
		}
		else
		{
			if( nFk != -1)
			{
				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_bookmarks "
					L"(parent,type,title,position,dateAdded,lastModified,fk) "
					L" values(%d,%d,'%s',%d,%d,%d, %d)",
					ppData[i]->nPid + nBeginId,
					ppData[i]->bFolder == true ? 2 : 1,
					ppData[i]->szTitle,
					ppData[i]->nOrder,
					(int)nowTime, 
					(int)nowTime,
					nFk
					);
			}
			else
			{
				swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into moz_bookmarks "
					L"(parent,type,title,position,dateAdded,lastModified) "
					L" values(%d,%d,'%s',%d,%d,%d)",
					ppData[i]->nPid + nBeginId,
					ppData[i]->bFolder == true ? 2 : 1,
					ppData[i]->szTitle,
					ppData[i]->nOrder,
					(int)nowTime, 
					(int)nowTime	
					);
			}
		}
		m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());
	}
}

BOOL FireFox3PlugIn::ImportFavoriteData( PFAVORITELINEDATA* ppData, int32 nDataNum )
{
	if (ppData == NULL || nDataNum == 0)
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

	wstring wstrDeleteSql =	 L"delete from moz_places where id in (select fk from moz_bookmarks where id not in (";
	wstrDeleteSql += strId + L"))";
	m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(wstrDeleteSql).c_str());

	wstrDeleteSql = L"delete from moz_bookmarks where id not in (";
	wstrDeleteSql += strId + L")";
	m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(wstrDeleteSql).c_str());

	InsertIntoDB(2, ppData, nDataNum);
	InsertIntoDB(3, ppData, nDataNum);

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