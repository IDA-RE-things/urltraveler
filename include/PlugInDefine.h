#pragma once

// PlugIn�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "PlugIn.h"


namespace plugin
{
	// PlugIn���⹫�����¼�
	enum E_PlugInEventValue
	{
		EVENT_VALUE_PLUGIN_OPEN = EVENT_VALUE_PLUGIN_BEGIN,		//	��������

		EVENT_VALUE_PLUGIN_LOAD_ALL,													//	���ز��
		EVENT_VALUE_PLUGIN_CHECK_IS_WORKED,									//	��鵱ǰ�Ĳ���Ƿ���Ҫ���������û�а�װ��Ӧ�����������ǰ�������Ҫ��װ
		EVENT_VALUE_PLUGIN_COMBINE_FAVORITE,									//	֪ͨģ����Ҫ��ȡ�ϲ�����ղؼ�
		EVENT_VALUE_PLUGIN_BEGIN_TO_SYNC,										// ֪ͨģ�鿪ʼ����ͬ��

		EVENT_VALUE_PLUGIN_END = EVENT_VALUE_PLUGIN_END ,			//���е��¼�����
	};

	// PlugIn�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_PlugInMessageValue
	{
		MESSAGE_VALUE_PLUGIN_LOAD_ALL_PLUGIN_FINISHED	=	MESSAGE_VALUE_PLUGIN_BEGIN,	//	֪ͨ���еĲ���Ѿ��������
		MESSAGE_VALUE_PLUGIN_EXPORT_BEGIN,								//	֪ͨ�������ʼ����
		MESSAGE_VALUE_PLUGIN_EXPORT_FINISHED,						//	֪ͨ������������������Ѿ��������
		MESSAGE_VALUE_PLUGIN_EXPORT_ALL_FINISHED,				//	�㲥֪ͨ�ղؼкϲ������Ѿ����

		MESSAGE_VALUE_PLUGIN_IMPORT_PRE_BEGIN,						//	ͬ������֮ǰ���������Ԥ����
		MESSAGE_VALUE_PLUGIN_IMPORT_BEGIN,							//	֪ͨ�������ʼ����
		MESSAGE_VALUE_PLUGIN_IMPORT_FINISHED,						//	֪ͨ������������������Ѿ��������
		MESSAGE_VALUE_PLUGIN_IMPORT_ALL_FINISHED,				//	֪ͨ���е�������Ѿ�ͬ������

		MESSAGE_VALUE_PLUGIN_EXINPORT_PROCESS,					//	֪ͨ�ϲ㵱ǰ�Ľ���
	};

	// PlugIn�ܹ������ֱ�ӵ���
	enum E_PlugInServiceValue
	{
		SERVICE_VALUE_PLUGIN_GET_AVAILABLE_PLUGIN	=	SERVICE_VALUE_PLUGIN_BEGIN,			//	��ȡ���õĲ��
	};


	//=================================================================//
	//                   PlugIn����ʹ�õ���Message�ṹ	   		 						//
	//=================================================================//
	struct PlugIn_ExportBeginMessage : MessageExtraInfo
	{
		PlugIn_ExportBeginMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_EXPORT_BEGIN;
			srcMId = MODULE_ID_PLUGIN; 

			pPlugIn = 0;
		}

		IPlugIn*	pPlugIn;
	};

	struct PlugIn_ExportEndMessage : MessageExtraInfo
	{
		PlugIn_ExportEndMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_EXPORT_FINISHED;
			srcMId = MODULE_ID_PLUGIN; 

			pPlugIn = 0;
			nFavoriteNum = 0;
			bSuccess = TRUE;
		}

		IPlugIn*	pPlugIn;
		int nFavoriteNum;
		BOOL	bSuccess;
	};

	struct PlugIn_ImportBeginMessage : MessageExtraInfo
	{
		PlugIn_ImportBeginMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_IMPORT_BEGIN;
			srcMId = MODULE_ID_PLUGIN; 

			pPlugIn = 0;
		}

		IPlugIn*	pPlugIn;
	};

	struct PlugIn_ImportEndMessage : MessageExtraInfo
	{
		PlugIn_ImportEndMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_IMPORT_FINISHED;
			srcMId = MODULE_ID_PLUGIN; 

			pPlugIn = 0;
			nFavoriteNum = 0;
			bSuccess = TRUE;
		}

		IPlugIn*	pPlugIn;
		int nFavoriteNum;
		BOOL	bSuccess;
	};

 	struct PlugIn_InExportEndMessage : MessageExtraInfo
	{
		PlugIn_InExportEndMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_EXINPORT_PROCESS;
			srcMId = MODULE_ID_PLUGIN; 

			ZeroMemory(szProcessText, sizeof(wchar_t)*MAX_PATH);
		}

		wchar_t	szProcessText[MAX_PATH];
	};

	//=================================================================//
	//                   PlugIn����ʹ�õ���event�ṹ	   		 	  //
	//=================================================================//
	struct PlugInEvent	:	public EventExtraInfo
	{
	public:
		PlugInEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_PLUGIN;
		}
	};


	//=================================================================//
	//                   PlugInManager����ʹ�õ���Service				   //
	//=================================================================//
	struct PlugIn_GetAvailablePlugInsService : public Service
	{
		PlugIn_GetAvailablePlugInsService()
		{
			serviceId	=	SERVICE_VALUE_PLUGIN_GET_AVAILABLE_PLUGIN;
			pvPlugIns = NULL;
		}

		std::vector<IPlugIn*>*	pvPlugIns;
	};
};