#include "stdafx.h"
#include "SettingModule.h"

using namespace setting;

namespace setting
{
	SettingModule*	g_SettingModule = NULL;
	CSettingModuleFactory*	g_SettingModuleFactory = NULL;
}

// 导出借口实现
IModuleFactory*	GetModuleFactory()
{
	if( g_SettingModuleFactory == NULL)
	{
		g_SettingModuleFactory = new CSettingModuleFactory();
		g_SettingModuleFactory->QueryModulePoint(1, (IModule*&)g_SettingModule);
		
		ASSERT( g_SettingModule != NULL);
	}

	return g_SettingModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_SettingModuleFactory == p);
	if( g_SettingModuleFactory  != NULL)
	{
		delete g_SettingModuleFactory;
		g_SettingModuleFactory = NULL;
	}
}

SettingModule::SettingModule()
{

}

SettingModule::~SettingModule()
{

}

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* SettingModule::GetModuleName() 
{
	return L"SettingModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const SettingModule::GetModuleId()
{
	return MODULE_ID_SETTING;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void SettingModule::ProcessEvent(const Event& evt)
{
}

//----------------------------------------------------------------------------------------
//名称: ProcessMessage
//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
//参数: 
//		@param	msg			需要处理的广播消息
//----------------------------------------------------------------------------------------
void SettingModule::ProcessMessage(const Message& msg) 
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
int32 SettingModule::CallDirect(const param lparam, param wparam) 
{
	return -1;
}