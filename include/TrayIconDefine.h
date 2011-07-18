#pragma once

// 图标管理模块的对外的头文件信息
#include "SndaBase.h"

namespace trayicon
{
	// TrayIcon对外公开的事件
	enum E_TrayIconEventValue
	{
		EVENT_VALUE_TRAYICON_SHOW = EVENT_VALUE_TRAYICON_BEGIN,		//	通知显示任务栏图标
		EVENT_VALUE_TRAYICON_SHOW_UPDATE_WND,										//	弹出更新提示框
	};

	// TrayIcon能够可能对外发送的广播消息
	enum E_TrayIconMessageValue
	{

	};

	// TrayIcon能够处理的直接调用
	enum E_TrayIconServiceValue
	{
	};

	//===========================================//
	//                   TrayIcon中所使用到的event结构					      //
	//===========================================//
	struct TrayIconEvent	:	public EventExtraInfo
	{
	public:
		TrayIconEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_TRAYICON;
		}
	};

	struct TrayIcon_ShowUpdateWndEvent	:	public	TrayIconEvent
	{
	public:
		   TrayIcon_ShowUpdateWndEvent()
		   {
			   nNewestVersion	=	0;
		   }

		   //	当前的最新版本
		   int	nNewestVersion;
	};
};