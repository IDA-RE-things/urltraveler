#include "stdafx.h"
#include "PlugIn.h"
#include "TTPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include "CppSQLite3.h"
#include <algorithm>
#include "CRCHash.h"

#pragma comment(lib, "shlwapi.lib")


TTPlugIn::TTPlugIn()
{
	m_pSqliteDatabase = NULL;
}

TTPlugIn::~TTPlugIn()
{
	UnLoad();
}

BOOL TTPlugIn::Load()
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

BOOL TTPlugIn::UnLoad()
{
	if( m_pSqliteDatabase != NULL)
	{
		m_pSqliteDatabase->close();
		delete m_pSqliteDatabase;
		m_pSqliteDatabase = NULL;
	}

	return TRUE;
}

int32 TTPlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* TTPlugIn::GetBrowserName()
{
	return L"腾讯TT浏览器";
}

wchar_t* TTPlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Tencent Traveler",
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

wchar_t* TTPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Tencent\\TencentTraveler\\100\\navinfo.db";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

wchar_t* TTPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Tencent\\TencentTraveler\\100\\navinfo.db";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

BOOL TTPlugIn::WriteQueryDataIntoData(CppSQLite3Query* pQuery, PFAVORITELINEDATA pData)
{
	CCRCHash ojbCrcHash;

	if( pQuery == NULL || pData == NULL)
		return FALSE;

	int nIndex = pQuery->getIntField("iIndex", 0);
	int nUrlIndex = pQuery->getIntField("iUrlIndex", 0);

	pData->nPid = pQuery->getIntField("iPIndex", 0);
	pData->nId = nIndex + ID_VALUE_TT_BEGIN;
	if (pData->nPid != 0)
	{
		pData->nPid += ID_VALUE_TT_BEGIN;
	}

	pData->bFolder = pQuery->getIntField("iUrlIndex", 0) == 0 ? true : false;

	wcscpy_s(pData->szTitle, MAX_PATH - 1, StringHelper::Utf8ToUnicode(pQuery->getStringField("strTitle", 0)).c_str());
	pData->szTitle[MAX_PATH - 1] = 0;

	pData->nOrder = pQuery->getIntField("iPos", 0);
	pData->nAddTimes = pQuery->getInt64Field("iCreateTime", 0);
	pData->nLastModifyTime = pQuery->getInt64Field("iLastModifyTime",0);

	ojbCrcHash.GetHash((BYTE *)pData->szTitle, 
		wcslen(pData->szTitle) * sizeof(wchar_t),
		(BYTE *)&pData->nHashId, sizeof(uint32));

	pData->nCatId = 0;
	pData->bDelete = false;

	// 查找对应的URL
	string strUrlQuerySql = "select url.strUrl from urltable as url,\
							favtable as fav where fav.iUrlIndex = " ;
	strUrlQuerySql += StringHelper::ConvertFromInt(nUrlIndex);
	strUrlQuerySql += " and url.iIndex = fav.iUrlIndex";

	CppSQLite3Query urlQuery = m_pSqliteDatabase->execQuery(strUrlQuerySql.c_str());
	if(!urlQuery.eof())
	{
		wcscpy_s(pData->szUrl, 1023, StringHelper::Utf8ToUnicode(urlQuery.getStringField("strUrl", 0)).c_str());
		pData->szUrl[1023] = 0;
	}

	return TRUE;
}

// 导出所有父结点为nParentId的结点的收藏数据
// nDataNum为导出的收藏数据
BOOL TTPlugIn::ExportFavoriteData(int nParentId, PFAVORITELINEDATA* ppData, int32& nCurrentIndex)
{
	wstring strSql = L"select * from favtable where iPIndex = " ;
	strSql += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(nParentId));

	CCRCHash ojbCrcHash;

	CppSQLite3Query Query = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(strSql).c_str());
	while(!Query.eof())
	{
		int nIndex = Query.getIntField("iIndex", 0);
		int nUrlIndex = Query.getIntField("iUrlIndex", 0);

		WriteQueryDataIntoData(&Query,ppData[nCurrentIndex]); 

		nCurrentIndex++;
		ExportFavoriteData(nIndex, ppData, nCurrentIndex);
		
		Query.nextRow();
	}

	return TRUE;
}

