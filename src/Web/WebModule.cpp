#include "stdafx.h"
#include "WebModule.h"
#include "Web.h"
#include "PathHelper.h"
#include "WebDefine.h"
#include "StringHelper.h"
#include <string>
#include "ImageHelper.h"
#include "httpmanager.h"

HMODULE	g_hModule = NULL;

using namespace std;
using namespace web;

namespace web
{
	WebModule*	g_WebModule = NULL;
	WebModuleFactory*	g_WebModuleFactory = NULL;
}

// �������ʵ��
IModuleFactory*	GetModuleFactory()
{
	if( g_WebModuleFactory == NULL)
	{
		g_WebModuleFactory = new WebModuleFactory();
		g_WebModuleFactory->QueryModulePoint(1, (IModule*&)g_WebModule);
		
		ASSERT( g_WebModule != NULL);
	}

	return g_WebModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_WebModuleFactory == p);
	if( g_WebModuleFactory  != NULL)
	{
		delete g_WebModuleFactory;
		g_WebModuleFactory = NULL;
	}
}

WebModule::WebModule()
{

}

WebModule::~WebModule()
{

}

BEGIN_EVENT_MAP(WebModule)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(WebModule)
END_SERVICE_MAP()

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* WebModule::GetModuleName() 
{
	return L"WebModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const WebModule::GetModuleId()
{
	return MODULE_ID_WEB;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void WebModule::ProcessEvent(const Event& evt)
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
void WebModule::ProcessMessage(const Message& msg) 
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
int32 WebModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}


void WebModule::OnEvent_SaveFavoriteIcon(Event* evt)
{
}

int32 WebModule:: OnService_GetFavoriteIcon(ServiceValue lService, param lparam)
{
	return -1;
}
