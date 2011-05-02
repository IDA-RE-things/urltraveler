#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImp.h"

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}


class MainFrameModule : public ModuleImpl
{
	DECLEAR_EVENT_MAP()

	DECLEAR_MESSAGE_MAP()
	DECLEAR_SERVICE_MAP()

public:
	MainFrameModule();
	~MainFrameModule();

	//----------------------------------------------------------------------------------------
	//����: GetModuleName
	//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	const wchar_t* GetModuleName();

 	//----------------------------------------------------------------------------------------
	//����: GetModuleId
	//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
	//����: 
	//		���ظ�ģ���Ψһ��ID
	//----------------------------------------------------------------------------------------
	uint32 const GetModuleId();

 	//----------------------------------------------------------------------------------------
	//����: ProcessEvent
	//����: ��ģ����Դ�������еĵ�Ե��¼�
	//����: 
	//		@param	evt			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	void ProcessEvent(const Event& evt);

 	//----------------------------------------------------------------------------------------
	//����: ProcessMessage
	//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
	//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
	//����: 
	//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
	//----------------------------------------------------------------------------------------
	void ProcessMessage(const Message& msg);

 	//----------------------------------------------------------------------------------------
	//����: CallDirect
	//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
	//			����������Ҫͨ�����ߡ�
	//����: 
	//		@param	lServiceValue		����1
	//		@param	rparam			����2
	//----------------------------------------------------------------------------------------
	int32 CallDirect(const ServiceValue lServiceValue, param rparam);

 	//----------------------------------------------------------------------------------------
	//����: PaybackExtraInfo
	//����: ĳ��ģ��������Զ���ĸ��ӵ�������Ҫ���������ģ�飬��ô�����Թ���һ��ExtraInfo�ṹ
	//	�������ģ��ʹ����Ϻ󣬸ýṹ���뱻�ͷţ����������ڴ�й¶���ͷű�����ģ��������ɡ�
	//	ĳ��ģ�鶼�����ṩһ��PaybackExtraInfo�ӿڣ��ͷ��Լ����Զ������������
	//����: 
	//		@param	valudId			��Ӧ��pExtraInfo��ֵ���ڲ����ݸ�ֵ���ж�Ӧ���ͷţ���ֵֻ��ģ���Լ����
	//		@param	pExtraInfo	��Ҫ�ͷŵ�ExtraInfo����
	//----------------------------------------------------------------------------------------
	void PaybackExtraInfo(uint32 valudId, void* pExtraInfo);

	// Event������
protected:

	void	OnEvent_OpenMainDlg(Event* pEvent);	// ��ӦEVENT_VALUE_MAINFRAME_OPEN�¼�����������	
	void	OnEvent_CloseMainDlg(Event* pEvent);	// ��ӦEVENT_VALUE_MAINFRAME_CLOSE�¼�����������
	void	OnEvent_HideMainDlg(Event* pEvent);	// ��ӦEVENT_VALUE_MAINFRAME_HIDE�¼�����������										

	// Message������
protected:

	void	OnMessage_CycleTrigged(Message* pMessage);
	void	OnMessage_Exit(Message* pMessage);	// �����˳��Ĺ㲥��Ϣ�������洦�����Ϣ��Ϊ�˳���׼��	

protected:

	void	OnService_Test(ServiceValue lServiceValue, param lParam);

};

class CMainFrameModuleFactory : public ModuleFactoryImpl<MainFrameModule>{};

namespace mainframe
{
	extern MainFrameModule*	g_MainFrameModule;
	extern CMainFrameModuleFactory*	g_MainFrameModuleFactory;
}