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
}


TTPlugIn::~TTPlugIn()
{
	
}

BOOL TTPlugIn::Load()
{
	return TRUE;
}

BOOL TTPlugIn::UnLoad()
{
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
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Tencent\\QQBrowser\\user_data\\0\\Bookmarks";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

wchar_t* TTPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Tencent\\QQBrowser\\user_data\\0\\history.db";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

BOOL TTPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	nDataNum = 0;
	return TRUE;
}

BOOL TTPlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}


#define MAX_BUFFER_LEN	4096

	return TRUE;
}

int32 TTPlugIn::GetFavoriteCount()
{
	return  0;
}