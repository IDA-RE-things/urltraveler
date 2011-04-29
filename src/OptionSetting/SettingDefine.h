#pragma once

// ���ý���Ķ����ͷ�ļ���Ϣ
#include "SndaBase.h"

namespace setting
{
	// Setting���⹫�����¼�
	enum E_SettingEventValue
	{
		EVENT_VALUE_SETTING_OPEN = EVENT_VALUE_SETTING_BEGIN,		//��������
		EVENT_VALUE_SETTING_CLOSE,						//	�رյ�ǰ������

		EVENT_VALUE_SETTING_END = EVENT_VALUE_SETTING_END ,			//���е��¼�����
	};

	// SETTING�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_SettingMessageValue
	{

	};

	// Setting�ܹ������ֱ�ӵ���
	enum E_SettingServiceValue
	{
	};

	//===========================================//
	//                   Setting����ʹ�õ���event�ṹ								 	  //
	//===========================================//
	struct SettingEvent	:	public ExtraInfo
	{
	public:
		SettingEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_SETTING;
		}
	};

	// ����������Ҫ�Ķ�Ӧ��Event
	struct Setting_OpenEvent	:	public	SettingEvent
	{
	public:
		Setting_OpenEvent()
		{
			eventValue	=	 EVENT_VALUE_SETTING_OPEN;
		}
	}

	//	�ر���������Ҫ�Ķ�Ӧ��Event
	struct Setting_CloseEvent	:	public	SettingEvent
	{
	public:
		Setting_OpenEvent()
		{
			eventValue	=	 EVENT_VALUE_SETTING_CLOSE;
		}
	}

};