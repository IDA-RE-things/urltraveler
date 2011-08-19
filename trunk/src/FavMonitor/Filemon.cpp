#include "stdafx.h"
#include "Filemon.h"

void NotifyChange( FILEMONINFO* pInfo)   
{   
	__try   
	{   
		FILE_NOTIFY_INFORMATION* pstFileNotiInfo = &pInfo->fileNotifyInfo;   
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

			if( pInfo->pUserFunc)   
			{   
				EnterCriticalSection( &pInfo->CriticalSection );   
				pInfo->pUserFunc( pChangePath,pstFileNotiInfo->Action );   
				LeaveCriticalSection( &pInfo->CriticalSection );   
			} 

			delete pChangePath;   

			if( pstFileNotiInfo->NextEntryOffset == 0) 
			{
				break;   
			}
			pstFileNotiInfo = (FILE_NOTIFY_INFORMATION*) (((PBYTE)pstFileNotiInfo) + pstFileNotiInfo->NextEntryOffset);   
		}while(1);   
	}   
	__except(EXCEPTION_EXECUTE_HANDLER)   
	{   
	}
};   


void MonitorRotuine(MONITORHANDLE* pMonitorHandle)   
{   
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

				if (pInfo == NULL)
				{
					continue;
				}

				memset(&Overlapped, 0, sizeof(OVERLAPPED));   

				memset(&pInfo->fileNotifyInfo, 0 ,NOTIFYSTRUCTBUFLENGTH); 

				Overlapped.hEvent = pMonitorHandle->hChangeEvnets[i];   

				bReadDirChange =  ReadDirectoryChangesW(pInfo->hFile, 
					&pInfo->fileNotifyInfo, MAX_CHANGESTREAMLENGTH, pInfo->bSubTree, 
					pInfo->iOption, 0, &Overlapped, 0);

				if(!bReadDirChange)
				{
					break;   // 设置不成功
				}
			}

			int iEventIndex = WaitForMultipleObjects (pMonitorHandle->nMonitorCount, 
				(const HANDLE*)&pMonitorHandle->hChangeEvnets,
				0,
				-1); 

			if (iEventIndex == 0)
			{
				break;
			}

			// 文件变化事件
			if (iEventIndex < pMonitorHandle->nMonitorCount)
			{
				NotifyChange(&pMonitorHandle->fileMonInfoList[iEventIndex]);
			}
		}   
		__except(EXCEPTION_EXECUTE_HANDLER)   
		{   
		}   
	}while(true); 

	SetEvent(pMonitorHandle->hChangeEvnets[0]);   
} 

MONITORHANDLE* CreateMonitor()
{
	MONITORHANDLE *pMonitorHandle = new MONITORHANDLE;

	pMonitorHandle->hThread = CreateThread(NULL, 
		NULL,
		(LPTHREAD_START_ROUTINE)MonitorRotuine,
		pMonitorHandle,
		CREATE_SUSPENDED,
		NULL);  
	memset(pMonitorHandle->hChangeEvnets, 0, sizeof(pMonitorHandle->hChangeEvnets));

	pMonitorHandle->nMonitorCount = 1;
	pMonitorHandle->hChangeEvnets[0] = CreateEvent(0, TRUE, FALSE, 0);

	return pMonitorHandle;
}

BOOL CloseMonitor(MONITORHANDLE *pMonitorHandle)
{
	if (pMonitorHandle != NULL)
	{
		CloseHandle(pMonitorHandle->hThread);
		CloseHandle(pMonitorHandle->hChangeEvnets[0]);
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
		SetEvent(pMonitorHandle->hChangeEvnets[0] );   
		WaitForSingleObject(pMonitorHandle->hChangeEvnets[0], -1);  

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



