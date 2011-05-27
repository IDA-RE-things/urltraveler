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

int32 C360SE2PlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* C360SE2PlugIn::GetBrowserName()
{
	return L"360SE2.0";
}

wchar_t* C360SE2PlugIn::GetInstallPath()
{
	// 360SE 2.0和360SE3.0的收藏夹方式不相同
	// 360 2.0使用和IE一致的收藏夹。360 3.0单独的数据库进行存储

	// 2.0  
	// {66D8959E-B7E9-4cd4-BC16-98711D815F2A}
	// DisplayIcon	C:\Program Files\360\360se\360SE.exe

	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{66D8959E-B7E9-4cd4-BC16-98711D815F2A}",
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

wchar_t* C360SE2PlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetHomeDir() + L"\\Favorites";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏
	return wcsdup(strPath.c_str());
}

wchar_t* C360SE2PlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetHomeDir() + L"\\AppData\\Local\\Microsoft\\Windows\\History";
	return wcsdup(strPath.c_str());
}

BOOL C360SE2PlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	// 由于使用IE收藏夹，因此不需要额外的导入导出，
	return TRUE;
}

BOOL C360SE2PlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	// 由于使用IE收藏夹，因此不需要额外的导入导出，
	return TRUE;
}

int32 C360SE2PlugIn::GetFavoriteCount()
{
	// 由于使用IE收藏夹，因此不需要额外的导入导出，
	return 0;
}