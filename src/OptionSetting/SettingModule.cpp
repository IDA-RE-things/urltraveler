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
//����: Load
//����: ������ͨ���÷�����ģ����м���
//����: 
//		@param	pManager			��ģ�����ߵ�ָ��	
//����: 
//		������سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL SettingModule::Load(IModuleManager* pManager)
{
	__super::Load(pManager);

	m_OptionCenter.LoadSetting();
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* SettingModule::GetModuleName() 
{
	return L"SettingModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const SettingModule::GetModuleId()
{
	return MODULE_ID_SETTING;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void SettingModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//����: ProcessMessage
//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
//����: 
//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
//----------------------------------------------------------------------------------------
void SettingModule::ProcessMessage(const Message& msg) 
{
}

//----------------------------------------------------------------------------------------
//����: CallDirect
//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
//			����������Ҫͨ�����ߡ�
//����: 
//		@param	lparam			����1
//		@param	rparam			����2
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