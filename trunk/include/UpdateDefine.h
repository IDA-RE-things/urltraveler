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

	/** 本模块中使用的一些全局宏定义常量
	本处的宏定义常量无实际语义,这些常量在代码中不作为有语义的常量使用
	如:字符串最大长度、数组最大长度等
	*/	
#define DB_MAX_LEN_BINARY_BUFFER			(20480+1)

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

	// 存储收藏夹图标的接口
	struct Update_FavIconSaveEvent	:	public UpdateEvent
	{
		Update_FavIconSaveEvent()
		{
		}

		~Update_FavIconSaveEvent()
		{
		}
	};

	//============================================================================//
	//                   Update中所使用到的Service结构			              //
	//============================================================================//
};