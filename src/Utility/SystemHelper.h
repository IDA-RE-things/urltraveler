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

	///ö������:����ϵͳ�汾��
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

	/// ���windows����ϵͳ�İ汾��Ϣ
	static BOOL GetOsVersion(OUT DWORD& dwMajorVer, IN OUT DWORD* pdwMinorVer = NULL,
		IN OUT DWORD* pdwBuildNum = NULL, IN OUT DWORD* pdwPlatformId = NULL,
		IN OUT LPTSTR szCSDVersion = NULL, IN OUT int nMaxLen = 0);

	/// ��Windows2000���Ժ�Ĳ���ϵͳ��
	static BOOL IsWindows2000OrLater();

	/// ��Windows2000����ϵͳ��
	static BOOL IsWindows2000();

	/// ��WindowsXP����ϵͳ��
	static BOOL IsWindowsXP();

	/// ��WindowsVistaϵͳ��
	static BOOL IsWindowsVista();

	/// ���windows����ϵͳ��ServicePack��Ϣ
	static wstring GetOsServicePackInfo();

	/// �ж�һ�����Ƿ���
	static BOOL IsKeyDown(int nVirtKey);
};