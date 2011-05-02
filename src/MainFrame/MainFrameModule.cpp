#include "MainFrameModule.h"
#include "MainFrameDefine.h"

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

}

BEGIN_EVENT_MAP(MainFrameModule)
	ON_EVENT(EVENT_VALUE_MAINFRAME_OPEN, OnEvent_OpenMainDlg)
	ON_EVENT(EVENT_VALUE_MAINFRAME_CLOSE, OnEvent_CloseMainDlg)
	ON_EVENT(EVENT_VALUE_MAINFRAME_HIDE, OnEvent_HideMainDlg)
END_EVENT_MAP()

BEGIN_MESSAGE_MAP(MainFrameModule)
	ON_MESSAGE(MESSAGE_VALUE_SYS_CYCLE_TRIGGER, OnMessage_CycleTrigged)
	ON_MESSAGE(MESSAGE_VALUE_EXIT, OnMessage_Exit)
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
	EventValue ev = evt.eventValue;
	ASSERT( ev != EVENT_VALUE_INVALID);

	const EventHandlerMapEntries* pEntry = GetThisEventMap();
	while( pEntry)
	{
		if(  pEntry->nEventValue != EVENT_VALUE_INVALID ||
			pEntry->nEventValue == 0)
			break;

		if( pEntry->nEventValue == ev
			&& pEntry->pfEventHandler != NULL)
		{
			(this->*pEntry->pfEventHandler)(&evt);
			return;
		}

		++pEntry;
	}

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
	MessageValue mv = msg.messageValue;
	ASSERT( mv != MESSAGE_VALUE_INVALID);

	const MessageHandlerMapEntries* pEntry = GetThisMessageMap();
	while( pEntry)
	{
		if( pEntry->nMessageValue == MESSAGE_VALUE_INVALID
			|| pEntry->nMessageValue == 0)
			break;

		if( pEntry->nMessageValue == mv
			&& pEntry->pfMessageHandler != NULL)
		{
			(this->*pEntry->pfMessageHandler)(&msg);
			return;
		}

		++pEntry;
	}

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
	ServiceValue event_value = (ServiceValue)lServiceValue;
	
/*
	EventHandlerTableEntry* pEntry = &MainFrameModule::m_eventTableDriven[0];
	while( pEntry)
	{
		if( pEntry->m_nEventValue == 0)
			break;

		if( pEntry->m_nEventValue == event_value
			&& pEntry->m_hHandler != NULL)
		{
			return (*pEntry->m_hHandler)(this, pEvent);
		}

		++pEntry;
	}
	
*/

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
void MainFrameModule::PaybackExtraInfo(uint32 valudId, void* pExtraInfo)
{
	return;
}

void	MainFrameModule::OnEvent_OpenMainDlg(Event* pEvent)
{
}

void	MainFrameModule::OnEvent_CloseMainDlg(Event* pEvent)
{

}

void	MainFrameModule::OnEvent_HideMainDlg(Event* pEvent)
{

}

void	MainFrameModule::OnMessage_Exit(Message* pMessage)
{

}

void	MainFrameModule::OnMessage_CycleTrigged(Message* pMessage)
{

}


void	MainFrameModule::OnService_Test(ServiceValue lServiceValue, param lParam)
{

}
