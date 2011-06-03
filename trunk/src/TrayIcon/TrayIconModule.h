#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "TrayMgr.h"

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class TrayIconModule : public ModuleImpl, public ITrayEvent
{
	DECLEAR_EVENT_MAP(TrayIconModule)
	DECLEAR_MESSAGE_MAP(TrayIconModule)

public:
	TrayIconModule();
	virtual ~TrayIconModule();

	//----------------------------------------------------------------------------------------
	//名称: Load
	//描述: 主程序通过该方法对模块进行加载
	//参数: 
	//		@param	pManager			主模块总线的指针	
	//返回: 
	//		如果加载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	BOOL Load(IModuleManager* pManager) ;

	//----------------------------------------------------------------------------------------
	//名称: Unload
	//描述: 主程序通过该方法对模块进行卸载
	//返回: 
	//		如果卸载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	BOOL Unload() ;

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

	// 响应任务栏图标上的事件
	virtual void OnTrayEvent(WPARAM w, LPARAM l);

	// 任务栏图标开始闪烁
	virtual void OnFlashStart(DWORD dwParam);

	// 任务栏图标结束闪烁
	virtual void OnFlashEnd(DWORD dwParam);

	// Event处理函数
protected:

	void	OnEvent_ShowTrayIcon(Event* pEvent);	
public:

	void    OnEvent_ShowAboutDialog(Event *pEvent);

protected:

	void	OnMessage_Show(Message* pMessage);
	// 程序退出的广播消息，主界面处理该消息，为退出做准备
	void	OnMessage_PreExit(Message* pMessage);		


private:
	void	CreatTrayIconMsgWnd();
	void DestroyTrayIconMsgWnd();

private:

	CTrayMgr		m_TrayMgr;
	HWND			m_hMsgWnd;		 //	内部的隐藏窗口，用来接收菜单的消息
};

class CTrayIconModuleFactory : public ModuleFactoryImpl<TrayIconModule>{};

namespace trayicon
{
	extern TrayIconModule*	g_TrayIconModule;
	extern CTrayIconModuleFactory*	g_TrayIconModuleFactory;
}