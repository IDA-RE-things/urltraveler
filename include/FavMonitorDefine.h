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
		EVENT_VALUE_FAVMONITOR_START_MONITOR	=	EVENT_VALUE_FAVMONITOR_BEGIN,	//	�������
		EVENT_VALUE_FAVMONITOR_STOP_MONITOR,	//	ֹͣ���
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
		SERVICE_VALUE_FAVMONITOR_REMOVE_MONITOR,

		SERVICE_VALUE_FAVMONITOR_END = SERVICE_VALUE_FAVMONITOR_END,		//���е��¼�����
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
		MONITORHANDLE hMonitorHandle; //���ؼ�ؾ��
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