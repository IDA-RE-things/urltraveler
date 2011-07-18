#pragma once

// PlugIn的对外的头文件信息

#include "SndaBase.h"
#include "PlugIn.h"


namespace plugin
{
	// PlugIn对外公开的事件
	enum E_PlugInEventValue
	{
		EVENT_VALUE_PLUGIN_OPEN = EVENT_VALUE_PLUGIN_BEGIN,		//	打开主界面

		EVENT_VALUE_PLUGIN_LOAD_ALL,												//	加载插件
		EVENT_VALUE_PLUGIN_CHECK_IS_WORKED,									//	检查当前的插件是否需要工作。如果没有安装对应的浏览器，则当前插件不需要安装
		EVENT_VALUE_PLUGIN_COMBINE_FAVORITE,								//	通知模块需要获取合并后的收藏夹

		EVENT_VALUE_PLUGIN_END = EVENT_VALUE_PLUGIN_END ,			//所有的事件结束
	};

	// PlugIn能够可能对外发送的广播消息
	enum E_PlugInMessageValue
	{
		MESSAGE_VALUE_PLUGIN_LOAD_FAVORITE_DATA_FINISHED	=	MESSAGE_VALUE_PLUGIN_BEGIN,	//	广播通知收藏夹合并数据已经完成
		MESSAGE_VALUE_PLUGIN_LOAD_ALL_FINISHED,					//	通知所有的插件已经加载完毕
	};

	// PlugIn能够处理的直接调用
	enum E_PlugInServiceValue
	{
		SERVICE_VALUE_PLUGIN_GET_AVAILABLE_PLUGIN	=	SERVICE_VALUE_PLUGIN_BEGIN,			//	获取可用的插件
	};

	//=================================================================//
	//                   PlugIn中所使用到的event结构	   		 	  //
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
	//                   PlugInManager中所使用到的Service				   //
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