#include "stdafx.h"
#include "SettingDefine.h"
#include "MainFrameDefine.h"
#include "SettingModule.h"

using namespace setting;
using namespace mainframe;

HMODULE	g_hModule = NULL;

namespace setting
{
	SettingModule*	g_SettingModule = NULL;
	SettingModuleFactory*	g_SettingModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(SettingModule)
EXPORT_RELEASEMODULEFACTORY(SettingModule)

SettingModule::SettingModule()
{
	m_pSettingWnd	=	NULL;
}

SettingModule::~SettingModule()
{
	if( m_pSettingWnd)
	{
		m_pSettingWnd->SendMessage(WM_CLOSE, 0, 0);
	}
}

BEGIN_EVENT_MAP(SettingModule)
	ON_EVENT(EVENT_VALUE_SETTING_OPEN, OnEvent_OpenSettingWnd)
END_EVENT_MAP(SettingModule)

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
	PROCESS_EVENT(evt);
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

void	SettingModule::OnEvent_OpenSettingWnd(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_SETTING_OPEN)
		return;

	Setting_OpenEvent* pOpenEvent = (Setting_OpenEvent*)pEvent->m_pstExtraInfo;

			// 强制弹出更新窗口进行升级
		mainframe::MainFrame_GetWndService stGetWndService;
		m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWndService);

		CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
		ASSERT(pMainFrameWnd != NULL);

		// 启动进度条界面
		if( m_pSettingWnd == NULL)
			m_pSettingWnd = new CSettingWnd();

		if( m_pSettingWnd != NULL )
		{ 
			if( m_pSettingWnd->GetHWND() == NULL)
			{
				m_pSettingWnd->Create(*pMainFrameWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
			}
			m_pSettingWnd->CenterWindow();
			pMainFrameWnd->ShowModal(*m_pSettingWnd);
		}

}