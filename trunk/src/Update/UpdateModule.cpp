#include "stdafx.h"
#include "UpdateModule.h"
#include "PathHelper.h"
#include "UpdateDefine.h"
#include "StringHelper.h"
#include <string>
#include "ImageHelper.h"
#include "MiscHelper.h"

HMODULE	g_hModule = NULL;

using namespace std;
using namespace update;

namespace update
{
	UpdateModule*	g_UpdateModule = NULL;
	UpdateModuleFactory*	g_UpdateModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(UpdateModule)
EXPORT_RELEASEMODULEFACTORY(UpdateModule)

UpdateModule::UpdateModule()
{

}

UpdateModule::~UpdateModule()
{

}

BEGIN_EVENT_MAP(UpdateModule)
	ON_EVENT(EVENT_VALUE_UPDATE_CHECK_UPDATEINFO, OnEvent_CheckUpdateInfo)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(UpdateModule)
END_SERVICE_MAP()

//----------------------------------------------------------------------------------------
//名称: Load
//描述: 主程序通过该方法对模块进行加载
//参数: 
//		@param	pManager			主模块总线的指针	
//返回: 
//		如果加载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
BOOL UpdateModule::Load(IModuleManager* pManager) 
{
	__super::Load(pManager);

	m_strUpdatePath	=	MiscHelper::GetUpdatePath();

	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* UpdateModule::GetModuleName() 
{
	return L"UpdateModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const UpdateModule::GetModuleId()
{
	return MODULE_ID_DATABASE;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void UpdateModule::ProcessEvent(const Event& evt)
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
void UpdateModule::ProcessMessage(const Message& msg) 
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
int32 UpdateModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void	UpdateModule::OnEvent_CheckUpdateInfo(Event* pEvent)
{

}