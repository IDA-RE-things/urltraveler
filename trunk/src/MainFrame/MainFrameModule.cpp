#include "MainFrameModule.h"

namespace mainframe
{
	MainFrameModule*	g_MainFrameModule = NULL;
	CMainFrameModuleFactory*	g_MainFrameModuleFactory = NULL;
}

using namespace mainframe;

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

//----------------------------------------------------------------------------------------
//����: Load
//����: ������ͨ���÷�����ģ����м���
//����: 
//		@param	pManager			��ģ�����ߵ�ָ��	
//����: 
//		������سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL MainFrameModule::Load(IModuleManager* pManager)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: Unload
//����: ������ͨ���÷�����ģ�����ж��
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL MainFrameModule::Unload() 
{
	return TRUE;
}

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
}

//----------------------------------------------------------------------------------------
//����: CallDirect
//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
//			����������Ҫͨ�����ߡ�
//����: 
//		@param	lparam			����1
//		@param	rparam			����2
//----------------------------------------------------------------------------------------
int32 MainFrameModule::CallDirect(const param lparam, param wparam) 
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
void MainFrameModule::PaybackExtraInfo(uint32 valudId, void* pExtraInfo)
{
	return;
}