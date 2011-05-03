#include "stdafx.h"
#include "TrayIconModule.h"
#include "TrayIconDefine.h"
#include "resource.h"
#include <sstream>


HMODULE	g_hModule = NULL;


using namespace trayicon;

namespace trayicon
{
	TrayIconModule*	g_TrayIconModule = NULL;
	CTrayIconModuleFactory*	g_TrayIconModuleFactory = NULL;
}

// �������ʵ��
IModuleFactory*	GetModuleFactory()
{
	if( g_TrayIconModuleFactory == NULL)
	{
		g_TrayIconModuleFactory = new CTrayIconModuleFactory();
		g_TrayIconModuleFactory->QueryModulePoint(1, (IModule*&)g_TrayIconModule);

		ASSERT( g_TrayIconModule != NULL);
	}

	return g_TrayIconModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_TrayIconModuleFactory == p);
	if( g_TrayIconModuleFactory  != NULL)
	{
		delete g_TrayIconModuleFactory;
		g_TrayIconModuleFactory = NULL;
	}
}

TrayIconModule::TrayIconModule()
{

}

TrayIconModule::~TrayIconModule()
{

}

BEGIN_EVENT_MAP(TrayIconModule)
	ON_EVENT(EVENT_VALUE_TRAYICON_SHOW,OnEvent_ShowTrayIcon)
END_EVENT_MAP()

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
				MessageBox(NULL, L"About", L"About", MB_OK);
				break;

			case IDM_APP_EXIT:
				{
					pTrayIconModule->GetModuleManager()->PushMessage(
						MakeMessage<MODULE_ID_TRAYICON>()(MESSAGE_VALUE_APP_EXIT));
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

//----------------------------------------------------------------------------------------
//����: PaybackExtraInfo
//����: ĳ��ģ��������Զ���ĸ��ӵ�������Ҫ���������ģ�飬��ô�����Թ���һ��ExtraInfo�ṹ
//	�������ģ��ʹ����Ϻ󣬸ýṹ���뱻�ͷţ����������ڴ�й¶���ͷű�����ģ��������ɡ�
//	ĳ��ģ�鶼�����ṩһ��PaybackExtraInfo�ӿڣ��ͷ��Լ����Զ������������
//����: 
//		@param	valudId			��Ӧ��pExtraInfo��ֵ���ڲ����ݸ�ֵ���ж�Ӧ���ͷţ���ֵֻ��ģ���Լ����
//		@param	pExtraInfo	��Ҫ�ͷŵ�ExtraInfo����
//----------------------------------------------------------------------------------------
void TrayIconModule::PaybackExtraInfo(uint32 valudId, void* pExtraInfo)
{

	return;
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

void	TrayIconModule::OnEvent_ShowTrayIcon(Event* pEvent)
{
	m_TrayMgr.Init();
	
	HICON hIcon = LoadIconW(g_hModule, MAKEINTRESOURCE(IDI_URLTRAVELER));
	m_TrayMgr.Add(hIcon, L"UrlTraveler");
	m_TrayMgr.AddEventHandler(this);
}