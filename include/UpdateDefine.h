#pragma once

// Update的对外的头文件信息

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace update
{
	// Update对外公开的事件
	enum E_UpdateEventValue
	{
		EVENT_VALUE_UPDATE_CHECK_UPDATEINFO = EVENT_VALUE_UPDATE_BEGIN,		// 检查当前应用程序是否存在更新
		EVENT_VALUE_UPDATE_SHOW_UPDATE_HINT_WND,			//	显示更新提示窗口
		EVENT_VALUE_UPDATE_SHOW_DOWNLOADING_WND,			//	显示下载进度窗口
	};

	// Update能够可能对外发送的广播消息
	enum E_UpdateMessageValue
	{
	};

	// Update能够处理的直接调用
	enum E_UpdateServiceValue
	{
		SERVICE_VALUE_UPDATE_FAVICON_LOAD = SERVICE_VALUE_UPDATE_BEGIN,					//	加载特定的URL的收藏夹图标
	};

	//============================================================================//
	//                   Update中所使用到的数据结构			                  //
	//============================================================================//



	//============================================================================//
	//                   Update中所使用到的event结构  			              //
	//============================================================================//
	struct UpdateEvent	:	public ExtraInfo
	{
		UpdateEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_UPDATE;
		}
	};

	// 通知提示显示更新信息对话框
#define MAX_UPDATE_VERSION_LEN			(32+1)
	struct Update_ShowUpdateInfoEvent	:	public UpdateEvent
	{
		Update_ShowUpdateInfoEvent()
		{
			eventValue = EVENT_VALUE_UPDATE_SHOW_UPDATE_HINT_WND;
			nVersion = 0;
		}

		int nVersion;
	};

	struct Update_ShowUpdateDownloadingEvent	:	public UpdateEvent
	{
		Update_ShowUpdateDownloadingEvent()
		{
			eventValue = EVENT_VALUE_UPDATE_SHOW_DOWNLOADING_WND;
			nLastestVersion = 0;
			ZeroMemory(szDownloadUrl, MAX_PATH*sizeof(wchar_t));
			ZeroMemory(szSavePath, MAX_PATH*sizeof(wchar_t));
		}

		int nLastestVersion;		//	最近的版本

		wchar_t	szDownloadUrl[MAX_PATH];	// 下载的URL
		wchar_t	szSavePath[MAX_PATH];
	};

	//============================================================================//
	//                   Update中所使用到的Service结构			              //
	//============================================================================//
};