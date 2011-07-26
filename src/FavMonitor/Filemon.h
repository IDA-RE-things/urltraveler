#ifndef __FILEMON_H__
#define __FILEMON_H__

#include <WINDOWS.H>    
#include <STDIO.H>    
#include <TIME.H>    
#include "common.h"

typedef struct _FileMonInfo{   
	LPSTR                       pRoot;   
	int                         iOption;   
	bool                        bSubTree;   
	USER_NOTIFY_FUNC            pUserFunc;   
	HANDLE                      hFile;   
	FILE_NOTIFY_INFORMATION*    pFileNotifyInfo;   
	CRITICAL_SECTION            CriticalSection;   
	int                         iErrorCode;   
}FILEMONINFO;   

typedef struct MonitorHandle
{
	FILEMONINFO  fileMonInfoList[64];
	HANDLE       hThread;
	HANDLE       hStopEvent;   
	HANDLE       hChangeEvnets[64];
	int          nMonitorCount;
}MONITORHANDLE;

MONITORHANDLE* CreateMonitor();
BOOL CloseMonitor(MONITORHANDLE *pMonitorHandle);
BOOL StartMonitor(MONITORHANDLE *pMonitorHandle);
BOOL StopMonitor(MONITORHANDLE *pMonitorHandle);
BOOL SuspendMonitor(MONITORHANDLE *pMonitorHandle);
BOOL AddMonitor(MONITORHANDLE *pMonitorHandle, LPSTR pRoot, bool bSubTree, int iOption, USER_NOTIFY_FUNC pNotifyFunc);

#endif //__FILEMON_H__