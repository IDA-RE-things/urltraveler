#include "TrayIconModule.h"
#include "TrayIconDefine.h"
#include "resource.h"

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

void	TrayIconModule::OnEvent_ShowTrayIcon(Event* pEvent)
{
	m_TrayMgr.Init();
	
	HICON hIcon = LoadIconW(g_hModule, MAKEINTRESOURCE(IDI_URLTRAVELER));
	m_TrayMgr.Add(hIcon, L"UrlTraveler");
}