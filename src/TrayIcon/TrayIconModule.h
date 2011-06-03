#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "TrayMgr.h"

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class TrayIconModule : public ModuleImpl, public ITrayEvent
{
	DECLEAR_EVENT_MAP(TrayIconModule)
	DECLEAR_MESSAGE_MAP(TrayIconModule)

public:
	TrayIconModule();
	virtual ~TrayIconModule();

	//----------------------------------------------------------------------------------------
	//����: Load
	//����: ������ͨ���÷�����ģ����м���
	//����: 
	//		@param	pManager			��ģ�����ߵ�ָ��	
	//����: 
	//		������سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	BOOL Load(IModuleManager* pManager) ;

	//----------------------------------------------------------------------------------------
	//����: Unload
	//����: ������ͨ���÷�����ģ�����ж��
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	BOOL Unload() ;

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
	//		@param	lparam			����1
	//		@param	rparam			����2
	//----------------------------------------------------------------------------------------
	int32 CallDirect(const param lparam, param wparam);

	// ��Ӧ������ͼ���ϵ��¼�
	virtual void OnTrayEvent(WPARAM w, LPARAM l);

	// ������ͼ�꿪ʼ��˸
	virtual void OnFlashStart(DWORD dwParam);

	// ������ͼ�������˸
	virtual void OnFlashEnd(DWORD dwParam);

	// Event������
protected:

	void	OnEvent_ShowTrayIcon(Event* pEvent);	
public:

	void    OnEvent_ShowAboutDialog(Event *pEvent);

protected:

	void	OnMessage_Show(Message* pMessage);
	// �����˳��Ĺ㲥��Ϣ�������洦�����Ϣ��Ϊ�˳���׼��
	void	OnMessage_PreExit(Message* pMessage);		


private:
	void	CreatTrayIconMsgWnd();
	void DestroyTrayIconMsgWnd();

private:

	CTrayMgr		m_TrayMgr;
	HWND			m_hMsgWnd;		 //	�ڲ������ش��ڣ��������ղ˵�����Ϣ
};

class CTrayIconModuleFactory : public ModuleFactoryImpl<TrayIconModule>{};

namespace trayicon
{
	extern TrayIconModule*	g_TrayIconModule;
	extern CTrayIconModuleFactory*	g_TrayIconModuleFactory;
}