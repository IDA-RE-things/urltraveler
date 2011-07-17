/*--

Copyright (c) 1999-2009 Tencent

Module Name:

Filemon.cpp

Abstract: 



Author: 

FrontLee  2009-11-25 20:37:41

Revision History:

--*/


#include "stdafx.h"
#include "Filemon.h"




/*++

Function Name: ThreadMonfileChangeEvent
Abstract: 
ReturnType: void 
Parameters:
[in/out] - stFileMonInfo* pInfo	--> 
Remarks:

*/
void ThreadMonfileChangeEvent( stFileMonInfo* pInfo)   
{   
	__try   
	{   
		FILE_NOTIFY_INFORMATION* pstFileNotiInfo = pInfo->pFileNotifyInfo;   
		size_t len_root = strlen(pInfo->pRoot); 

		do   
		{   
			LPWSTR pWStr = (LPWSTR)&pstFileNotiInfo->FileName[0];   
			size_t buf_len = len_root + pstFileNotiInfo->FileNameLength;   

			LPSTR pChangePath = new char[ buf_len ];   
			memset( pChangePath, 0, buf_len );   
			lstrcpyA( pChangePath, pInfo->pRoot );   
			UnicodeToAnsi_Ptr(pWStr, pstFileNotiInfo->FileNameLength /2,
				pChangePath + len_root );   

			if( pInfo->pUserFunc )   
			{   
				EnterCriticalSection( &pInfo->CriticalSection );   
				pInfo->pUserFunc( pChangePath,pstFileNotiInfo->Action );   
				LeaveCriticalSection( &pInfo->CriticalSection );   
			} 

			delete pChangePath;   

			if( pstFileNotiInfo->NextEntryOffset == 0 ) 
			{
				break;   
			}
			pstFileNotiInfo = (FILE_NOTIFY_INFORMATION*) (((PBYTE)pstFileNotiInfo) + pstFileNotiInfo->NextEntryOffset);   
		}while(1);   

		delete pInfo->pFileNotifyInfo;   
		delete pInfo;   
	}   
	__except(EXCEPTION_EXECUTE_HANDLER)   
	{   
		Monfile_err_log(" ====== error of event thread ======");   
	}
	// 	Monfile_err_log("\n === break of event thread ===");   
};   

#ifndef MAX_CHANGESTREAMLENGTH    
#define MAX_CHANGESTREAMLENGTH 0x20000    
#endif    

#define NOTIFYSTRUCTBUFLENGTH sizeof(int)*3 + MAX_CHANGESTREAMLENGTH    


/*++

Function Name: ThreadMonfileMain
Abstract: 
ReturnType: void 
Parameters:
[in/out] - stFileMonInfo* pInfo	--> 
Remarks:

*/
void ThreadMonfileMain( stFileMonInfo* pInfo)   
{   
	HANDLE hEventList[2];   
	hEventList[0] = pInfo->hChangeEvent;   
	hEventList[1] = pInfo->hStopEvent;   

	OVERLAPPED Overlapped;   

	_ReadDirectoryChangesW ReadDirectoryChangesW;   
	ReadDirectoryChangesW = (_ReadDirectoryChangesW) GetProcAddress(GetModuleHandleA("kernel32"),"ReadDirectoryChangesW");   

	do   
	{   
		__try   
		{   
			memset( &Overlapped, 0, sizeof(OVERLAPPED) );   
			Overlapped.hEvent = pInfo->hChangeEvent;   

			pInfo->pFileNotifyInfo = (FILE_NOTIFY_INFORMATION*) new BYTE[ NOTIFYSTRUCTBUFLENGTH ];   

			if( pInfo->pFileNotifyInfo == NULL)   
			{   
				Monfile_err_log("alloc memory error");   
				break;   
			} 

			memset( pInfo->pFileNotifyInfo , 0 ,NOTIFYSTRUCTBUFLENGTH );   

			// 设置监控目录、缓冲区长度、是否包含子目录及监控事件
			bool bReadDirChange =  ReadDirectoryChangesW(pInfo->hFile, 
				pInfo->pFileNotifyInfo, MAX_CHANGESTREAMLENGTH, pInfo->bSubTree, 
				pInfo->iOption, 0, &Overlapped, 0);

			if(!bReadDirChange)
			{
				break;   // 设置不成功
			}

			int iEventIndex = WaitForMultipleObjects ( 2,(const HANDLE*)&hEventList, 0, -1 );   

			// 文件变化事件
			if( iEventIndex == 0 )   
			{   
				stFileMonInfo* pNewInfo;   
				pNewInfo = new stFileMonInfo;   
				memcpy( pNewInfo , pInfo, sizeof(stFileMonInfo) );   

				CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ThreadMonfileChangeEvent,pNewInfo,NULL,NULL);   
			}

			// 停止监控事件
			if (iEventIndex == 1 )   
			{   
				break;
			}   

		}   
		__except(EXCEPTION_EXECUTE_HANDLER)   
		{   
			Monfile_err_log("\n === error of main thread ===");   
		}   
	}while(true); 

	Monfile_err_log("\n === break of main thread ===");   

	SetEvent( pInfo->hStopEvent);   
} 


