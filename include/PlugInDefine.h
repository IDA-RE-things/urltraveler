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

		EVENT_VALUE_PLUGIN_LOAD_ALL,												//	���ز��
		EVENT_VALUE_PLUGIN_CHECK_IS_WORKED,									//	��鵱ǰ�Ĳ���Ƿ���Ҫ���������û�а�װ��Ӧ�����������ǰ�������Ҫ��װ
		EVENT_VALUE_PLUGIN_COMBINE_FAVORITE,								//	֪ͨģ����Ҫ��ȡ�ϲ�����ղؼ�

		EVENT_VALUE_PLUGIN_END = EVENT_VALUE_PLUGIN_END ,			//���е��¼�����
	};

	// PlugIn�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_PlugInMessageValue
	{
		MESSAGE_VALUE_PLUGIN_LOAD_FAVORITE_DATA_FINISHED	=	MESSAGE_VALUE_PLUGIN_BEGIN,	//	�㲥֪ͨ�ղؼкϲ������Ѿ����
		MESSAGE_VALUE_PLUGIN_LOAD_ALL_FINISHED,					//	֪ͨ���еĲ���Ѿ��������
	};

	// PlugIn�ܹ������ֱ�ӵ���
	enum E_PlugInServiceValue
	{
		SERVICE_VALUE_PLUGIN_GET_AVAILABLE_PLUGIN	=	SERVICE_VALUE_PLUGIN_BEGIN,			//	��ȡ���õĲ��
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