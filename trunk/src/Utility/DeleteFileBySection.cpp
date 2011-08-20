#include <stdafx.h>
#include "DeleteFileBySection.h"
#include <Winioctl.h>

CDeleteFileBySection::CDeleteFileBySection(void)
{
}

CDeleteFileBySection::~CDeleteFileBySection(void)
{
}
//读一个扇区的数据
BOOL CDeleteFileBySection::ReadDisk(TCHAR Desk,LONGLONG nSector,BYTE *pData)
{
	BOOL ret;
	HANDLE hFile;
	TCHAR Path[] = _T("\\\\.\\A:");

	Path[4]=Desk;

	hFile = CreateFile(Path,				
		GENERIC_READ|GENERIC_WRITE,			
		FILE_SHARE_READ|FILE_SHARE_WRITE,	
		NULL,								
		OPEN_EXISTING,						
		0,				
		NULL);								

	DWORD dwLen;
	LARGE_INTEGER Pos;
	Pos.QuadPart = nSector;
	SetFilePointerEx(hFile,Pos,NULL,FILE_BEGIN);
	ret = ReadFile(hFile,pData, 512, &dwLen, NULL);
	CloseHandle(hFile);

	return ret;
}
BOOL CDeleteFileBySection::WriteDisk(TCHAR Desk,LONGLONG nSector,BYTE *pData,DWORD size)
{
	BOOL ret;
	HANDLE hFile;
	TCHAR Path[]=_T("\\\\.\\A:");
	Path[4]=Desk;
	hFile = CreateFile(Path,				
		GENERIC_READ|GENERIC_WRITE,			
		FILE_SHARE_READ|FILE_SHARE_WRITE,	
		NULL,								
		OPEN_EXISTING,						
		FILE_FLAG_NO_BUFFERING,				
		NULL);								
	DWORD dwLen;
	LARGE_INTEGER Pos;
	Pos.QuadPart = nSector;
	SetFilePointerEx(hFile,Pos,NULL,FILE_BEGIN);
	ret=WriteFile(hFile,pData,size, &dwLen, NULL);
	CloseHandle(hFile);
	return ret;
}

