#include "stdafx.h"
#include "Filemon.h"



void ThreadMonfileChangeEvent( FILEMONINFO* pInfo)   
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
	}
};   

#ifndef MAX_CHANGESTREAMLENGTH    
#define MAX_CHANGESTREAMLENGTH 0x20000    
#endif    

#define NOTIFYSTRUCTBUFLENGTH sizeof(int)*3 + MAX_CHANGESTREAMLENGTH    

void ThreadMonfileMain(MONITORHANDLE* pMonitorHandle)   
{   
	HANDLE hEventList[2];   

	OVERLAPPED Overlapped;   
	bool bReadDirChange = false;

	_ReadDirectoryChangesW ReadDirectoryChangesW;   
	ReadDirectoryChangesW = (_ReadDirectoryChangesW) GetProcAddress(GetModuleHandleA("kernel32"),"ReadDirectoryChangesW");   

	do   
	{   
		__try   
		{   
			for (int i = 0; i < pMonitorHandle->nMonitorCount; i++)
			{
				FILEMONINFO *pInfo = &pMonitorHandle->fileMonInfoList[i];

				memset( &Overlapped, 0, sizeof(OVERLAPPED) );   

				pInfo->pFileNotifyInfo = (FILE_NOTIFY_INFORMATION*) new BYTE[ NOTIFYSTRUCTBUFLENGTH ];   

				if( pInfo->pFileNotifyInfo == NULL)   
				{   
					Monfile_err_log("alloc memory error");   
					break;   
				} 

				memset( pInfo->pFileNotifyInfo , 0 ,NOTIFYSTRUCTBUFLENGTH ); 

				Overlapped.hEvent = pMonitorHandle->hChangeEvnets[i];   

				bool bReadDirChange =  ReadDirectoryChangesW(pInfo->hFile, 
					pInfo->pFileNotifyInfo, MAX_CHANGESTREAMLENGTH, pInfo->bSubTree, 
					pInfo->iOption, 0, &Overlapped, 0);
			}
			// ���ü��Ŀ¼�����������ȡ��Ƿ������Ŀ¼������¼�
			
			if(!bReadDirChange)
			{
				break;   // ���ò��ɹ�
			}

			int iEventIndex = WaitForMultipleObjects (pMonitorHandle->nMonitorCount, 
				(const HANDLE*)&pMonitorHandle->hChangeEvnets,
				0,
				-1); 

			// �ļ��仯�¼�
			if( iEventIndex == 0 )   
			{   
				//FILEMONINFO* pNewInfo;   
				//pNewInfo = new FILEMONINFO;   
				//memcpy( pNewInfo , pInfo, sizeof(FILEMONINFO) );   

				//CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ThreadMonfileChangeEvent,pNewInfo,NULL,NULL);   
			}

			// ֹͣ����¼�
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

	SetEvent(pMonitorHandle->hStopEvent);   
} 
/*
void MonFile_Stop(PVOID pstFileMonInfo)   
{   
	FILEMONINFO* pInfo = (FILEMONINFO*) pstFileMonInfo;   
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
} */  

MONITORHANDLE* CreateMonitor()
{
	MONITORHANDLE *pMonitorHandle = new MONITORHANDLE;

	pMonitorHandle->hThread = CreateThread(NULL, 
		NULL,
		(LPTHREAD_START_ROUTINE)ThreadMonfileMain,
		pMonitorHandle,
		CREATE_SUSPENDED,
		NULL);  
	pMonitorHandle->hStopEvent = CreateEvent(0, TRUE, FALSE, 0);
	pMonitorHandle->nMonitorCount = 0;
	memset(pMonitorHandle->hChangeEvnets, 0, sizeof(pMonitorHandle->hChangeEvnets));

	return pMonitorHandle;
}

BOOL CloseMonitor(MONITORHANDLE *pMonitorHandle)
{
	if (pMonitorHandle != NULL)
	{
		CloseHandle(pMonitorHandle->hThread);
		CloseHandle(pMonitorHandle->hStopEvent);
		delete pMonitorHandle;

		return TRUE;
	}

	return FALSE;
}

BOOL StartMonitor(MONITORHANDLE *pMonitorHandle)
{
	if (pMonitorHandle != NULL)
	{
		::ResumeThread(pMonitorHandle->hThread);

		return TRUE;
	}

	return FALSE;
}

BOOL StopMonitor(MONITORHANDLE *pMonitorHandle)
{
	if (pMonitorHandle != NULL)
	{
		SetEvent(pMonitorHandle->hStopEvent );   
		WaitForSingleObject(pMonitorHandle->hStopEvent, -1);  

		return TRUE;
	}

	return FALSE;
}

BOOL SuspendMonitor(MONITORHANDLE *pMonitorHandle)
{
	if (pMonitorHandle != NULL)
	{
		::SuspendThread(pMonitorHandle->hThread);

		return TRUE;
	}

	return FALSE;
}

BOOL AddMonitor(MONITORHANDLE *pMonitorHandle, LPSTR pRoot, bool bSubTree, int iOption, USER_NOTIFY_FUNC pNotifyFunc)   
{   
	FILEMONINFO *pInfo = NULL;

	if (pMonitorHandle == NULL && pMonitorHandle->nMonitorCount < 64)
	{
		return FALSE;
	}

	pInfo = &pMonitorHandle->fileMonInfoList[pMonitorHandle->nMonitorCount];

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

	pInfo->pRoot = new char[strlen(pRoot) + 1];   
	lstrcpyA(pInfo->pRoot,pRoot);   
	pInfo->bSubTree = bSubTree;   
	pInfo->iOption = iOption;   
	pInfo->pUserFunc = pNotifyFunc;   

	pMonitorHandle->hChangeEvnets[pMonitorHandle->nMonitorCount] = CreateEvent(0, TRUE, TRUE, 0);   
	pMonitorHandle->nMonitorCount++;

	InitializeCriticalSection(&pInfo->CriticalSection);   

	return TRUE;   
}   



