#include "stdafx.h"
#include "WebModule.h"
#include "WebDefine.h"
#include "StringHelper.h"
#include <string>
#include "RequestManager.h"
#include "HttpClient.h"

HMODULE	g_hModule = NULL;

using namespace std;
using namespace web;

namespace web
{
	WebModule*	g_WebModule = NULL;
	WebModuleFactory*	g_WebModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(WebModule)
EXPORT_RELEASEMODULEFACTORY(WebModule)


WebModule::WebModule()
{
	m_pRequestManager = new CRequestManager;

	m_unCookie = 0;
	m_pRequestManager->m_pHttpClient = CHttpClient::Instance();
}

WebModule::~WebModule()
{
	if (m_pRequestManager)
	{
		delete m_pRequestManager;
		m_pRequestManager = NULL;
	}
}

BEGIN_EVENT_MAP(WebModule)
	ON_EVENT(EVENT_VALUE_WEB_GET_FAVICON, OnEvent_GetFavoriteIcon)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(WebModule)
END_SERVICE_MAP()

BEGIN_MESSAGE_MAP(WebModule)
	ON_MESSAGE(MESSAGE_VALUE_CORE_CYCLE_TRIGGER, OnMessage_CycleTrigged)
END_MESSAGE_MAP()


BOOL 
WebModule::Unload()
{
	std::map<Cookie, Event*>::iterator itr = m_mapCookie2Event.begin();
	for( ; itr != m_mapCookie2Event.end(); itr++)
	{
		if( itr->second)
		{
			Event* pE = itr->second;

			// ��ʹ���У�����m_mapCookie2Event�л��ж��ͬ���ĵ�ַ��ԭ����δ��ȷ
			// δ��ֹ��������Ԥ�ȼ��
			delete itr->second;
			itr->second = NULL;
		}
	}
	m_mapCookie2Event.clear();

	return TRUE;
}

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
int32 WebModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void WebModule::ProcessGetResponse()
{
	// ����WinHTTP��Ӧ����
	uint8 uNum = 0;
	while( 1)
	{
		HTTPCONTEXT httpContext  = m_pRequestManager->ProcessResp();
		if( uNum++ > 10)
			break;

		// û�л�ȡ����Ч����Ӧ����
		if( httpContext.uOpPolicy == INVALID_SEQNO  )
		{
			break;
		}

		// ��������
		if( httpContext.uOpPolicy == REPEATED_REQ)
		{
			httpContext.strContentData.clear();
			continue;
		}

		// ��ȡ���ݳɹ�
		if( httpContext.uOpPolicy == SUCCESS_RESP)
		{
			// ������յ���buffer���������ϲ㣬�������Ĳ�ɾ������������
			if( httpContext.eReqType == FRAG_BUFFER )
			{
				assert( m_pRequestManager && m_pRequestManager->m_pHttpClient );

                // ��������һ��
                if( httpContext.uParam2 == 1)
                {
                    m_pRequestManager->m_pHttpClient->DeleteSeqNo(httpContext.nReqSeq);
                }
			}
			else
			{
                m_pRequestManager->m_pHttpClient->DeleteSeqNo( httpContext.nReqSeq);
			}
		}
		else
		{
			if( httpContext.nErrorCode == ERR_TASK_WRITE_FILE_ERROR)
			{
			}
			else
			{
			}

			// �����˳�ʱ�ȴ������д����̴���
			Event e;
			e.param0 = httpContext.uOpPolicy;
			e.param2 = httpContext.nReqSeq;
			e.eventValue = httpContext.nEventValue + 1;
			e.srcMId = MODULE_ID_WEB;
 
			std::map<uint32, uint32>::iterator itr = g_WebModule->m_mapSeqNo2ModuleId.find( httpContext.nReqSeq);
			if( itr != g_WebModule->m_mapSeqNo2ModuleId.end())
			{
				e.desMId = itr->second;
			}

			g_WebModule->m_pModuleManager->PushEvent(e);
            m_pRequestManager->m_pHttpClient->DeleteSeqNo( httpContext.nReqSeq);
		}
	}
}

void WebModule::ProcessSendUrl( )
{
	UINT nNum = 0;
	while( nNum < 3)
	{
		HTTPCONTEXT* pContext = NULL ;
		if( m_pRequestManager->PopUrl( pContext) )
		{
			m_pRequestManager->ReqUrl( pContext);
		}

		nNum++;
	}
}

void WebModule::OnEvent_GetFavoriteIcon(Event* pEvent)
{
	Web_GetFavIconReqEvent* pE = (Web_GetFavIconReqEvent*)pEvent->m_pstExtraInfo;
	if( pE == NULL)
	{
		ASSERT(0);
		return;
	}

	uint32 nSeqNo = m_pRequestManager->PushUrl(	pEvent->eventValue,0,pE->szFavoriteUrl,NULL);
	if( nSeqNo != INVALID_SEQNO)
	{
		m_mapSeqNo2ModuleId[nSeqNo] = pEvent->srcMId;
	}
}

void WebModule::OnMessage_CycleTrigged(Message* pMessage)
{
	ProcessGetResponse();
	ProcessSendUrl();
}

int32 WebModule:: OnService_GetFavoriteIcon(ServiceValue lService, param lparam)
{
	return -1;
}
