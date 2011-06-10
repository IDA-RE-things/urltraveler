#include "stdafx.h"
#include "PlugIn.h"
#include "QQPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include "CppSQLite3.h"
#include <algorithm>
#include "CRCHash.h"

#pragma comment(lib, "shlwapi.lib")


QQPlugIn::QQPlugIn()
{
}


QQPlugIn::~QQPlugIn()
{
	
}

BOOL QQPlugIn::Load()
{
	return TRUE;
}

BOOL QQPlugIn::UnLoad()
{
	return TRUE;
}

int32 QQPlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* QQPlugIn::GetBrowserName()
{
	return L"腾讯QQ浏览器";
}

wchar_t* QQPlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\QQBrowser",
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

wchar_t* QQPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Tencent\\QQBrowser\\user_data\\0\\Bookmarks";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

wchar_t* QQPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Tencent\\QQBrowser\\user_data\\0\\history.db";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

BOOL QQPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	nDataNum = 0;
	return TRUE;
}

BOOL QQPlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}


#define MAX_BUFFER_LEN	4096

	return TRUE;
}

int32 QQPlugIn::GetFavoriteCount()
{
	return  0;
}