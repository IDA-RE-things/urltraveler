#pragma once

// 设置界面的对外的头文件信息
#include "SndaBase.h"

namespace setting
{
	// Setting对外公开的事件
	enum E_SettingEventValue
	{
		EVENT_VALUE_SETTING_OPEN = EVENT_VALUE_SETTING_BEGIN,		//打开主界面
		EVENT_VALUE_SETTING_CLOSE,						//	关闭当前主界面

		EVENT_VALUE_SETTING_END = EVENT_VALUE_SETTING_END ,			//所有的事件结束
	};

	// SETTING能够可能对外发送的广播消息
	enum E_SettingMessageValue
	{

	};

	// Setting能够处理的直接调用
	enum E_SettingServiceValue
	{
	};

	//===========================================//
	//                   Setting中所使用到的event结构								 	  //
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

	// 打开主界面需要的对应的Event
	struct Setting_OpenEvent	:	public	SettingEvent
	{
	public:
		Setting_OpenEvent()
		{
			eventValue	=	 EVENT_VALUE_SETTING_OPEN;
		}
	}

	//	关闭主界面需要的对应的Event
	struct Setting_CloseEvent	:	public	SettingEvent
	{
	public:
		Setting_OpenEvent()
		{
			eventValue	=	 EVENT_VALUE_SETTING_CLOSE;
		}
	}

};