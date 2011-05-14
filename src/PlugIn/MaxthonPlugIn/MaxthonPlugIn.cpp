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

// �������ʵ��
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

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©

	return _wcsdup(strPath.c_str());
}

wchar_t* CMaxthonPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Maxthon3\\Users\\guest\\History\\History.dat";

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©

	return _wcsdup(strPath.c_str());
}

BOOL CMaxthonPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	return TRUE;
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
