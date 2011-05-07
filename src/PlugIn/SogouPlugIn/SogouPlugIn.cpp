#include "stdafx.h"
#include "SogouPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "PathHelper.h"

#pragma comment(lib, "shlwapi.lib")

CSogouPlugIn aa;

CSogouPlugIn::CSogouPlugIn()
{
	GetInstallPath();
	GetPluginVersion();
	const wchar_t *p = GetFavoriteDataPath();

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

	//需要复制一份，由调用者进行释放，不然strPath被析构时，返回野指针

	return _tcsdup(strPath.c_str());
}

const wchar_t* CSogouPlugIn::GetHistoryDataPath()
{
	return NULL;
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
