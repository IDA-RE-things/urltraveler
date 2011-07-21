#include "stdafx.h"
#include "UIlib.h"
#include "LoginModule.h"
#include "MiscHelper.h"
#include "LoginDefine.h"
#include "WebDefine.h"
#include "ImageHelper.h"
#include "TxConfig.h"
#include "MiscHelper.h"
#include "StringHelper.h"
#include "MainFrameDefine.h"

using namespace login;
using namespace mainframe;

namespace login
{
	LoginModule*	g_LoginModule = NULL;
	LoginModuleFactory*	g_LoginModuleFactory = NULL;
}

// 导出借口实现
EXPORT_GETMODULEFACTORY(LoginModule)
EXPORT_RELEASEMODULEFACTORY(LoginModule)

LoginModule::LoginModule()
{

}

LoginModule::~LoginModule()
{

}

BEGIN_EVENT_MAP(LoginModule)
	ON_EVENT(EVENT_VALUE_LOGIN_OPEN, OnEvent_OpenLoginDlg)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(LoginModule)
END_SERVICE_MAP()

BEGIN_MESSAGE_MAP(LoginModule)
END_MESSAGE_MAP()

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* LoginModule::GetModuleName() 
{
	return L"LoginModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const LoginModule::GetModuleId()
{
	return MODULE_ID_LOGIN;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void LoginModule::ProcessEvent(const Event& evt)
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
void LoginModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lparam			参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 LoginModule::CallDirect(const param lparam, param wparam) 
{
	CALL_DIRECT(lparam, wparam);
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
void LoginModule::PaybackExtraInfo(uint32 valudId, void* pExtraInfo)
{
	return;
}

void	LoginModule::OnEvent_OpenLoginDlg(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_LOGIN_OPEN)
		return;

	// 强制弹出更新窗口进行升级
	mainframe::MainFrame_GetWndService stGetWndService;
	m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWndService);

	CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
	ASSERT(pMainFrameWnd != NULL);
	pMainFrameWnd->ShowWindow(SW_NORMAL);

	CLoginPreWnd* pLoginFrame = new CLoginPreWnd();
	if( pLoginFrame == NULL )
		return ;

	pLoginFrame->Create(pMainFrameWnd->GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
	pLoginFrame->CenterWindow();
	pMainFrameWnd->ShowModal(*pLoginFrame);
}
