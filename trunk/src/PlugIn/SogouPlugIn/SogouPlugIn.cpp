#include "stdafx.h"
#include "SogouPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "PathHelper.h"
#include "Decoder.h"
#include "CppSQLite3.h"

using namespace sogouplugin;

#pragma comment(lib, "shlwapi.lib")

namespace sogouplugin
{
	CSogouPlugIn*	g_SogouPlugIn	=	NULL;
}

// 导出借口实现
IPlugIn*	GetPlugIn()
{
	if( g_SogouPlugIn == NULL)
	{
		g_SogouPlugIn = new CSogouPlugIn();
		ASSERT( g_SogouPlugIn != NULL);
	}

	return g_SogouPlugIn;
}

void	ReleasePlugIn( IPlugIn* p)
{
	ASSERT( g_SogouPlugIn == p);
	if( g_SogouPlugIn  != NULL)
	{
		delete g_SogouPlugIn;
		g_SogouPlugIn = NULL;
	}
}

CSogouPlugIn aa;

CSogouPlugIn::CSogouPlugIn()
{
	GetInstallPath();
	GetPluginVersion();
	const wchar_t *p = GetFavoriteDataPath();
	std::string strDecodeContent;

	typedef struct _winFileMem 
	{
		unsigned char *pMemPointer;
		unsigned long  ulMemSize;
	}winFileMem;

	decode("C:\\Users\\linjinming.SNDA\\AppData\\Roaming\\SogouExplorer\\Favorite2.dat", strDecodeContent);

	CppSQLite3DB  m_SqliteDatabase;

	winFileMem stFileMem;

	stFileMem.ulMemSize = strDecodeContent.length();
	stFileMem.pMemPointer = new unsigned char[strDecodeContent.length()];
	memcpy(stFileMem.pMemPointer, strDecodeContent.c_str(), stFileMem.ulMemSize);

	m_SqliteDatabase.openmem((char *)&stFileMem, "");

	CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from dbInfo");

	int dbVer = Query.getIntField("value");

	//调用者自己进行释放
	free((void *)p);
}

CSogouPlugIn::~CSogouPlugIn()
{

}

BOOL CSogouPlugIn::IsWorked()
{
	return GetPluginVersion() != 0;
}

int32 CSogouPlugIn::GetPluginVersion()
{
	wchar_t szVersion[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szVersion); 
	int32   nVersion = 0;

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\SogouExplorer",
		L"DisplayVersion", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szVersion, 
		&dwSize))
	{
		std::string strVersion = StringHelper::UnicodeToANSI(szVersion);
		std::vector<std::string> vecResutl = StringHelper::Split(strVersion, '.');

		if (vecResutl.size() < 4)
		{
			return 0;
		}

		BYTE nMajaor = atol(vecResutl[0].c_str());
		BYTE nMinor  = atol(vecResutl[1].c_str());
		WORD nBuilderNum = atol(vecResutl[3].c_str());

		nVersion = MAKELONG(MAKEWORD(nMajaor, nMinor), nBuilderNum);

		return nVersion ;
	}

	return 0;
}

const wchar_t* CSogouPlugIn::GetBrowserName()
{
	return L"SogouExplorer";
}

const wchar_t* CSogouPlugIn::GetInstallPath()
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
		if (PathRemoveFileSpec(szPath))
		{
			MessageBox(NULL, szPath, szPath, NULL);
			return szPath;
		}
	}

	return NULL;
}

const wchar_t* CSogouPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\SogouExplorer";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _tcsdup(strPath.c_str());
}

const wchar_t* CSogouPlugIn::GetHistoryDataPath()
{
	return GetFavoriteDataPath();
}

BOOL CSogouPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	return TRUE;
}

BOOL CSogouPlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	return TRUE;
}

BOOL CSogouPlugIn::ImportFavoriteData( FAVORITELINEDATA stData )
{
	return TRUE;
}
