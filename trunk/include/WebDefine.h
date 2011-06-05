#pragma once

// Web的对外的头文件信息

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace web
{
	// Web对外公开的事件
	enum E_WebEventValue
	{
		// Favicon对应的图标
		EVENT_VALUE_WEB_GET_FAVICON = EVENT_VALUE_WEB_BEGIN,		// 通知去拉取Favorite图标
	};

	// Web能够可能对外发送的广播消息
	enum E_WebMessageValue
	{
	};

	// Web能够处理的直接调用
	enum E_WebServiceValue
	{
		SERVICE_VALUE_WEB_FAVICON_LOAD = SERVICE_VALUE_WEB_BEGIN,					//	加载特定的URL的收藏夹图标
	};

	//============================================================================//
	//                   Web中所使用到的数据结构			                  //
	//============================================================================//



	//============================================================================//
	//                   Web中所使用到的event结构  			              //
	//============================================================================//
	struct WebEvent	:	public ExtraInfo
	{
		WebEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_WEB;
		}
	};

	// 向网络请求收藏夹ICON图标
	struct Web_GetFavIconEvent	:	public WebEvent
	{
		Web_GetFavIconEvent()
		{
			eventValue	=	 EVENT_VALUE_WEB_GET_FAVICON;
			ZeroMemory(szFavoriteUrl, MAX_PATH);
		}

		// 收藏夹图标的URL
		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
	};
};