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

// 导出借口实现
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

// 创建和销毁内部窗口
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
//名称: Load
//描述: 主程序通过该方法对模块进行加载
//参数: 
//		@param	pManager			主模块总线的指针	
//返回: 
//		如果加载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
BOOL TrayIconModule::Load(IModuleManager* pManager)
{
	__super::Load(pManager);
	
	CreatTrayIconMsgWnd();
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: Unload
//描述: 主程序通过该方法对模块进行卸载
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
BOOL TrayIconModule::Unload()
{
	DestroyTrayIconMsgWnd();
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* TrayIconModule::GetModuleName() 
{
	return L"TrayIconModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const TrayIconModule::GetModuleId()
{
	return MODULE_ID_MAINFRAME;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void TrayIconModule::ProcessEvent(const Event& evt)
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
void TrayIconModule::ProcessMessage(const Message& msg) 
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
int32 TrayIconModule::CallDirect(const param lparam, param wparam) 
{
	return -1;
}

//----------------------------------------------------------------------------------------
//名称: PaybackExtraInfo
//描述: 某个模块如果有自定义的复杂的数据需要传给其余的模块，那么它可以构造一个ExtraInfo结构
//	在其余的模块使用完毕后，该结构必须被释放，否则会造成内存泄露。释放必须由模块自身完成。
//	某个模块都必须提供一个PaybackExtraInfo接口，释放自己的自定义的数据类型
//参数: 
//		@param	valudId			对应的pExtraInfo的值，内部根据该值进行对应的释放，该值只有模块自己理解
//		@param	pExtraInfo	需要释放的ExtraInfo数据
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