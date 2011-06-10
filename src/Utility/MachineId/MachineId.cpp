#include "stdafx.h"
#include <tchar.h>
#include "MachineId.h"
#include <shlwapi.h>

CMachineId::CMachineId()
{
}

CMachineId::~CMachineId()
{

}

bool CMachineId::GetMac( BYTE *lpMac, UINT &nLen )
{
	if (lpMac == NULL || nLen < 6)
	{
		return false;
	}

	TCHAR szKeyName[MAX_PATH];
	DWORD dwKeyNameLen = MAX_PATH;
	HKEY  hKey = NULL;
	TCHAR szDeviceName[MAX_PATH];
	DWORD dwDeviceNameLen = MAX_PATH;


	if (ERROR_SUCCESS == RegOpenKey(HKEY_LOCAL_MACHINE, 
		_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards"),
		&hKey))
	{
		if (ERROR_SUCCESS == SHEnumKeyEx(hKey,
			1,
			szKeyName,
			&dwKeyNameLen))
		{
			TCHAR szSubKeyName[1024] = {0};

			_stprintf_s(szSubKeyName, 1023,
				_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\%s"),
				szKeyName);

			HKEY hSubKey;
			if (ERROR_SUCCESS == RegOpenKey(HKEY_LOCAL_MACHINE, 
				szSubKeyName, &hSubKey))
			{
				DWORD dwKeyType = REG_SZ;
				if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, _T("ServiceName"), NULL, &dwKeyType, (BYTE *)szDeviceName, &dwDeviceNameLen))
				{
					TCHAR szSymbolicDeviceName[MAX_PATH] = {0};
					_stprintf_s(szSymbolicDeviceName, MAX_PATH, _T("\\\\.\\%s"), szDeviceName);
					HANDLE hDevice = CreateFile(szSymbolicDeviceName,
						FILE_ALL_ACCESS,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

					#define OID_802_3_PERMANENT_ADDRESS             0x01010101
					#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
						    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
					)
					#define _NDIS_CONTROL_CODE(request,method) \
		            CTL_CODE(0x17, request, method, 0)

					#define IOCTL_NDIS_QUERY_GLOBAL_STATS   _NDIS_CONTROL_CODE(0, 2)
					if (hDevice)
					{
						ULONG nCmd = OID_802_3_PERMANENT_ADDRESS;
						DWORD dwByteReturned = 0;
						DeviceIoControl(hDevice, 
							IOCTL_NDIS_QUERY_GLOBAL_STATS,
							&nCmd,
							sizeof(ULONG),
							lpMac,
							nLen,
							&dwByteReturned,
							NULL);
						CloseHandle(hDevice);

						return true;
					}

				}
			}
		}
	}

	return false;
}

bool CMachineId::GetCpuId( BYTE *lpCpuId, UINT &nLen )
{
	DWORD    adwCpuid[2] = {0};

	if ((lpCpuId == NULL) || (nLen < sizeof(adwCpuid)))
	{
		return false;
	}

	__try
	{
		__asm
		{
			mov eax, 1
			cpuid
			mov adwCpuid[0], eax
			mov adwCpuid[4], edx
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	memcpy(lpCpuId, adwCpuid, sizeof(adwCpuid));

	return true;
}
