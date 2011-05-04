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

// �������ʵ��
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
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* MainFrameModule::GetModuleName() 
{
	return L"MainFrameModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const MainFrameModule::GetModuleId()
{
	return MODULE_ID_MAINFRAME;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void MainFrameModule::ProcessEvent(const Event& evt)
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
void MainFrameModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
}

//----------------------------------------------------------------------------------------
//����: CallDirect
//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
//			����������Ҫͨ�����ߡ�
//����: 
//		@param	lServiceValue		����1
//		@param	rparam			����2
//----------------------------------------------------------------------------------------
int32 MainFrameModule::CallDirect(const ServiceValue lServiceValue, param rparam) 
{	
	CALL_DIRECT(lServiceValue, rparam);
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
