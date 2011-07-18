#include "stdafx.h"
#include "TrayIconModule.h"
#include "TrayIconDefine.h"
#include "resource.h"
#include <sstream>
#include "AboutDlg.h"
#include "MainFrameDefine.h"
#include "SettingDefine.h"

HMODULE	g_hModule = NULL;

using namespace trayicon;
using namespace setting;

namespace trayicon
{
	TrayIconModule*	g_TrayIconModule = NULL;
	TrayIconModuleFactory*	g_TrayIconModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(TrayIconModule)
EXPORT_RELEASEMODULEFACTORY(TrayIconModule)

TrayIconModule::TrayIconModule()
{

}

TrayIconModule::~TrayIconModule()
{

}

BEGIN_EVENT_MAP(TrayIconModule)
	ON_EVENT(EVENT_VALUE_TRAYICON_SHOW,OnEvent_ShowTrayIcon)
	ON_EVENT(EVENT_VALUE_TRAYICON_SHOW_UPDATE_WND, OnEvent_ShowUpdateWnd)
END_EVENT_MAP()

BEGIN_MESSAGE_MAP(TrayIconModule)
	ON_MESSAGE(MESSAGE_VALUE_CORE_BEGIN_SHOW, OnMessage_Show)
	ON_MESSAGE(MESSAGE_VALUE_CORE_PRE_APP_EXIT, OnMessage_PreExit)
END_MESSAGE_MAP()

static LRESULT CALLBACK AppCycleProc(HWND inWindow, UINT inMsg, WPARAM wParam, LPARAM lParam)
{
	TrayIconModule * pTrayIconModule = (TrayIconModule*)GetWindowLong(inWindow, GWL_USERDATA);
	if(pTrayIconModule)
	{
		if(inMsg == WM_COMMAND)
		{
			int wmId    = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);

			switch (wmId)
			{
			case IDM_APP_ABOUT:
				pTrayIconModule->OnEvent_ShowAboutDialog(NULL);
				break;

			case IDM_SETTING:
				{
					setting::Setting_OpenEvent* pEvent = new setting::Setting_OpenEvent();
					pEvent->srcMId = MODULE_ID_MAINFRAME;
					pTrayIconModule->GetModuleManager()->PushEvent(*pEvent);

					break;
				}

			case IDM_APP_EXIT:
				{
					// ֪ͨ����ģ���˳�֮ǰ���б�Ҫ��׼������
					pTrayIconModule->GetModuleManager()->PushMessage(
						MakeMessage<MODULE_ID_TRAYICON>()(MESSAGE_VALUE_CORE_PRE_APP_EXIT));

					// ֪ͨ��ʽ�˳�
/*
					Event e;
					e.eventValue = EVENT_VALUE_CORE_MAIN_LOOP_EXIT;
					e.srcMId = MODULE_ID_TRAYICON;
					e.desMId = MODULE_ID_CORE;
					pTrayIconModule->GetModuleManager()->PushEvent(e);
*/

					pTrayIconModule->GetModuleManager()->PushEvent(
						MakeEvent<MODULE_ID_TRAYICON>()(EVENT_VALUE_CORE_MAIN_LOOP_EXIT, 
							MODULE_ID_CORE));
					break;
				}

			case IDM_SHOW_MAIN:
				{
					// ֪ͨ����ģ���˳�֮ǰ���б�Ҫ��׼������
					pTrayIconModule->GetModuleManager()->PushEvent(
						MakeEvent<MODULE_ID_TRAYICON>()(mainframe::EVENT_VALUE_MAINFRAME_SHOW, 
							MODULE_ID_MAINFRAME));

					break;
				}

			default:
				return DefWindowProc(inWindow, inMsg, wParam, lParam);
			}

			return 1 ;
		}

		return DefWindowProc(inWindow, inMsg, wParam, lParam);
	}

	return DefWindowProcW(inWindow, inMsg, wParam, lParam);
}

