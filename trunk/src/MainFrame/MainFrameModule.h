#pragma once

#include "SndaBase.h"
#include "ModuleImpl.h"
#include "MainFrameWnd.h"

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}


class CMainFrameWnd;
class MainFrameModule : public ModuleImpl
{
	DECLEAR_EVENT_MAP(MainFrameModule)
		DECLEAR_MESSAGE_MAP(MainFrameModule)
		DECLEAR_SERVICE_MAP(MainFrameModule)

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


	// Event������
protected:

	void	OnEvent_OpenMainDlg(Event* pEvent);	// ��ӦEVENT_VALUE_MAINFRAME_OPEN�¼�����������	
	void	OnEvent_CloseMainDlg(Event* pEvent);	// ��ӦEVENT_VALUE_MAINFRAME_CLOSE�¼����ر�������
	void	OnEvent_ShowMainDlg(Event* pEvent);		// ��ӦEVENT_VALUE_MAINFRAME_SHOW�¼�����ʾ������										
	void	OnEvent_HideMainDlg(Event* pEvent);		// ��ӦEVENT_VALUE_MAINFRAME_HIDE�¼�������������	

	void	OnEvent_FavoriteIconArrive(Event* pEvent);		//	��ַ��Ӧ��ICONͼ��ﵽ
	void	OnEvent_AddUrl(Event*	pEvent);						//	����һ���µ�URL
	void	OnEvent_DeleteFavorite(Event* pEvent);			//	ɾ���������ղؼ�¼
	void	OnEvent_OpenUrl(Event* pEvent);			//	�򿪸������ղ���ַ
	void	OnEvent_CopyUrl(Event* pEvent);			//	������ַ
	void	OnEvent_DeleteFavoriteFold(Event* pEvent);	//	ɾ���ղؼ�
	void	OnEvent_SelectTreeListItem(Event* pEvent);	//	ѡ��������е�ĳһ��

	// Message������
protected:

	void	OnMessage_CycleTrigged(Message* pMessage);
	void	OnMessage_Show(Message* pMessage);
	void	OnMessage_PreExit(Message* pMessage);	// �����˳��Ĺ㲥��Ϣ�������洦�����Ϣ��Ϊ�˳���׼��	
	void	OnMessage_FavoriteLoaded(Message* pMessage);		//	�ղؼ��Ѿ��ϲ�����
	void	OnMessage_PlugInLoaded(Message* pMessage);

protected:

	int32 OnService_GetMainWnd(ServiceValue lServiceValue, param	lParam);

private:
	CMainFrameWnd* m_pMainFrame;
};

class MainFrameModuleFactory : public ModuleFactoryImpl<MainFrameModule>{};

namespace mainframe
{
	extern MainFrameModule*	g_MainFrameModule;
	extern MainFrameModuleFactory*	g_MainFrameModuleFactory;
}