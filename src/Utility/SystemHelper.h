#pragma once

#include "SndaBase.h"
#include <string>

using namespace std;

class UTILITY_API SystemHelper
{
private:
	SystemHelper();
	~SystemHelper();

public:

	///枚举类型:操作系统版本号
	enum E_OperationVersion
	{
		///Unknown Operation System
		e_UnKnown = -1,
		///Win32
		e_Win32 = 0,
		///Win95
		e_Win95,
		///Win98
		e_Win98,
		///WinME
		e_WinME,
		///WinNT
		e_WinNT,
		///Win2000
		e_Win2000,
		///WinXP
		e_WinXP,
		///Win2003
		e_Win2003,
		///Win Vista
		e_WinVista,
		///Win2008
		e_Win2008
	};

	static E_OperationVersion GetOsVersion();

	/// 获得windows操作系统的版本信息
	static BOOL GetOsVersion(OUT DWORD& dwMajorVer, IN OUT DWORD* pdwMinorVer = NULL,
		IN OUT DWORD* pdwBuildNum = NULL, IN OUT DWORD* pdwPlatformId = NULL,
		IN OUT LPTSTR szCSDVersion = NULL, IN OUT int nMaxLen = 0);

	/// 是Windows2000及以后的操作系统吗？
	static BOOL IsWindows2000OrLater();

	/// 是Windows2000操作系统吗？
	static BOOL IsWindows2000();

	/// 是WindowsXP操作系统吗？
	static BOOL IsWindowsXP();

	/// 是WindowsVista系统吗？
	static BOOL IsWindowsVista();

	/// 获得windows操作系统的ServicePack信息
	static wstring GetOsServicePackInfo();

	/// 判断一个键是否按下
	static BOOL IsKeyDown(int nVirtKey);
};