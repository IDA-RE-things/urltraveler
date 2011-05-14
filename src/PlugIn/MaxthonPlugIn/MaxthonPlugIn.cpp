#include "stdafx.h"
#include "MaxthonPlugIn.h"
#include <string>
#include "Decoder.h"
#include "StringHelper.h"
#include <shlwapi.h>
#include "PathHelper.h"
#include "CppSQLite3.h"

#pragma comment(lib, "shlwapi.lib")

namespace maxthonplugin
{
	CMaxthonPlugIn*	g_MaxthonPlugIn	=	NULL;
}

// 导出借口实现
IPlugIn*	GetPlugIn()
{
	if( g_MaxthonPlugIn == NULL)
	{
		g_MaxthonPlugIn = new CMaxthonPlugIn();
		ASSERT( g_MaxthonPlugIn != NULL);
	}

	return g_MaxthonPlugIn;
}

void	ReleasePlugIn( IPlugIn* p)
{
	ASSERT( g_MaxthonPlugIn == p);
	if( g_MaxthonPlugIn  != NULL)
	{
		delete g_MaxthonPlugIn;
		g_MaxthonPlugIn = NULL;
	}
}



CMaxthonPlugIn::CMaxthonPlugIn()
{
	Load();
	int32 nCount = GetFavoriteCount();
	UnLoad();

}

CMaxthonPlugIn::~CMaxthonPlugIn()
{

}

BOOL CMaxthonPlugIn::Load()
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

BOOL CMaxthonPlugIn::UnLoad()
{
	if (m_pMemFavoriteDB)
	{
		FILE *fOut = fopen("maxthon_b.db", "wb");
		fwrite(m_pMemFavoriteDB->pMemPointer, 1, m_pMemFavoriteDB->ulMemSize, fOut);
		fclose(fOut);
		free(m_pMemFavoriteDB->pMemPointer);
		free(m_pMemFavoriteDB);
		m_pMemFavoriteDB = NULL;
		return TRUE;
	}

	return FALSE;

}

BOOL CMaxthonPlugIn::IsWorked( wchar_t *pszBrowserVersion, int32 &nLen )
{
	return TRUE;
}

int32 CMaxthonPlugIn::GetPluginVersion()
{
	return 1;
}

const wchar_t* CMaxthonPlugIn::GetBrowserName()
{
	return L"Maxthon";
}

wchar_t* CMaxthonPlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Maxthon3",
		L"DisplayIcon", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		if (PathRemoveFileSpec(szPath))
		{
			wcscat(szPath, L"\\Maxthon.exe");
			return _wcsdup(szPath);
		}
	}

	return NULL;
}

wchar_t* CMaxthonPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Maxthon3\\Users\\guest\\Favorite\\Favorite.dat";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _wcsdup(strPath.c_str());
}

wchar_t* CMaxthonPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Maxthon3\\Users\\guest\\History\\History.dat";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _wcsdup(strPath.c_str());
}

BOOL CMaxthonPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from MyFavNodes");
		int i = 0;

		while(!Query.eof() && i < nDataNum)
		{
			int nBlobLen = 0;
			Query.getBlobField("parent_id", nBlobLen);
			Query.getBlobField("id", nBlobLen);

			pData[i].bFolder = Query.getIntField("type", 0);
			_tcscpy_s(pData[i].szTitle, MAX_PATH - 1, StringHelper::Utf8ToUnicode(Query.getStringField("title", 0)).c_str());
			pData[i].szTitle[MAX_PATH - 1] = 0;
			_tcscpy_s(pData[i].szUrl, 1023, StringHelper::Utf8ToUnicode(Query.getStringField("url", 0)).c_str());
			pData[i].szUrl[1023] = 0;
			pData[i].nOrder = Query.getIntField("sequenceNO", 0);

			pData[i].nCatId = Query.getIntField("category", 0);

			Query.nextRow();
			i++;
		}

		nDataNum = i;

		return TRUE;
	}

	return FALSE;
}

BOOL CMaxthonPlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	return TRUE;
}

int32 CMaxthonPlugIn::GetFavoriteCount()
{
	int32 nCount = 0;

	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select count(*) as Total from MyFavNodes");

		nCount = Query.getIntField("Total");
	}

	return nCount;

}