/*++

Function Name: MonFile_Stop
Abstract: 
ReturnType: void 
Parameters:
[in/out] - PVOID pstFileMonInfo	--> 
Remarks:

*/
void MonFile_Stop(PVOID pstFileMonInfo)   
{   
	stFileMonInfo* pInfo = (stFileMonInfo*) pstFileMonInfo;   
	if(pInfo == NULL)   
		return;   
	SetEvent ( pInfo->hStopEvent );   
	WaitForSingleObject ( pInfo->hStopEvent, -1 );   
	CloseHandle ( pInfo->hStopEvent );   
	CloseHandle ( pInfo->hChangeEvent );   
	DeleteCriticalSection( &pInfo->CriticalSection ); 

	if (pInfo->pRoot != NULL)
	{
		delete pInfo->pRoot;
		pInfo->pRoot = NULL;
	}

	if (pInfo != NULL)
	{
		delete pInfo;
		pInfo = NULL;
	}
}   


/*++

Function Name: MonFile_Start
Abstract: 
ReturnType: PVOID 
Parameters:
[in/out] - LPSTR pRoot	--> 
[in/out] - bool bSubTree	--> 
[in/out] - int iOption	--> 
[in/out] - USER_NOTIFY_FUNC pNotifyFunc	--> 
Remarks:

*/
PVOID MonFile_Start(LPSTR pRoot,bool bSubTree,int iOption,USER_NOTIFY_FUNC pNotifyFunc)   
{   
	stFileMonInfo* pInfo = new stFileMonInfo;   
	memset(pInfo,0,sizeof(stFileMonInfo));   

	if(iOption == 0)   
	{   
		iOption =   
			FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|    
			FILE_NOTIFY_CHANGE_ATTRIBUTES| FILE_NOTIFY_CHANGE_SIZE|   
			FILE_NOTIFY_CHANGE_LAST_WRITE| FILE_NOTIFY_CHANGE_LAST_ACCESS|   
			FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_SECURITY;   
	}

	pInfo->hFile =    
		CreateFileA (pRoot,    
		GENERIC_READ|GENERIC_WRITE,   
		FILE_SHARE_READ|FILE_SHARE_WRITE| FILE_SHARE_DELETE,   
		NULL,   
		OPEN_EXISTING,   
		FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,    
		NULL);   

	pInfo->pRoot = new char[ strlen(pRoot)+1 ];   
	lstrcpyA(pInfo->pRoot,pRoot);   
	pInfo->bSubTree = bSubTree;   
	pInfo->iOption = iOption;   
	pInfo->pUserFunc = pNotifyFunc;   

	pInfo->hStopEvent = CreateEvent (0, true, false, 0); 
	pInfo->hChangeEvent = CreateEvent (0, true, true, 0);   

	InitializeCriticalSection( &pInfo->CriticalSection );   

	HANDLE hThread = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ThreadMonfileMain,pInfo,NULL,NULL);   
	CloseHandle( hThread );   

	return (PVOID)pInfo;   
}   



