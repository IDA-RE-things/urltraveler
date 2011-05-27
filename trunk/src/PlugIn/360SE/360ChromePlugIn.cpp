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
#include "PathHelper.h"

#include "360ChromePlugIn.h"
#include "360SEPlugInFactory.h"


using namespace n360seplugin;

#pragma comment(lib, "shlwapi.lib")


C360ChromePlugIn::C360ChromePlugIn()
{
}


C360ChromePlugIn::~C360ChromePlugIn()
{
	
}

BOOL C360ChromePlugIn::Load()
{
	return TRUE;
}

BOOL C360ChromePlugIn::UnLoad()
{
	return TRUE;
}

int32 C360ChromePlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* C360ChromePlugIn::GetBrowserName()
{
	return L"360SE2.0";
}

wchar_t* C360ChromePlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\360chrome.exe",
		L"Path", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		swprintf_s(szPath,MAX_PATH-1, L"%s%s", szPath, L"\\360chrome.exe"); 
		return _wcsdup(szPath);
	}

	return NULL;
}

wchar_t* C360ChromePlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetLocalAppDataDir() + L"\\360Chrome\Chrome\User Data\Default\\Bookmarks";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

wchar_t* C360ChromePlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetLocalAppDataDir() + L"\\360Chrome\Chrome\User Data\Default\\History";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return _wcsdup(strPath.c_str());
}

BOOL C360ChromePlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	return TRUE;
}

BOOL C360ChromePlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	return TRUE;
}

int32 C360ChromePlugIn::GetFavoriteCount()
{
	return 0;
}