BOOL TTPlugIn::ExportFavoriteData( PFAVORITELINEDATA* ppData, int32& nDataNum )
{
	if (ppData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	string strSql = "select fav.* from favtable as fav where fav.iPIndex = 0";

	CppSQLite3Query Query = m_pSqliteDatabase->execQuery(strSql.c_str());

	// 当前插入的位置
	int nCurrentIndex = 0;
	while(!Query.eof())
	{
		int nId = Query.getIntField("iIndex", 0);
		
		// 先记录nIndex为nId的结点的信息
		WriteQueryDataIntoData(&Query, ppData[nCurrentIndex]);
		nCurrentIndex ++;
		ExportFavoriteData(nId,ppData,nCurrentIndex);

		Query.nextRow();
	}

	nDataNum = nCurrentIndex;

	return TRUE;
}

BOOL TTPlugIn::ImportFavoriteData( PFAVORITELINEDATA* ppData, int32 nDataNum )
{
	if (ppData == NULL || *ppData == NULL || nDataNum == 0)
	{
		return FALSE;
	}


#define MAX_BUFFER_LEN	4096

	return TRUE;
}

// 获取父结点为nParentId的所有结点的数目
int TTPlugIn::GetFavoriteCount(int nParentId)
{
	int nCount = 0;

	wstring strSql = L"select count(*) as total from favtable where iPIndex = " ;
	strSql += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(nParentId));

	CppSQLite3Query Query = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(strSql).c_str());
	const char* pTotal = Query.fieldValue("total");
	nCount =  StringHelper::ConvertToInt(Query.fieldValue("total"));

	strSql = L"select * from favtable where iPIndex = " ;
	strSql += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(nParentId));

	Query = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(strSql).c_str());
	while(!Query.eof())
	{
		int nId = Query.getIntField("iIndex", 0);
		nCount += GetFavoriteCount(nId);

		Query.nextRow();
	}

	return nCount;
}

int32 TTPlugIn::GetFavoriteCount()
{
	int nTotalNumber = 0;

	CppSQLite3Query Query = m_pSqliteDatabase->execQuery("select * from favtable where iPIndex=0");
	while(!Query.eof() )
	{
		nTotalNumber += 1;
		int nIndex = Query.getIntField("iIndex", 0);
		nTotalNumber += GetFavoriteCount(nIndex);

		Query.nextRow();
	}

	return nTotalNumber;
}

void TTPlugIn::InsertIntoDB(int nRootId, PFAVORITELINEDATA* ppData, int32 nDataNum)
{
	if (ppData == NULL || *ppData == NULL || nDataNum == 0)
	{
		return ;
	}

#define MAX_BUFFER_LEN	4096

	wchar_t szInsert[MAX_BUFFER_LEN] = {0};

	// 清空收藏夹数据库
	int nBeginId = 0;
	swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"delete from favtable ");
	CppSQLite3Query selectQuery = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(szInsert).c_str());

	for (int i = 0; i < nDataNum; i++)
	{
		if (ppData[i]->bDelete == true)
		{
			nBeginId--;
			continue;
		}

/*
				ReplaceSingleQuoteToDoubleQuote(ppData[i]->szTitle);
				ReplaceSingleQuoteToDoubleQuote(ppData[i]->szUrl);
		
				int nFk = -1;
				if( wcscspn(ppData[i]->szUrl,L"") != 0)
				{
					// 检查url是否存在
					swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"select * from urltable where strUrl ='%s'",
						ppData[i]->szUrl);
					CppSQLite3Query urlQuery = m_pSqliteDatabase->execQuery(StringHelper::UnicodeToUtf8(szInsert).c_str());
		
					// 该URL不存在，则插入一条新的记录
					if( urlQuery.eof() == true)
					{
						swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into urltable "
							L"(strUrl,strTitle,iLastVisitTime,iVisitCount) "
							L" values('%s','%s')",
							ppData[i]->szUrl,
							ppData[i]->szTitle,time(NULL), 1);
						m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());
		
						swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"select max(iIndex) from urltable ");
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
				}*/
		
		//m_pSqliteDatabase->execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());
	}
}