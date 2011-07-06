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
int32 SettingModule::CallDirect(const param lparam, param wparam) 
{
	return -1;
}

void	SettingModule::OnEvent_OpenSettingWnd(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_SETTING_OPEN)
		return;

	Setting_OpenEvent* pOpenEvent = (Setting_OpenEvent*)pEvent->m_pstExtraInfo;

			// ǿ�Ƶ������´��ڽ�������
		mainframe::MainFrame_GetWndService stGetWndService;
		m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWndService);

		CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
		ASSERT(pMainFrameWnd != NULL);

		// ��������������
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