// �����������ڲ�����
void TrayIconModule::CreatTrayIconMsgWnd()
{
	std::wstring className = L"UrlTravelerApp";

	WNDCLASSEX theWndClass;
	theWndClass.cbSize = sizeof(theWndClass);
	theWndClass.style = 0;
	theWndClass.lpfnWndProc = &AppCycleProc;
	theWndClass.cbClsExtra = 0;
	theWndClass.cbWndExtra = 0;
	theWndClass.hInstance = NULL;
	theWndClass.hIcon = NULL;
	theWndClass.hCursor = NULL;
	theWndClass.hbrBackground = NULL;
	theWndClass.lpszMenuName = NULL;
	theWndClass.lpszClassName = className.c_str();
	theWndClass.hIconSm = NULL;
	ATOM theWndAtom = ::RegisterClassEx(&theWndClass);
	ASSERT(theWndAtom != NULL);

	m_hMsgWnd = ::CreateWindow( className.c_str(), className.c_str(), 
		WS_POPUP, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, NULL, NULL);
	ASSERT(m_hMsgWnd != NULL);

	SetWindowLongW(m_hMsgWnd, GWL_USERDATA, (LONG)this);
}

void TrayIconModule::DestroyTrayIconMsgWnd()
{
	::DestroyWindow(m_hMsgWnd);

	std::wstring className = L"UrlTravelerApp";
	UnregisterClass(className.c_str(), NULL );
}

//----------------------------------------------------------------------------------------
//����: Load
//����: ������ͨ���÷�����ģ����м���
//����: 
//		@param	pManager			��ģ�����ߵ�ָ��	
//����: 
//		������سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL TrayIconModule::Load(IModuleManager* pManager)
{
	__super::Load(pManager);
	
	CreatTrayIconMsgWnd();
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: Unload
//����: ������ͨ���÷�����ģ�����ж��
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL TrayIconModule::Unload()
{
	DestroyTrayIconMsgWnd();
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* TrayIconModule::GetModuleName() 
{
	return L"TrayIconModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const TrayIconModule::GetModuleId()
{
	return MODULE_ID_MAINFRAME;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void TrayIconModule::ProcessEvent(const Event& evt)
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
void TrayIconModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
}

//----------------------------------------------------------------------------------------
//����: CallDirect
//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
//			����������Ҫͨ�����ߡ�
//����: 
//		@param	lparam			����1
//		@param	rparam			����2
//----------------------------------------------------------------------------------------
int32 TrayIconModule::CallDirect(const param lparam, param wparam) 
{
	return -1;
}

void TrayIconModule::OnTrayEvent(WPARAM w, LPARAM l)
{
	uint32 uMsgId = l;
	switch (uMsgId)
	{
	case WM_RBUTTONUP:
		{
			HMENU	hPopMenu;
			hPopMenu = ::LoadMenuW(g_hModule, MAKEINTRESOURCE(IDR_APPMENU)); 
			hPopMenu = ::GetSubMenu(hPopMenu, 0);

			POINT pt;
			GetCursorPos(&pt);
			::TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x,pt.y,0, m_hMsgWnd, NULL);
		}
		break;

	}
}

void TrayIconModule::OnFlashStart(DWORD dwParam)
{

}

void TrayIconModule::OnFlashEnd(DWORD dwParam)
{

}

void TrayIconModule::OnEvent_ShowTrayIcon(Event* pEvent)
{
	m_TrayMgr.Init();
	
	HICON hIcon = LoadIconW(g_hModule, MAKEINTRESOURCE(IDI_URLUPLOAD));
	m_TrayMgr.Add(hIcon, L"UrlTraveler");
	m_TrayMgr.AddEventHandler(this);
}

void TrayIconModule::OnEvent_ShowAboutDialog( Event *pEvent )
{
	mainframe::MainFrame_GetWndService stGetWnd;
	m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWnd);

	CWindowWnd* pParentWnd = reinterpret_cast<CWindowWnd*>(stGetWnd.pBaseWnd);
	ASSERT(pParentWnd != NULL);

	CAboutDlg dlg;
	dlg.Create(*pParentWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);	
	dlg.CenterWindow();
	pParentWnd->ShowModal(dlg);
}

void TrayIconModule::OnEvent_ShowUpdateWnd(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue == EVENT_VALUE_TRAYICON_SHOW_UPDATE_WND)
		return;

	TrayIcon_ShowUpdateWndEvent* pShowEvent = (TrayIcon_ShowUpdateWndEvent*)pEvent->m_pstExtraInfo;

}

void TrayIconModule::OnMessage_Show(Message* pMessage)
{
	m_TrayMgr.Init();
	
	HICON hIcon = LoadIconW(g_hModule, MAKEINTRESOURCE(IDI_URLUPLOAD));
	m_TrayMgr.Add(hIcon, L"UrlTraveler");
	m_TrayMgr.AddEventHandler(this);
}

void TrayIconModule::OnMessage_PreExit(Message* pMessage)
{

}