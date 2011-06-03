#pragma once

// Database的对外的头文件信息

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace database
{
	// Database对外公开的事件
	enum E_DatabaseEventValue
	{
		// Favicon对应的图标
		EVENT_VALUE_DATABASE_FAVICON_SAVE = EVENT_VALUE_DATABASE_BEGIN,		//打开主界面
	};

	// Database能够可能对外发送的广播消息
	enum E_DatabaseMessageValue
	{
	};

	// Database能够处理的直接调用
	enum E_DatabaseServiceValue
	{
		SERVICE_VALUE_DATABASE_FAVICON_LOAD,					//	加载特定的URL的收藏夹图标
	};

	//============================================================================//
	//                   Database中所使用到的数据结构			                  //
	//============================================================================//



	//============================================================================//
	//                   Database中所使用到的event结构  			              //
	//============================================================================//
	struct DatabaseEvent	:	public ExtraInfo
	{
		DatabaseEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_DATABASE;
		}
	};

	// 存储收藏夹图标的接口
	struct Database_FavIconSaveEvent	:	public DatabaseEvent
	{
		Database_FavIconSaveEvent()
		{
			eventValue	=	 EVENT_VALUE_DATABASE_FAVICON_SAVE;
			ZeroMemory(szFavoriteUrl, MAX_PATH);
		}

		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
		HICON	hIcon;					//	下载的图标数据
	};

	//============================================================================//
	//                   Database中所使用到的Service结构			              //
	//============================================================================//
	struct Database_GetFavoriteIconService
	{
		Database_GetFavoriteIconService()
		{
			ZeroMemory(szFavoriteUrl, MAX_PATH);
			hcon	=	NULL;
		}

		wchar_t szFavoriteUrl[MAX_PATH];										//	
		HICON	hcon;				//	返回的ICON	
	};

};