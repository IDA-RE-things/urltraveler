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
		SERVICE_VALUE_DATABASE_FAVICON_LOAD = SERVICE_VALUE_DATABASE_BEGIN,					//	加载特定的URL的收藏夹图标
	};

	/** 本模块中使用的一些全局宏定义常量
	本处的宏定义常量无实际语义,这些常量在代码中不作为有语义的常量使用
	如:字符串最大长度、数组最大长度等
	*/	
#define DB_MAX_LEN_BINARY_BUFFER			(20480+1)

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

			nIconDataLen = 0;
			pIconData = NULL;
		}

		~Database_FavIconSaveEvent()
		{
			if( pIconData != NULL)
			{
				delete pIconData;
				pIconData = NULL;
			}

			nIconDataLen = 0;
		}

		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
		int	nIconDataLen;					//	Icon缓冲区的产度
		const char*	pIconData;			//	ICON数据
	};

	//============================================================================//
	//                   Database中所使用到的Service结构			              //
	//============================================================================//
	struct Database_GetFavoriteIconService : public Service
	{
		Database_GetFavoriteIconService()
		{
			serviceId = SERVICE_VALUE_DATABASE_FAVICON_LOAD;
			ZeroMemory(szFavoriteUrl, MAX_PATH);
			hcon	=	NULL;
		}

		wchar_t szFavoriteUrl[MAX_PATH];										//	
		HICON	hcon;				//	返回的ICON	
	};

};