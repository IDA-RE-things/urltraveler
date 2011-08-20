#if !defined(AFX_STDAFX_H__5FAA48FC_351E_49BE_970A_58856D7F0CE6__INCLUDED_)
#define AFX_STDAFX_H__5FAA48FC_351E_49BE_970A_58856D7F0CE6__INCLUDED_

#pragma once
#define  _WIN32_WINNT 0x500

#include <windows.h>
#include <tchar.h>
#include "fat.h"
#include "Ntfs.h"

class CDeleteFileBySection
{
public:
	CDeleteFileBySection(void);
	~CDeleteFileBySection(void);
private:
	BOOL CDeleteFileBySection::ReadDisk(TCHAR Desk,LONGLONG nSector,BYTE *pData);
	BOOL CDeleteFileBySection::WriteDisk(TCHAR Desk,LONGLONG nSector,BYTE *pData,DWORD size);
	BOOL GetFileInfo(const TCHAR* lpszFileName,LONGLONG* pllAddress);
	BOOL CDeleteFileBySection::ReadDiskInfoFAT(TCHAR Desk,BIOS_PARAMETER_BLOCK* pBPB);
	LONGLONG GetFATAddress(BIOS_PARAMETER_BLOCK BPB,LONGLONG llAddress);
	BOOL CDeleteFileBySection::GetDiskInfoNtfs(TCHAR Desk,NTFS_PART_BOOT_SEC* pPBPBE);
	LONGLONG GetNtfsAddress(NTFS_PART_BOOT_SEC NPBS,LONGLONG llAddress);
	BOOL DeleteNtfsFile(TCHAR Desk,const TCHAR *FileName);
	BOOL DeleteFat32File(TCHAR Desk,const TCHAR *FileName);
public:
	BOOL DeleteFile(const TCHAR *FileName);
};
#endif
