#include "stdafx.h"
#include "PlugIn.h"
#include "360SE3PlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include "CppSQLite3.h"
#include <algorithm>
#include "CRCHash.h"
#include "360SE2PlugIn.h"

#pragma comment(lib, "shlwapi.lib")


C360SE2PlugIn::C360SE2PlugIn()
{
}


C360SE2PlugIn::~C360SE2PlugIn()
{
	
}

BOOL C360SE2PlugIn::Load()
{
	return TRUE;
}

BOOL C360SE2PlugIn::UnLoad()
{
	return TRUE;
}

BOOL C360SE2PlugIn::IsWorked()
{
	// 360SE 2.0��360SE3.0���ղؼз�ʽ����ͬ
	// 360 2.0ʹ�ú�IEһ�µ��ղؼС�360 3.0���������ݿ���д洢

	// 2.0  
	// {66D8959E-B7E9-4cd4-BC16-98711D815F2A}
	// DisplayIcon	C:\Program Files\360\360se\360SE.exe

/*
	wchar_t szVersion[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szVersion); 
	int32   nVersion = 0;

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{23F3F476-BE34-4f48-9C77-2806A8393EC4}",
		L"DisplayVersion", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szVersion, 
		&dwSize))
	{
		return TRUE;
	}
*/

	return FALSE;
}

int32 C360SE2PlugIn::GetPluginVersion()
{
	return 1;
}

const wchar_t* C360SE2PlugIn::GetBrowserName()
{
	return L"360SE2.0";
}

wchar_t* C360SE2PlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{23F3F476-BE34-4f48-9C77-2806A8393EC4}",
		L"UninstallString", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		return _wcsdup(szPath);
	}

	return NULL;
}

wchar_t* C360SE2PlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\360se\\data\\360sefav.db";

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©
	return _wcsdup(strPath.c_str());
}

wchar_t* C360SE2PlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\data\\history.dat";

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©
	return _wcsdup(strPath.c_str());
}

BOOL C360SE2PlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	return TRUE;
}

BOOL C360SE2PlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	return TRUE;
}

int32 C360SE2PlugIn::GetFavoriteCount()
{
	return 0;
}