BOOL CDeleteFileBySection::GetFileInfo(const TCHAR* lpszFileName,LONGLONG* pllAddress)
{
	HANDLE hFileDevice = CreateFile(lpszFileName,FILE_READ_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
	if(hFileDevice == INVALID_HANDLE_VALUE)
		return FALSE;
	TCHAR InBuffer[MAX_PATH] = {0};
	TCHAR OutBuffer[MAX_PATH] = {0};
	RETRIEVAL_POINTERS_BUFFER *p;
	DWORD dwRetLen;
	BOOL bRet = DeviceIoControl(hFileDevice,FSCTL_GET_RETRIEVAL_POINTERS,InBuffer,sizeof(STARTING_VCN_INPUT_BUFFER),OutBuffer,MAX_PATH,&dwRetLen,NULL);
	p=(RETRIEVAL_POINTERS_BUFFER*)OutBuffer;
	CloseHandle(hFileDevice);
	*pllAddress = p->Extents[0].Lcn.QuadPart;
	return bRet;
}
#define FATINFOLEN 512

BOOL CDeleteFileBySection::ReadDiskInfoFAT(TCHAR Desk,BIOS_PARAMETER_BLOCK* pBPB)
{
	TCHAR Path[] = _T("\\\\.\\A:");
	Path[4] = Desk;
	HANDLE hFileDevice = CreateFile(Path,GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(hFileDevice == INVALID_HANDLE_VALUE)
		return FALSE;

	PBYTE pBuffer = new BYTE[FATINFOLEN];
	DWORD dwLen;

	if(! ReadFile(hFileDevice,pBuffer, FATINFOLEN, &dwLen, NULL))
		return FALSE;
	CloseHandle(hFileDevice);
	PACKED_BIOS_PARAMETER_BLOCK_EX PBPBE;
	memcpy(&PBPBE,pBuffer+0x0B,sizeof(PACKED_BIOS_PARAMETER_BLOCK_EX));
	delete []pBuffer;	

	if(! IsBpbFat32(&PBPBE))	
		return FALSE;
	FatUnpackBios(pBPB,&PBPBE);
	return TRUE;
}
LONGLONG CDeleteFileBySection::GetFATAddress(BIOS_PARAMETER_BLOCK BPB,LONGLONG llAddress)
{
	LONGLONG laregint;
	laregint = llAddress;
	laregint *= BPB.SectorsPerCluster;
	laregint += BPB.Fats * BPB.LargeSectorsPerFat + BPB.ReservedSectors;
	laregint *= BPB.BytesPerSector;
	return laregint;
}
BOOL CDeleteFileBySection::GetDiskInfoNtfs(TCHAR Desk,NTFS_PART_BOOT_SEC* pPBPBE)
{
	TCHAR Path[] = _T("\\\\.\\A:");
	Path[4] = Desk;
	HANDLE hFileDevice = CreateFile(Path, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if(hFileDevice == INVALID_HANDLE_VALUE)
		return FALSE;

	PBYTE pBuffer = new BYTE[FATINFOLEN];
	DWORD dwLen;

	if (!ReadFile(hFileDevice, pBuffer, FATINFOLEN, &dwLen, NULL))
		return FALSE;
	CloseHandle(hFileDevice);
	memcpy(pPBPBE, pBuffer, sizeof(NTFS_PART_BOOT_SEC));
	delete []pBuffer;	

	return TRUE;	
}
LONGLONG CDeleteFileBySection::GetNtfsAddress(NTFS_PART_BOOT_SEC NPBS,LONGLONG llAddress)
{
	LONGLONG laregint;
	laregint = llAddress;
	laregint *= NPBS.bpb.uchSecPerClust;
	laregint *= NPBS.bpb.wBytesPerSec;
	return laregint;
}


BOOL CDeleteFileBySection::DeleteNtfsFile(TCHAR Desk,const TCHAR *FileName)
{
	NTFS_PART_BOOT_SEC NPBS;
	LONGLONG llAddress;
	BYTE pData[512];

	if(!GetDiskInfoNtfs(Desk,&NPBS))
		return false;

	if(memicmp(&NPBS.chOemID,"NTFS",4)!=0)
		return false;
	if(!GetFileInfo(FileName,&llAddress))
		return false;

	LONGLONG LongInt1 = GetNtfsAddress(NPBS,llAddress);
	if(ReadDisk(Desk,LongInt1,pData))
	{
		//if(memcmp(pData,"MZ",2)==0)
		//{
			//memcpy(pData,"\xC3\xC3",2);
			//ZeroMemory(pData,sizeof(pData));

            for (int i= 0; i < 512; ++i)
            {
                pData[i] ^= 0x23;
            }

			WriteDisk(Desk,LongInt1,pData,512);
			return true;
		//}
	}
	return false;
}

BOOL CDeleteFileBySection::DeleteFat32File(TCHAR Desk,const TCHAR *FileName)
{
	BIOS_PARAMETER_BLOCK BPB;
	LONGLONG llAddress;
	BYTE pData[512];

	if(!ReadDiskInfoFAT(Desk,&BPB))
		return false;

	if(!GetFileInfo(FileName,&llAddress))
		return false;

	LONGLONG LongInt = GetFATAddress(BPB,llAddress);
	if(ReadDisk(Desk,LongInt,pData))
	{
		//if(memcmp(pData,"MZ",2)==0)
		//{
			//memcpy(pData,"\xC3\xC3",2);
			//ZeroMemory(pData,sizeof(pData));
            for (int i= 0; i < 512; ++i)
            {
                pData[i] ^= 0x23;
            }
			WriteDisk(Desk,LongInt,pData,512);
			return true;
		//}
	}
	return false;
}

BOOL CDeleteFileBySection::DeleteFile(const TCHAR *FileName)
{
	TCHAR Desk;
	TCHAR FileSystemName[MAX_PATH];
	TCHAR DriverName[]=_T("A:\\");
	DriverName[0]=Desk=FileName[0];
	if(GetVolumeInformation(DriverName,NULL,0,NULL,NULL,NULL,FileSystemName,254))
	{
		if(_tcsicmp(FileSystemName,_T("ntfs"))==0)
			return DeleteNtfsFile(Desk,FileName);
		if(_tcsicmp(FileSystemName,_T("fat32"))==0)
			return DeleteFat32File(Desk,FileName);
	}
	else
	{
		return false;
	}
	return true;
}
