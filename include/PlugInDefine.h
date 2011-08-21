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

		EVENT_VALUE_PLUGIN_LOAD_ALL,													//	加载插件
		EVENT_VALUE_PLUGIN_CHECK_IS_WORKED,									//	检查当前的插件是否需要工作。如果没有安装对应的浏览器，则当前插件不需要安装
		EVENT_VALUE_PLUGIN_COMBINE_FAVORITE,									//	通知模块需要获取合并后的收藏夹
		EVENT_VALUE_PLUGIN_BEGIN_TO_SYNC,										// 通知模块开始进行同步

		EVENT_VALUE_PLUGIN_END = EVENT_VALUE_PLUGIN_END ,			//所有的事件结束
	};

	// PlugIn能够可能对外发送的广播消息
	enum E_PlugInMessageValue
	{
		MESSAGE_VALUE_PLUGIN_LOAD_ALL_PLUGIN_FINISHED	=	MESSAGE_VALUE_PLUGIN_BEGIN,	//	通知所有的插件已经加载完毕
		MESSAGE_VALUE_PLUGIN_EXPORT_BEGIN,								//	通知浏览器开始导出
		MESSAGE_VALUE_PLUGIN_EXPORT_FINISHED,						//	通知给定的浏览器的数据已经导出完毕
		MESSAGE_VALUE_PLUGIN_EXPORT_ALL_FINISHED,				//	广播通知收藏夹合并数据已经完成

		MESSAGE_VALUE_PLUGIN_IMPORT_PRE_BEGIN,						//	同步数据之前进行清理的预处理
		MESSAGE_VALUE_PLUGIN_IMPORT_BEGIN,							//	通知浏览器开始导入
		MESSAGE_VALUE_PLUGIN_IMPORT_FINISHED,						//	通知给定的浏览器的数据已经导入完毕
		MESSAGE_VALUE_PLUGIN_IMPORT_ALL_FINISHED,				//	通知所有的浏览器已经同步结束

		MESSAGE_VALUE_PLUGIN_EXINPORT_PROCESS,					//	通知上层当前的进度
	};

	// PlugIn能够处理的直接调用
	enum E_PlugInServiceValue
	{
		SERVICE_VALUE_PLUGIN_GET_AVAILABLE_PLUGIN	=	SERVICE_VALUE_PLUGIN_BEGIN,			//	获取可用的插件
	};


	//=================================================================//
	//                   PlugIn中所使用到的Message结构	   		 						//
	//=================================================================//
	struct PlugIn_ExportBeginMessage : MessageExtraInfo
	{
		PlugIn_ExportBeginMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_EXPORT_BEGIN;
			srcMId = MODULE_ID_PLUGIN; 

			pPlugIn = 0;
		}

		IPlugIn*	pPlugIn;
	};

	struct PlugIn_ExportEndMessage : MessageExtraInfo
	{
		PlugIn_ExportEndMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_EXPORT_FINISHED;
			srcMId = MODULE_ID_PLUGIN; 

			pPlugIn = 0;
			nFavoriteNum = 0;
			bSuccess = TRUE;
		}

		IPlugIn*	pPlugIn;
		int nFavoriteNum;
		BOOL	bSuccess;
	};

	struct PlugIn_ImportBeginMessage : MessageExtraInfo
	{
		PlugIn_ImportBeginMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_IMPORT_BEGIN;
			srcMId = MODULE_ID_PLUGIN; 

			pPlugIn = 0;
		}

		IPlugIn*	pPlugIn;
	};

	struct PlugIn_ImportEndMessage : MessageExtraInfo
	{
		PlugIn_ImportEndMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_IMPORT_FINISHED;
			srcMId = MODULE_ID_PLUGIN; 

			pPlugIn = 0;
			nFavoriteNum = 0;
			bSuccess = TRUE;
		}

		IPlugIn*	pPlugIn;
		int nFavoriteNum;
		BOOL	bSuccess;
	};

 	struct PlugIn_InExportEndMessage : MessageExtraInfo
	{
		PlugIn_InExportEndMessage()
		{
			messageValue = MESSAGE_VALUE_PLUGIN_EXINPORT_PROCESS;
			srcMId = MODULE_ID_PLUGIN; 

			ZeroMemory(szProcessText, sizeof(wchar_t)*MAX_PATH);
		}

		wchar_t	szProcessText[MAX_PATH];
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