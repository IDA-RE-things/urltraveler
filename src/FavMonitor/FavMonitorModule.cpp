#include "stdafx.h"
#include "FavMonitorModule.h"
#include <sstream>

HMODULE	g_hModule = NULL;

using namespace favmonitor;

namespace favmonitor
{
	FavMonitorModule*	g_FavMonitorModule = NULL;
	FavMonitorModuleFactory*	g_FavMonitorModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(FavMonitorModule)
EXPORT_RELEASEMODULEFACTORY(FavMonitorModule)

FavMonitorModule::FavMonitorModule()
{

}

FavMonitorModule::~FavMonitorModule()
{

}

BEGIN_EVENT_MAP(FavMonitorModule)
	//ON_EVENT(EVENT_VALUE_TRAYICON_SHOW,OnEvent_ShowTrayIcon)
	//ON_EVENT(EVENT_VALUE_TRAYICON_SHOW_UPDATE_WND, OnEvent_ShowUpdateWnd)
END_EVENT_MAP()

BEGIN_MESSAGE_MAP(FavMonitorModule)
	//ON_MESSAGE(MESSAGE_VALUE_CORE_BEGIN_SHOW, OnMessage_Show)
	//ON_MESSAGE(MESSAGE_VALUE_CORE_PRE_APP_EXIT, OnMessage_PreExit)
END_MESSAGE_MAP()

//----------------------------------------------------------------------------------------
//名称: Load
//描述: 主程序通过该方法对模块进行加载
//参数: 
//		@param	pManager			主模块总线的指针	
//返回: 
//		如果加载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
BOOL FavMonitorModule::Load(IModuleManager* pManager)
{
	__super::Load(pManager);
	
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: Unload
//描述: 主程序通过该方法对模块进行卸载
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
BOOL FavMonitorModule::Unload()
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* FavMonitorModule::GetModuleName() 
{
	return L"FavMonitorModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const FavMonitorModule::GetModuleId()
{
	return MODULE_ID_FAVMONITOR;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void FavMonitorModule::ProcessEvent(const Event& evt)
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
void FavMonitorModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lparam			参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 FavMonitorModule::CallDirect(const param lparam, param wparam) 
{
	return -1;
}