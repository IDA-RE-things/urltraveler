#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"

#include "SettingWnd.h"
#include "OptionCenter.h"


extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class SettingModule : public ModuleImpl
{
	DECLEAR_EVENT_MAP(SettingModule);
	DECLEAR_SERVICE_MAP(SettingModule);

public:
	SettingModule();
	~SettingModule();

	//----------------------------------------------------------------------------------------
	//����: Load
	//����: ������ͨ���÷�����ģ����м���
	//����: 
	//		@param	pManager			��ģ�����ߵ�ָ��	
	//����: 
	//		������سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Load(IModuleManager* pManager);

	//----------------------------------------------------------------------------------------
	//����: GetModuleName
	//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetModuleName();

 	//----------------------------------------------------------------------------------------
	//����: GetModuleId
	//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
	//����: 
	//		���ظ�ģ���Ψһ��ID
	//----------------------------------------------------------------------------------------
	virtual uint32 const GetModuleId();

 	//----------------------------------------------------------------------------------------
	//����: ProcessEvent
	//����: ��ģ����Դ�������еĵ�Ե��¼�
	//����: 
	//		@param	evt			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	virtual void ProcessEvent(const Event& evt);

 	//----------------------------------------------------------------------------------------
	//����: ProcessMessage
	//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
	//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
	//����: 
	//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
	//----------------------------------------------------------------------------------------
	virtual void ProcessMessage(const Message& msg);

 	//----------------------------------------------------------------------------------------
	//����: CallDirect
	//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
	//			����������Ҫͨ�����ߡ�
	//����: 
	//		@param	lparam			����1
	//		@param	rparam			����2
	//----------------------------------------------------------------------------------------
	virtual int32 CallDirect(const param lServiceValue, param wparam);

public:

	OptionCenter	m_OptionCenter;  

protected:

	void	OnEvent_OpenSettingWnd(Event* pEvent);

protected:

	int	OnService_GetAutoStartWithWindow(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetRememberPassword(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetAutoLogin(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetExitWhenCloseWnd(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetAutoLocalSync(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetAutoRemoteSync(ServiceValue lServiceValue, param	lParam);
	int	OnService_GetUpdateType(ServiceValue lServiceValue, param	lParam);

protected:
	CSettingWnd*	m_pSettingWnd;

};

class SettingModuleFactory : public ModuleFactoryImpl<SettingModule>{};

namespace setting
{
	extern SettingModule*	g_SettingModule;
	extern SettingModuleFactory*	g_SettingModuleFactory;
}

#define  OPTION_CENTER         (setting::g_SettingModule->m_OptionCenter)