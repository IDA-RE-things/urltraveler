#pragma once

// FavMonitor的对外的头文件信息

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace favmonitor 
{
	// FavMonitor对外公开的事件
	enum E_FavMonitorEventValue
	{
	};

	// FavMonitor能够可能对外发送的广播消息
	enum E_FavMonitorMessageValue
	{
		MESSAGE_VALUE_FAVMONITOR_FILE_CHANGE = MESSAGE_VALUE_FAVMONITOR_BEGIN,
		MESSAGE_VALUE_FAVMONITOR_END = MESSAGE_VALUE_FAVMONITOR_END,
	};

	// FavMonitor能够处理的直接调用
	enum E_FavMonitorServiceValue
	{
		SERVICE_VALUE_FAVMONITOR_ADD_MONITOR = SERVICE_VALUE_FAVMONITOR_BEGIN,	
		SERVICE_VALUE_FAVMONITOR_START_MONITOR,
		SERVICE_VALUE_FAVMONITOR_STOP_MONITOR,
		SERVICE_VALUE_FAVMONITOR_SUSPEND_MONITOR,
		SERVICE_VALUE_FAVMONITOR_END = SERVICE_VALUE_FAVMONITOR_END,		//所有的事件结束
	};

	//=======================================
	//                             Message
	//=======================================
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

	//=======================================
	//                             Service
	//=======================================
	struct FavMonitor_AddMonitorService	: Service 
	{
		FavMonitor_AddMonitorService()
		{
			serviceId	=	SERVICE_VALUE_FAVMONITOR_ADD_MONITOR;
		}

		wchar_t	szFile[MAX_PATH];
	};

	struct FavMonitor_StartMonitorService	: Service 
	{
		FavMonitor_StartMonitorService()
		{
			serviceId	=	SERVICE_VALUE_FAVMONITOR_START_MONITOR;
		}
	};

	struct FavMonitor_StopMonitorService	: Service 
	{
		FavMonitor_StopMonitorService()
		{
			serviceId	=	SERVICE_VALUE_FAVMONITOR_STOP_MONITOR;
		}
	};

	struct FavMonitor_SuspendMonitorService	: Service 
	{
		FavMonitor_SuspendMonitorService()
		{
			serviceId	=	SERVICE_VALUE_FAVMONITOR_SUSPEND_MONITOR;
		}
	};
};