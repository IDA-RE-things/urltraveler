#include "stdafx.h"
#include "SogouPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"

#pragma comment(lib, "shlwapi.lib")

CSogouPlugIn::CSogouPlugIn()
{
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


		return nVersion;
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
			return szPath;
		}
	}

	return NULL;
}

const wchar_t* CSogouPlugIn::GetFavoriteDataPath()
{
	return NULL;
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
