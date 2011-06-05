//////////////////////////////////////////////////////////////////////////
// SystemHelper.cpp

#include "stdafx.h"
#include "SystemHelper.h"
#include "tchar.h"

SystemHelper::SystemHelper()
{

}

SystemHelper::~SystemHelper()
{

}


BOOL SystemHelper::GetOsVersion(OUT DWORD& dwMajorVer, IN OUT DWORD* pdwMinorVer/* = NULL*/,
				  IN OUT DWORD* pdwBuildNum/* = NULL*/, IN OUT DWORD* pdwPlatformId/* = NULL*/,
				  IN OUT LPTSTR szCSDVersion/* = NULL*/, IN OUT int nMaxLen/* = 0*/)
{
	dwMajorVer = 0;

	BOOL bRet = FALSE;

	OSVERSIONINFO osInfo;
	osInfo.dwOSVersionInfoSize = sizeof(osInfo);
	bRet = GetVersionEx(&osInfo);

	if (bRet)
	{
		dwMajorVer = osInfo.dwMajorVersion;
		if (pdwMinorVer)	*pdwMinorVer	= osInfo.dwMinorVersion;
		if (pdwBuildNum)	*pdwBuildNum	= osInfo.dwBuildNumber;
		if (pdwPlatformId)	*pdwPlatformId	= osInfo.dwPlatformId;
		if (szCSDVersion)	lstrcpyn(szCSDVersion, osInfo.szCSDVersion, nMaxLen);
	}
	else
	{
		ASSERT(FALSE);
	}

	return bRet;
}

SystemHelper::E_OperationVersion SystemHelper::GetOsVersion()
{
	const int iBufLen = 80;
	E_OperationVersion eOperationVersion = e_UnKnown;
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		{
			return eOperationVersion;
		}
	}

	switch (osvi.dwPlatformId)
	{
		// Test for the Windows NT product family.
	case VER_PLATFORM_WIN32_NT:

		if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
		{
			if( osvi.wProductType == VER_NT_WORKSTATION )
			{
				eOperationVersion = e_WinVista;
				///StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
			}
			else 
			{
				eOperationVersion = e_Win2008;
				///StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
			}
		}


		// Test for the specific product.
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			eOperationVersion = e_Win2003;
			printf ("Microsoft Windows Server 2003, ");
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			eOperationVersion = e_WinXP;
			///printf ("Microsoft Windows XP ");
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			eOperationVersion = e_Win2000;
			printf ("Microsoft Windows 2000 ");
		}

		if ( osvi.dwMajorVersion <= 4 )
		{
			eOperationVersion = e_WinNT;
			printf("Microsoft Windows NT ");
		}

		// Test for specific product on Windows NT 4.0 SP6 and later.
		if( bOsVersionInfoEx )
		{
			// Test for the workstation type.
			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				if( osvi.dwMajorVersion == 4 )
				{
					///printf ( "Workstation 4.0 " );
				}
				else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				{
					///printf ( "Home Edition " );
				}
				else
				{
					///printf ( "Professional " );
				}
			}

			// Test for the server type.
			else if ( osvi.wProductType == VER_NT_SERVER || 
				osvi.wProductType == VER_NT_DOMAIN_CONTROLLER )
			{
				if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2)
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						printf ( "Datacenter Edition " );
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						printf ( "Enterprise Edition " );
					else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
						printf ( "Web Edition " );
					else printf ( "Standard Edition " );
				}
				else if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0)
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						printf ( "Datacenter Server " );
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						printf ( "Advanced Server " );
					else printf ( "Server " );
				}
				else  // Windows NT 4.0 
				{
					if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						printf ("Server 4.0, Enterprise Edition " );
					else printf ( "Server 4.0 " );
				}
			}
		}
		// Test for specific product on Windows NT 4.0 SP5 and earlier
		else  
		{
			HKEY hKey;
			TCHAR szProductType[iBufLen];
			DWORD dwBufLen = iBufLen;
			LONG lRet;

			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
				0, KEY_QUERY_VALUE, &hKey );
			if( lRet != ERROR_SUCCESS )
			{
				return eOperationVersion;
			}

			lRet = RegQueryValueEx( hKey, _T("ProductType"), NULL, NULL,
				(LPBYTE) szProductType, &dwBufLen);
			if( (lRet != ERROR_SUCCESS) || (dwBufLen > iBufLen) )
			{
				return eOperationVersion;
			}

			RegCloseKey( hKey );

			if ( lstrcmpi( _T("WINNT"), szProductType) == 0 )
				printf( "Workstation " );
			if ( lstrcmpi( _T("LANMANNT"), szProductType) == 0 )
				printf( "Server " );
			if ( lstrcmpi( _T("SERVERNT"), szProductType) == 0 )
				printf( "Advanced Server " );
			printf( "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
		}

		// Display service pack (if any) and build number.

		if( osvi.dwMajorVersion == 4 && 
			lstrcmpi( osvi.szCSDVersion, _T("Service Pack 6") ) == 0 )
		{ 
			HKEY hKey;
			LONG lRet;

			// Test for SP6 versus SP6a.
			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
				0, KEY_QUERY_VALUE, &hKey );
			if( lRet == ERROR_SUCCESS )
				printf( "Service Pack 6a (Build %d)\n", 
				osvi.dwBuildNumber & 0xFFFF );         
			else // Windows NT 4.0 prior to SP6a
			{
				/*printf( "%s (Build %d)\n",
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
				*/
			}

			RegCloseKey( hKey );
		}
		else // not Windows NT 4.0 
		{
			/*
			printf( "%s (Build %d)\n",
			osvi.szCSDVersion,
			osvi.dwBuildNumber & 0xFFFF);
			*/
		}

		break;

		// Test for the Windows Me/98/95.
	case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			eOperationVersion = e_Win95;
			printf ("Microsoft Windows 95 ");
			if (osvi.szCSDVersion[1]=='C' || osvi.szCSDVersion[1]=='B')
			{
				///printf("OSR2 " );
			}
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			eOperationVersion = e_Win98;
			///printf ("Microsoft Windows 98 ");
			if ( osvi.szCSDVersion[1] == 'A' )
			{
				///printf("SE " );
			}
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			eOperationVersion = e_WinME;
			///printf ("Microsoft Windows Millennium Edition\n");
		} 
		break;

	case VER_PLATFORM_WIN32s:
		{
			eOperationVersion = e_Win32;
			///printf ("Microsoft Win32s\n");
		}		
		break;
	}
	return eOperationVersion; 
}

