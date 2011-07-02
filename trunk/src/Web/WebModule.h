#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include <vector>
#include "WebDefine.h"
#include <map>

using namespace std;
using namespace web;


extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class CRequestManager;
class WebModule : public ModuleImpl
{
	DECLEAR_SERVICE_MAP(WebModule)
	DECLEAR_EVENT_MAP(WebModule)
	DECLEAR_MESSAGE_MAP(WebModule)

public:
	WebModule();
	~WebModule();

	//----------------------------------------------------------------------------------------
	//����: Unload
	//����: ������ͨ���÷�����ģ�����ж��
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Unload();

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
	int32 CallDirect(const ServiceValue lServiceValue, param wparam);

protected:

	void OnEvent_GetFavoriteIcon(Event* pEvent);
	void OnEvent_OpenUrlTraveler(Event* pEvent);
	void OnEvent_CloseUrlTraveler(Event* pEvent);
	void OnEvent_LoginInUrlTraveler(Event* pEvent);
	void OnEvent_LoginOutUrlTraveler(Event* pEvent);
	void OnEvent_ReportUserInfo(Event* pEvent);
	void	OnEvent_CheckUpdateConfig(Event* pEvent);
	void	OnEvent_DownloadUpdateFile(Event* pEvent);

protected:

	int32 OnService_GetFavoriteIcon(ServiceValue lService, param wparam);

protected:
	void OnMessage_CycleTrigged(Message* pMessage);

private:

	void	ProcessGetResponse();
	void	ProcessSendUrl( );

public:

	PROXYDATA			m_ProxyData;

	CRequestManager*	m_pRequestManager;				// ��HTTP����Ĺ����������̨��Web��������������

	Cookie	m_unCookie;
	std::map<Cookie, Event*> m_mapCookie2Event;				// ֧��Event�¼�

	std::map<uint32, uint32>	m_mapSeqNo2ModuleId;	//	�������к���ģ��ID�Ķ�Ӧ��ϵ
};

class WebModuleFactory : public ModuleFactoryImpl<WebModule>{};

namespace web
{
	extern WebModule*	g_WebModule;
	extern WebModuleFactory*	g_WebModuleFactory;
}