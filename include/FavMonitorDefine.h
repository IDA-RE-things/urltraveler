#pragma once

// FavMonitor�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace favmonitor 
{
	// FavMonitor���⹫�����¼�
	enum E_FavMonitorEventValue
	{
	};

	// FavMonitor�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_FavMonitorMessageValue
	{
		MESSAGE_VALUE_FAVMONITOR_FILE_CHANGE = MESSAGE_VALUE_FAVMONITOR_BEGIN,
		MESSAGE_VALUE_FAVMONITOR_END = MESSAGE_VALUE_FAVMONITOR_END,
	};

	// FavMonitor�ܹ������ֱ�ӵ���
	enum E_FavMonitorServiceValue
	{
		SERVICE_VALUE_FAVMONITOR_ADD_MONITOR = SERVICE_VALUE_FAVMONITOR_BEGIN,	
		SERVICE_VALUE_FAVMONITOR_START_MONITOR,
		SERVICE_VALUE_FAVMONITOR_STOP_MONITOR,
		SERVICE_VALUE_FAVMONITOR_SUSPEND_MONITOR,
		SERVICE_VALUE_FAVMONITOR_END = SERVICE_VALUE_FAVMONITOR_END,		//���е��¼�����
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