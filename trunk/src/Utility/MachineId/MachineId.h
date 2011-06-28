#pragma once
#include <Windows.h>

class CMachineId
{
public:
	CMachineId();
	~CMachineId();
	bool GetMac(BYTE *lpMac, UINT &nLen);
	bool GetCpuId(BYTE *lpCpuId, UINT &nLen);
};
