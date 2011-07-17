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

}

FavMonitorModule::~FavMonitorModule()
{

}

BEGIN_EVENT_MAP(FavMonitorModule)
	//ON_EVENT(EVENT_VALUE_FAVMONITOR_MONITOR_FILE, OnEvent_MonitorFile)
END_EVENT_MAP()

BEGIN_MESSAGE_MAP(FavMonitorModule)
END_MESSAGE_MAP()

BEGIN_SERVICE_MAP(FavMonitorModule)
	ON_SERVICE(SERVICE_VALUE_FAVMONITOR_ADD_MONITOR, OnService_AddMonitor)
	ON_SERVICE(SERVICE_VALUE_FAVMONITOR_REMOVE_MONITOR, OnService_RemoveMonitor)
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

	pAddMonitorService->hMonitorHandle = MonFile_Start((LPSTR)StringHelper::UnicodeToANSI(pAddMonitorService->szFile).c_str(),
		false,
		0,
		FavMonitorModule::NotifyRotuine);

	return 0;
}

int32 FavMonitorModule::OnService_RemoveMonitor( ServiceValue lServiceValue, param lParam )
{
	FavMonitor_RemoveMonitorService *pRemoveMonitorService = (FavMonitor_RemoveMonitorService *)lParam;

	if (pRemoveMonitorService->hMonitorHandle != NULL)
	{
		MonFile_Stop(pRemoveMonitorService->hMonitorHandle);
	}

	return 0;
}
