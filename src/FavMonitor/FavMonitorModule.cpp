#include "stdafx.h"
#include "FavMonitorModule.h"
#include "FavMonitorDefine.h"
#include "Filemon.h"
#include "StringHelper.h"
#include <sstream>

HMODULE	g_hModule = NULL;

using namespace favmonitor;

namespace favmonitor
{
	FavMonitorModule*	g_FavMonitorModule = NULL;
	FavMonitorModuleFactory*	g_FavMonitorModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(FavMonitorModule)
EXPORT_RELEASEMODULEFACTORY(FavMonitorModule)

FavMonitorModule::FavMonitorModule()
{
	m_monitorHandle = NULL;
}

FavMonitorModule::~FavMonitorModule()
{
	CloseMonitor(m_monitorHandle);
}

BEGIN_EVENT_MAP(FavMonitorModule)
	ON_EVENT(EVENT_VALUE_FAVMONITOR_START_MONITOR, OnEvent_StartMonitor)
	ON_EVENT(EVENT_VALUE_FAVMONITOR_STOP_MONITOR, OnEvent_StopMonitor)
END_EVENT_MAP()




BEGIN_MESSAGE_MAP(FavMonitorModule)
END_MESSAGE_MAP()

BEGIN_SERVICE_MAP(FavMonitorModule)
	ON_SERVICE(SERVICE_VALUE_FAVMONITOR_ADD_MONITOR, OnService_AddMonitor)
	ON_SERVICE(SERVICE_VALUE_FAVMONITOR_START_MONITOR, OnService_StartMonitor)
	ON_SERVICE(SERVICE_VALUE_FAVMONITOR_STOP_MONITOR, OnService_StopMonitor)
	ON_SERVICE(SERVICE_VALUE_FAVMONITOR_SUSPEND_MONITOR, OnService_SuspendMonitor)
END_SERVICE_MAP();

//----------------------------------------------------------------------------------------
//����: Load
//����: ������ͨ���÷�����ģ����м���
//����: 
//		@param	pManager			��ģ�����ߵ�ָ��	
//����: 
//		������سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL FavMonitorModule::Load(IModuleManager* pManager)
{
	__super::Load(pManager);

	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: Unload
//����: ������ͨ���÷�����ģ�����ж��
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL FavMonitorModule::Unload()
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* FavMonitorModule::GetModuleName() 
{
	return L"FavMonitorModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const FavMonitorModule::GetModuleId()
{
	return MODULE_ID_FAVMONITOR;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void FavMonitorModule::ProcessEvent(const Event& evt)
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
void FavMonitorModule::ProcessMessage(const Message& msg) 
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
int32 FavMonitorModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void	FavMonitorModule::OnEvent_StartMonitor(Event* pEvent)
{
}

void	FavMonitorModule::OnEvent_StopMonitor(Event* pEvent)
{
}

void FavMonitorModule::NotifyRotuine( LPSTR pPath,int iActionType )
{
	FavMonitor_FileChangeMessage *pFileChangeMessage = new FavMonitor_FileChangeMessage;

	wcscpy_s(pFileChangeMessage->szFile, MAX_PATH - 1, StringHelper::ANSIToUnicode(pPath).c_str());

	pFileChangeMessage->iActionType = iActionType;
	g_FavMonitorModule->GetModuleManager()->PushMessage(*pFileChangeMessage);
}

int32 FavMonitorModule::OnService_AddMonitor( ServiceValue lServiceValue, param lParam )
{
	FavMonitor_AddMonitorService *pAddMonitorService = (FavMonitor_AddMonitorService *)lParam;

	if (m_monitorHandle == NULL)
	{
		m_monitorHandle = CreateMonitor();
	}

	AddMonitor(m_monitorHandle,
		(LPSTR)StringHelper::UnicodeToANSI(pAddMonitorService->szFile).c_str(),
		false, 
		0, 
		FavMonitorModule::NotifyRotuine);

	return 0;
}

int32 FavMonitorModule::OnService_StartMonitor( ServiceValue lServiceValue, param lParam )
{
	if (m_monitorHandle == NULL)
	{
		return -1;
	}

	StartMonitor(m_monitorHandle);

	return 0;
}


int32 FavMonitorModule::OnService_StopMonitor( ServiceValue lServiceValue, param lParam )
{
	if (m_monitorHandle == NULL)
	{
		return -1;
	}

	StopMonitor(m_monitorHandle);

	return 0;
}

int32 FavMonitorModule::OnService_SuspendMonitor( ServiceValue lServiceValue, param lParam )
{
	if (m_monitorHandle == NULL)
	{
		return -1;
	}

	SuspendMonitor(m_monitorHandle);

	return 0;
}