BOOL SystemHelper::IsWindows2000OrLater()
{
	BOOL bRet = FALSE;

	DWORD dwMajorVersion = 0;
	bRet = GetOsVersion(dwMajorVersion) && (dwMajorVersion >= 5);

	return bRet;
}

BOOL SystemHelper::IsWindows2000()
{
	BOOL bRet = FALSE;

	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;

	bRet = GetOsVersion(dwMajorVersion, &dwMinorVersion) && 
		(dwMajorVersion == 5) && (dwMinorVersion == 0);

	return bRet;
}

BOOL SystemHelper::IsWindowsXP()
{
	BOOL bRet = FALSE;

	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;

	bRet = GetOsVersion(dwMajorVersion, &dwMinorVersion) && 
		(dwMajorVersion == 5) && (dwMinorVersion == 1);

	return bRet;
}

BOOL SystemHelper::IsWindowsVista()
{
	BOOL bRet = FALSE;

	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;

	bRet = GetOsVersion(dwMajorVersion, &dwMinorVersion) && 
		(dwMajorVersion == 6) && (dwMinorVersion == 0);

	return bRet;

}

wstring SystemHelper::GetOsServicePackInfo()
{
	wchar_t szVersion[MAX_PATH];

	DWORD dwMajorVersion = 0;
	GetOsVersion(dwMajorVersion, NULL, NULL, NULL, szVersion, MAX_PATH);
	return szVersion;
}

BOOL SystemHelper::IsKeyDown(int nVirtKey)
{
	return GetKeyState(nVirtKey) < 0;
}