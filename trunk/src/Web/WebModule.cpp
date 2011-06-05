#include "stdafx.h"
#include "WebModule.h"
#include "Web.h"
#include "PathHelper.h"
#include "WebDefine.h"
#include "StringHelper.h"
#include <string>
#include "ImageHelper.h"
#include "httpmanager.h"

HMODULE	g_hModule = NULL;

using namespace std;
using namespace web;

namespace web
{
	WebModule*	g_WebModule = NULL;
	WebModuleFactory*	g_WebModuleFactory = NULL;
}

// 导出借口实现
IModuleFactory*	GetModuleFactory()
{
	if( g_WebModuleFactory == NULL)
	{
		g_WebModuleFactory = new WebModuleFactory();
		g_WebModuleFactory->QueryModulePoint(1, (IModule*&)g_WebModule);
		
		ASSERT( g_WebModule != NULL);
	}

	return g_WebModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_WebModuleFactory == p);
	if( g_WebModuleFactory  != NULL)
	{
		delete g_WebModuleFactory;
		g_WebModuleFactory = NULL;
	}
}

WebModule::WebModule()
{

}

WebModule::~WebModule()
{

}

BEGIN_EVENT_MAP(WebModule)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(WebModule)
END_SERVICE_MAP()

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* WebModule::GetModuleName() 
{
	return L"WebModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const WebModule::GetModuleId()
{
	return MODULE_ID_WEB;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void WebModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//名称: ProcessMessage
//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
//参数: 
//		@param	msg			需要处理的广播消息
//----------------------------------------------------------------------------------------
void WebModule::ProcessMessage(const Message& msg) 
{
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lparam			参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 WebModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}


void WebModule::OnEvent_SaveFavoriteIcon(Event* evt)
{
}

int32 WebModule:: OnService_GetFavoriteIcon(ServiceValue lService, param lparam)
{
	return -1;
}
