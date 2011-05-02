#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImp.h"
#include "TrayMgr.h"
#include "TrayEvent.h"


extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class TrayIconModule : public ModuleImpl
{
	DECLEAR_EVENT_MAP()

public:
	TrayIconModule();
	virtual ~TrayIconModule();

	//----------------------------------------------------------------------------------------
	//名称: GetModuleName
	//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
	//返回: 
	//		如果卸载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	const wchar_t* GetModuleName();

	//----------------------------------------------------------------------------------------
	//名称: GetModuleId
	//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
	//返回: 
	//		返回该模块的唯一的ID
	//----------------------------------------------------------------------------------------
	uint32 const GetModuleId();

	//----------------------------------------------------------------------------------------
	//名称: ProcessEvent
	//描述: 该模块可以处理的所有的点对点事件
	//参数: 
	//		@param	evt			需要处理的事件
	//----------------------------------------------------------------------------------------
	void ProcessEvent(const Event& evt);

	//----------------------------------------------------------------------------------------
	//名称: ProcessMessage
	//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
	//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
	//参数: 
	//		@param	msg			需要处理的广播消息
	//----------------------------------------------------------------------------------------
	void ProcessMessage(const Message& msg);

	//----------------------------------------------------------------------------------------
	//名称: CallDirect
	//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
	//			方法而不需要通过总线。
	//参数: 
	//		@param	lparam			参数1
	//		@param	rparam			参数2
	//----------------------------------------------------------------------------------------
	int32 CallDirect(const param lparam, param wparam);

	//----------------------------------------------------------------------------------------
	//名称: PaybackExtraInfo
	//描述: 某个模块如果有自定义的复杂的数据需要传给其余的模块，那么它可以构造一个ExtraInfo结构
	//	在其余的模块使用完毕后，该结构必须被释放，否则会造成内存泄露。释放必须由模块自身完成。
	//	某个模块都必须提供一个PaybackExtraInfo接口，释放自己的自定义的数据类型
	//参数: 
	//		@param	valudId			对应的pExtraInfo的值，内部根据该值进行对应的释放，该值只有模块自己理解
	//		@param	pExtraInfo	需要释放的ExtraInfo数据
	//----------------------------------------------------------------------------------------
	void PaybackExtraInfo(uint32 valudId, void* pExtraInfo);


	// Event处理函数
protected:

	void	OnEvent_ShowTrayIcon(Event* pEvent);	

private:

	CTrayMgr		m_TrayMgr;
};

class CTrayIconModuleFactory : public ModuleFactoryImpl<TrayIconModule>{};

namespace trayicon
{
	extern TrayIconModule*	g_TrayIconModule;
	extern CTrayIconModuleFactory*	g_TrayIconModuleFactory;
}