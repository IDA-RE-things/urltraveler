#pragma once

// DataCenter的对外的头文件信息

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace favmonitor 
{
	// DataCenter对外公开的事件
	enum E_FavMonitorEventValue
	{
	};

	// DataCenter能够可能对外发送的广播消息
	enum E_FavMonitorMessageValue
	{
		MESSAGE_VALUE_FAVMONITOR_FILE_CHANGE = MESSAGE_VALUE_FAVMONITOR_BEGIN,
		MESSAGE_VALUE_FAVMONITOR_END = MESSAGE_VALUE_FAVMONITOR_END,
	};

	// DataCenter能够处理的直接调用
	enum E_FavMonitorServiceValue
	{
		SERVICE_VALUE_FAVMONITOR_ADD_MONITOR = SERVICE_VALUE_FAVMONITOR_BEGIN,	
		SERVICE_VALUE_FAVMONITOR_REMOVE_MONITOR,
		SERVICE_VALUE_FAVMONITOR_END = SERVICE_VALUE_FAVMONITOR_END,		//所有的事件结束
	};

	struct FavMonitor_FileChangeMessage : MessageExtraInfo
	{
		FavMonitor_FileChangeMessage()
		{
			messageValue = MESSAGE_VALUE_FAVMONITOR_FILE_CHANGE;
			srcMId = MODULE_ID_FAVMONITOR; 

			memset(szFile, 0, sizeof(szFile));
			iActionType = 0;
		}


		wchar_t szFile[MAX_PATH];
		int iActionType;
	};


	typedef PVOID MONITORHANDLE;

	struct FavMonitor_AddMonitorService	: Service 
	{
		FavMonitor_AddMonitorService()
		{
			serviceId	=	SERVICE_VALUE_FAVMONITOR_ADD_MONITOR;
		}

		wchar_t	szFile[MAX_PATH];
		MONITORHANDLE hMonitorHandle; //返回监控句柄
	};

	struct FavMonitor_RemoveMonitorService	: Service 
	{
		FavMonitor_RemoveMonitorService()
		{
			serviceId	=	SERVICE_VALUE_FAVMONITOR_REMOVE_MONITOR;
		}

		MONITORHANDLE hMonitorHandle;
	};
};