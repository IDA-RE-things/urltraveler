#include "stdafx.h"
#include "SettingDefine.h"
#include "MainFrameDefine.h"
#include "SettingModule.h"
#include "OptionCenter.h"

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

BEGIN_SERVICE_MAP(SettingModule)
	ON_SERVICE(SERVICE_VALUE_SETTING_GET_AUTO_START, OnService_GetAutoStartWithWindow)
	ON_SERVICE(SERVICE_VALUE_SETTING_GET_REMEMBER_PWD, OnService_GetRememberPassword)
	ON_SERVICE(SERVICE_VALUE_SETTING_GET_AUTO_LOGIN, OnService_GetAutoLogin)
	ON_SERVICE(SERVICE_VALUE_SETTING_GET_CLOSE_WND_WHEN_EXIT, OnService_GetExitWhenCloseWnd)
	ON_SERVICE(SERVICE_VALUE_SETTING_GET_LOCAL_SYNC, OnService_GetAutoLocalSync)
	ON_SERVICE(SERVICE_VALUE_SETTING_GET_REMOTE_SYNC, OnService_GetAutoRemoteSync)
	ON_SERVICE(SERVICE_VALUE_SETTING_GET_UPDATE_TYPE, OnService_GetUpdateType)
END_SERVICE_MAP();

//----------------------------------------------------------------------------------------
//名称: Load
//描述: 主程序通过该方法对模块进行加载
//参数: 
//		@param	pManager			主模块总线的指针	
//返回: 
//		如果加载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
BOOL SettingModule::Load(IModuleManager* pManager)
{
	__super::Load(pManager);

	m_OptionCenter.LoadSetting();
	return TRUE;
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
int32 SettingModule::CallDirect(const param lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void	SettingModule::OnEvent_OpenSettingWnd(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_SETTING_OPEN)
		return;

	Setting_OpenEvent* pOpenEvent = (Setting_OpenEvent*)pEvent->m_pstExtraInfo;

	mainframe::MainFrame_GetWndService stGetWnd;
	m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWnd);

	CWindowWnd* pParentWnd = reinterpret_cast<CWindowWnd*>(stGetWnd.pBaseWnd);
	ASSERT(pParentWnd != NULL);

	CSettingWnd* pSettingWnd = new CSettingWnd();
	pSettingWnd->Create(*pParentWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);	
	pSettingWnd->CenterWindow();
	pParentWnd->ShowModal(pSettingWnd->GetHWND());
}

int	SettingModule::OnService_GetAutoStartWithWindow(ServiceValue lServiceValue, param	lParam)
{
	Setting_GetStartWithWindowService* pService = (Setting_GetStartWithWindowService*)lParam;
	pService->bStart = OPTION_CENTER.m_bAutoStart;
	return -1;
}

int	SettingModule::OnService_GetRememberPassword(ServiceValue lServiceValue, param	lParam)
{
	Setting_GetRememberPwdService* pService = (Setting_GetRememberPwdService*)lParam;
	return -1;
}

int	SettingModule::OnService_GetAutoLogin(ServiceValue lServiceValue, param	lParam)
{
	Setting_GetAutoLoginService* pService = (Setting_GetAutoLoginService*)lParam;
	return -1;
}

int	SettingModule::OnService_GetExitWhenCloseWnd(ServiceValue lServiceValue, param	lParam)
{
	Setting_GetExitWhenCloseWndService* pService = (Setting_GetExitWhenCloseWndService*)lParam;
	pService->bExit = !OPTION_CENTER.m_bMinToTray;
	return -1;
}

int	SettingModule::OnService_GetAutoLocalSync(ServiceValue lServiceValue, param	lParam)
{
	Setting_GetAutoLocalSyncService* pService = (Setting_GetAutoLocalSyncService*)lParam;
	return -1;
}

int	SettingModule::OnService_GetAutoRemoteSync(ServiceValue lServiceValue, param	lParam)
{
	Setting_GetAutoRemoteSyncService* pService = (Setting_GetAutoRemoteSyncService*)lParam;
	return -1;
}

int	SettingModule::OnService_GetUpdateType(ServiceValue lServiceValue, param	lParam)
{
	Setting_GetUpateTypeService* pService = (Setting_GetUpateTypeService*)lParam;
	return -1;
}