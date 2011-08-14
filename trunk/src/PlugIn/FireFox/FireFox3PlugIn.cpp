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
// 标签菜单中需要排除的记录
// 这些是系统的关键字，目前仅仅包括简体版和繁体版
//==============================================================
#define BOOKMARKS_MENU_EXCLUED_SQL L" and type <> 3 and ( title <> '最近使用的书签' and title <> '最近使用的标签'  \
	and title <> 'Mozilla Firefox' and title <> '最近加入的書籤' and title<>'最近新增的標籤'  \
	and title <> '获取书签附加组件' and title <> '取得書籤類附加元件' and title <> 'Recently Bookmarked' \
	and title <> 'Recent Tags')"

#define BOOKMARKS_MENU_INCLUED_SQL L" and ( title = '最近使用的书签' or title = '最近使用的标签'  \
	or title = 'Mozilla Firefox' or title = '最近加入的書籤' or title = '最近新增的標籤'  \
	or title = '获取书签附加组件' or title = '取得書籤類附加元件'or type = 3 or title = 'Recently Bookmarked' \
	or title = 'Recent Tags')"

// 标签工具栏中要排除的记录
#define BOOKMARKS_TOOLSBAR_EXCLUED_SQL	L" and type <> 3 and ( title <> '访问最多' and title <> '最新头条'  \
	and title <> '最常瀏覽' and title <> '即時新聞' and title<> '新手上路' and title <> 'Most Visited' and title <> 'Latest Headlines')"

#define BOOKMARKS_TOOLSBAR_INCLUED_SQL	L" and ( title = '访问最多' or title = '最新头条'  \
	or title = '最常瀏覽' or title = '即時新聞' or title = '新手上路' or type = 3 or title = 'Most Visited' or title = 'Latest Headlines')"


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

	// 获取键信息(主要是子键数和键值数, 用于下面遍历)   
	// 注: 也可以不用主动获取数量, 而由枚举函数(RegEnumXXX)返回 ERROR_NO_MORE_ITEMS 来判断枚举结束.   
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

	const long MAX_KEY = 255;                       // 键最大长度   
	wchar_t ptszSubKey[MAX_KEY] = _T("");             // 子键字符串   
	DWORD cbSubKey = MAX_KEY; 

	// 遍历子键   
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
						// firefox3.0的路径格式为 C:\Program Files\Mozilla Firefox\firefox.exe,0
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

// 因为Firefox的目录比较特殊，它位于C:\Users\zhangzq\AppData\Roaming\Mozilla\Firefox\Profiles\*.default
// 目录下，*为一随即数，因此必须先计算出该目录
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


	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
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

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

// 获取所有的系统的记录ID
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

// 导出所有父结点为nParentId的结点的收藏数据
// nDataNum为导出的收藏数据
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

		// 查找对应的URL
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

	// 当前插入的位置
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

	// 找到插入的起始id
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
			// 检查url是否存在
			swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"select * from moz_places where url ='%s'",
				ppData[i]->szUrl);
			CppSQLite3Query urlQuery = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(szInsert).c_str());

			// 该URL不存在，则插入一条新的记录
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
				// 书签菜单栏目
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
				// 书签菜单栏目
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

// 获取父结点为nParentId的所有结点的数目
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