#ifndef __FILEMON_H__
#define __FILEMON_H__

#include <WINDOWS.H>    
#include <STDIO.H>    
#include <TIME.H>    
#include "common.h"

#ifndef MAX_CHANGESTREAMLENGTH    
#define MAX_CHANGESTREAMLENGTH 0x20000    
#endif    

#define NOTIFYSTRUCTBUFLENGTH (sizeof(int)*3 + MAX_CHANGESTREAMLENGTH)

typedef struct _FileMonInfo{   
	LPSTR                       pRoot;   
	int                         iOption;   
	bool                        bSubTree;   
	USER_NOTIFY_FUNC            pUserFunc;   
	HANDLE                      hFile;   
	union
	{
		FILE_NOTIFY_INFORMATION    fileNotifyInfo;
		unsigned char szBuffer[NOTIFYSTRUCTBUFLENGTH];
	};
	CRITICAL_SECTION            CriticalSection;   
	int                         iErrorCode;   
}FILEMONINFO;   

typedef struct MonitorHandle
{
	FILEMONINFO  fileMonInfoList[65];
	HANDLE       hThread;
	HANDLE       hChangeEvnets[65];//第0个event非常特殊，是退出线程事件
	int          nMonitorCount;
}MONITORHANDLE;

MONITORHANDLE* CreateMonitor();
BOOL CloseMonitor(MONITORHANDLE *pMonitorHandle);
BOOL StartMonitor(MONITORHANDLE *pMonitorHandle);
BOOL StopMonitor(MONITORHANDLE *pMonitorHandle);
BOOL SuspendMonitor(MONITORHANDLE *pMonitorHandle);
BOOL AddMonitor(MONITORHANDLE *pMonitorHandle, LPSTR pRoot, bool bSubTree, int iOption, USER_NOTIFY_FUNC pNotifyFunc);

#endif //__FILEMON_H__