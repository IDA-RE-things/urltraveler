#pragma once

// MainFrame的对外的头文件信息

#include "SndaBase.h"
#include "UrlTravelerHeader.h"

namespace mainframe
{
	// MainFrame对外公开的事件
	enum E_MainFrameEventValue
	{
		EVENT_VALUE_MAINFRAME_OPEN = EVENT_VALUE_MAINFRAME_BEGIN,		//打开主界面
		EVENT_VALUE_MAINFRAME_CLOSE,						//	关闭当前主界面
		EVENT_VALUE_MAINFRAME_SHOW,						//	显示主对话框
		EVENT_VALUE_MAINFRAME_HIDE,							//	隐藏当前主界面
		EVENT_VALUE_MAINFRAME_OPENLOGIN,				//	打开登录对话框
		EVENT_VALUE_MAINFRAME_DELETE_FAVORITE,				//	删除某个给定的收藏夹记录
		EVENT_VALUE_MAINFRAME_DELETE_FAVORITE_FOLD,	//	删除某个给定的收藏夹目录
		EVENT_VALUE_MAINFRAME_ADD_URL,				//	增加一个新的URL
		EVENT_VALUE_MAINFRAME_OPEN_URL,				//	param0 为需要打开的URL的index
		EVENT_VALUE_MAINFRAME_COPY_URL,				//	param0 为需要拷贝的URL的index
		EVENT_VALUE_MAINFRAME_EDIT_URL,
		EVENT_VALUE_MAINFRAME_RENAME_URL,
		EVENT_VALUE_MAINFRAME_SHARE_URL,

		EVENT_VALUE_MAINFRAME_END = EVENT_VALUE_MAINFRAME_END ,			//所有的事件结束
	};

	// MainFrame能够可能对外发送的广播消息
	enum E_MainFrameMessageValue
	{
	};

	// MainFrame能够处理的直接调用
	enum E_MainFrameServiceValue
	{
		//	获取主窗口的句柄，一些模块需要指定主窗口为父窗口
		SERVICE_VALUE_MAINFRAME_GET_MAINWND	=	SERVICE_VALUE_MAINFRAME_BEGIN,				
	};

	//===========================================//
	//                   MainFrame中所使用到的event结构								 	  //
	//===========================================//
	struct MainFrameEvent	:	public EventExtraInfo
	{
	public:
		MainFrameEvent()
		{
			srcMId	=	MODULE_ID_MAINFRAME;
			desMId	=	MODULE_ID_INVALID;
		}
	};

	enum	E_MainFrameTab
	{
		TAB_FAVORITE,
		TAB_HISTORY,	
	};

	// 打开主界面需要的对应的Event
	struct MainFrame_OpenEvent	:	public	MainFrameEvent
	{
	public:
		MainFrame_OpenEvent()
		{
			eventValue	=	 EVENT_VALUE_MAINFRAME_OPEN;
			eTabType		=	TAB_FAVORITE;
		}

		E_MainFrameTab	eTabType;			//	指定打开后显示哪一个Tab页面
	};

	// 删除某个收藏记录
	struct MainFrame_DeleteFavoriteEvent	:	public MainFrameEvent
	{
		MainFrame_DeleteFavoriteEvent()
		{
			eventValue	=	EVENT_VALUE_MAINFRAME_DELETE_FAVORITE;
			nDeleteNodeId = -1;
		}

		//	需要删除的URL的索引
		int nDeleteNodeId;	
	};

	// 删除某个收藏夹记录
	struct MainFrame_DeleteFavoriteFoldEvent	:	public MainFrameEvent
	{
		MainFrame_DeleteFavoriteFoldEvent()
		{
			eventValue	=	EVENT_VALUE_MAINFRAME_DELETE_FAVORITE_FOLD;
			nDeleteIndex	= -1;
		}

		//	需要删除的URL的索引
		int	nDeleteIndex;	
	};

	//===========================================//
	//                   MainFrame中所使用到的Service							 	  //
	//===========================================//
	struct MainFrame_GetWndService	:	public Service
	{
		MainFrame_GetWndService()
		{
			serviceId	=	SERVICE_VALUE_MAINFRAME_GET_MAINWND;
		}

		void*		pBaseWnd;			//	当前主窗口的句柄
	};
};