/*--

Copyright (c) 1999-2009 Tencent

Module Name:

Filemon.h

Abstract: 

文件监控描述文件

Author: 

FrontLee  2009-11-25 20:34:03

Revision History:

--*/

#ifndef __FILEMON_H__
#define __FILEMON_H__

#include <WINDOWS.H>    
#include <STDIO.H>    
#include <TIME.H>    
#include "common.h"


//////////////////////////////////////////////////////////////////////////
//
// 上面部分应该放到到共同文件中
//
//////////////////////////////////////////////////////////////////////////

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