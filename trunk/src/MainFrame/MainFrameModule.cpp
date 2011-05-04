#include "stdafx.h"
#include "MainFrameModule.h"
#include "MainFrameDefine.h"
#include "TrayIconDefine.h"
#include "UIDemo.h"

using namespace mainframe;

namespace mainframe
{
	MainFrameModule*	g_MainFrameModule = NULL;
	CMainFrameModuleFactory*	g_MainFrameModuleFactory = NULL;
}

// 导出借口实现
IModuleFactory*	GetModuleFactory()
{
	if( g_MainFrameModuleFactory == NULL)
	{
		g_MainFrameModuleFactory = new CMainFrameModuleFactory();
		g_MainFrameModuleFactory->QueryModulePoint(1, (IModule*&)g_MainFrameModule);

		ASSERT( g_MainFrameModule != NULL);
	}

	return g_MainFrameModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_MainFrameModuleFactory == p);
	if( g_MainFrameModuleFactory  != NULL)
	{
		delete g_MainFrameModuleFactory;
		g_MainFrameModuleFactory = NULL;
	}
}

MainFrameModule::MainFrameModule()
{

}

MainFrameModule::~MainFrameModule()
{
	::CoUninitialize();
}

BEGIN_EVENT_MAP(MainFrameModule)
	ON_EVENT(EVENT_VALUE_MAINFRAME_OPEN, OnEvent_OpenMainDlg)
	ON_EVENT(EVENT_VALUE_MAINFRAME_CLOSE, OnEvent_CloseMainDlg)
	ON_EVENT(EVENT_VALUE_MAINFRAME_HIDE, OnEvent_HideMainDlg)
END_EVENT_MAP()

BEGIN_MESSAGE_MAP(MainFrameModule)
	ON_MESSAGE(MESSAGE_VALUE_CORE_BEGIN_SHOW, OnMessage_Show)
	ON_MESSAGE(MESSAGE_VALUE_CORE_CYCLE_TRIGGER, OnMessage_CycleTrigged)
	ON_MESSAGE(MESSAGE_VALUE_CORE_PRE_APP_EXIT, OnMessage_PreExit)
END_MESSAGE_MAP()

BEGIN_SERVICE_MAP(MainFrameModule)
	ON_SERVICE(SERVICE_VALUE_TEST, OnService_Test)
END_SERVICE_MAP();

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* MainFrameModule::GetModuleName() 
{
	return L"MainFrameModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const MainFrameModule::GetModuleId()
{
	return MODULE_ID_MAINFRAME;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void MainFrameModule::ProcessEvent(const Event& evt)
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
void MainFrameModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lServiceValue		参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 MainFrameModule::CallDirect(const ServiceValue lServiceValue, param rparam) 
{	
	CALL_DIRECT(lServiceValue, rparam);
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
void MainFrameModule::PaybackExtraInfo(uint32 valueId, void* pExtraInfo)
{
	if( valueId == EVENT_VALUE_MAINFRAME_OPEN)
	{
		delete (MainFrame_OpenEvent*)pExtraInfo;
	}

	return;
}

void	MainFrameModule::OnEvent_OpenMainDlg(Event* pEvent)
{
	MainFrame_OpenEvent* pE = (MainFrame_OpenEvent*)pEvent->m_pstExtraInfo;

   
	return;
}

void	MainFrameModule::OnEvent_CloseMainDlg(Event* pEvent)
{

}

void	MainFrameModule::OnEvent_HideMainDlg(Event* pEvent)
{

}

void MainFrameModule::OnMessage_Show(Message* pMessage)
{
	CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("\\skin\\FlashRes"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return;

	CFrameWnd* pFrame = new CFrameWnd();
	if( pFrame == NULL ) return;
	pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);
	//CPaintManagerUI::MessageLoop();

}

void	MainFrameModule::OnMessage_PreExit(Message* pMessage)
{
}

void	MainFrameModule::OnMessage_CycleTrigged(Message* pMessage)
{
}

int32 MainFrameModule::OnService_Test(ServiceValue lServiceValue, param lParam)
{
	return -1;
}
