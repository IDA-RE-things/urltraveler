#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"

#include "SettingWnd.h"
#include "OptionCenter.h"


extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class SettingModule : public ModuleImpl
{
	DECLEAR_EVENT_MAP(SettingModule);
	DECLEAR_SERVICE_MAP(SettingModule);

public:
	SettingModule();
	~SettingModule();

	//----------------------------------------------------------------------------------------
	//名称: Load
	//描述: 主程序通过该方法对模块进行加载
	//参数: 
	//		@param	pManager			主模块总线的指针	
	//返回: 
	//		如果加载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Load(IModuleManager* pManager);

	//----------------------------------------------------------------------------------------
	//名称: GetModuleName
	//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
	//返回: 
	//		如果卸载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetModuleName();

 	//----------------------------------------------------------------------------------------
	//名称: GetModuleId
	//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
	//返回: 
	//		返回该模块的唯一的ID
	//----------------------------------------------------------------------------------------
	virtual uint32 const GetModuleId();

 	//----------------------------------------------------------------------------------------
	//名称: ProcessEvent
	//描述: 该模块可以处理的所有的点对点事件
	//参数: 
	//		@param	evt			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual void ProcessEvent(const Event& evt);

 	//----------------------------------------------------------------------------------------
	//名称: ProcessMessage
	//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
	//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
	//参数: 
	//		@param	msg			需要处理的广播消息
	//----------------------------------------------------------------------------------------
	virtual void ProcessMessage(const Message& msg);

 	//----------------------------------------------------------------------------------------
	//名称: CallDirect
	//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
	//			方法而不需要通过总线。
	//参数: 
	//		@param	lparam			参数1
	//		@param	rparam			参数2
	//----------------------------------------------------------------------------------------
	virtual int32 CallDirect(const param lServiceValue, param wparam);

public:

	OptionCenter	m_OptionCenter;  

protected:

	void	OnEvent_OpenSettingWnd(Event* pEvent);

protected:

	int	OnService_GetAutoStartWithWindow(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetRememberPassword(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetAutoLogin(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetExitWhenCloseWnd(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetAutoLocalSync(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetAutoRemoteSync(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetUpdateType(ServiceValue lServiceValue, param	lParam);

protected:
	CSettingWnd*	m_pSettingWnd;

};

class SettingModuleFactory : public ModuleFactoryImpl<SettingModule>{};

namespace setting
{
	extern SettingModule*	g_SettingModule;
	extern SettingModuleFactory*	g_SettingModuleFactory;
}

#define  OPTION_CENTER         (setting::g_SettingModule->m_OptionCenter)