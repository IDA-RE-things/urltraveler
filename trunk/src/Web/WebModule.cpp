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
END_EVENT_MAP()

BEGIN_SERVICE_MAP(WebModule)
	ON_SERVICE(SERVICE_VALUE_WEB_GET_FAVICON, OnService_GetFavoriteIcon)
	ON_SERVICE(SERVICE_VALUE_WEB_OPEN_URLTRAVELER, OnService_OpenUrlTraveler)
	ON_SERVICE(SERVICE_VALUE_WEB_CLOSE_URLTRAVELER, OnService_CloseUrlTraveler)
	ON_SERVICE(SERVICE_VALUE_WEB_LOGININ_URLTRAVELER, OnService_LoginInUrlTraveler)
	ON_SERVICE(SERVICE_VALUE_WEB_LOGINOUT_URLTRAVELER, OnService_LoginOutUrlTraveler)
	ON_SERVICE(SERVICE_VALUE_WEB_REPORT_USERINFO, OnService_ReportUserInfo)
	ON_SERVICE(SERVICE_VALUE_WEB_CHECK_UPDATE_CONFIG,OnService_CheckUpdateConfig)
	ON_SERVICE(SERVICE_VALUE_WEB_DOWNLOAD_UPDATE_FILE,OnService_DownloadUpdateFile)
	ON_SERVICE(SERVICE_VALUE_WEB_QUERY_DOWNLOAD_FILE_PROESS, OnService_QueryDownloadUpdateFileProcess)
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
			e.eventValue = httpContext.nEventValue - SERVICE_VALUE_WEB_BEGIN + EVENT_VALUE_WEB_BEGIN;
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

int32 WebModule::OnService_GetFavoriteIcon(ServiceValue lService, param wparam)
{
	Web_GetFavIconqService* pService = (Web_GetFavIconqService*)wparam;
	if( pService == NULL || pService->srcMId == MODULE_ID_INVALID)
	{
		ASSERT(0);
		return -1;
	}

	uint32 nSeqNo = m_pRequestManager->PushUrl(	pService->serviceId,0,pService->szFavoriteUrl,NULL);
	if( nSeqNo != INVALID_SEQNO)
	{
		m_mapSeqNo2ModuleId[nSeqNo] = pService->srcMId;
	}

	return nSeqNo;
}

int32 WebModule::OnService_OpenUrlTraveler(ServiceValue lService, param wparam)
{
	Web_OpenTravelerService* pService = (Web_OpenTravelerService*)wparam;
	if( pService == NULL || pService->serviceId  != SERVICE_VALUE_WEB_OPEN_URLTRAVELER)
	{
		ASSERT(0);
		return -1;
	}

	uint32 nSeqNo = m_pRequestManager->PushUrl(pService->serviceId,0,
		L"http://1.urltraveler.sinaapp.com/login.php",NULL);
	if( nSeqNo != INVALID_SEQNO)
	{
		m_mapSeqNo2ModuleId[nSeqNo] = pService->srcMId;
	}

	return nSeqNo;
}

int32 WebModule::OnService_CloseUrlTraveler(ServiceValue lService, param wparam)
{
	Web_CloseTravelerService* pService = (Web_CloseTravelerService*)wparam;
	if( pService == NULL || pService->serviceId  != SERVICE_VALUE_WEB_CLOSE_URLTRAVELER)
	{
		ASSERT(0);
		return -1;
	}

	uint32 nSeqNo = m_pRequestManager->PushUrl(pService->serviceId,0,
		L"http://1.urltraveler.sinaapp.com/logout.php",NULL);
	if( nSeqNo != INVALID_SEQNO)
	{
		m_mapSeqNo2ModuleId[nSeqNo] = pService->srcMId;
	}

	return nSeqNo;
}

int32 WebModule::OnService_LoginInUrlTraveler(ServiceValue lService, param wparam)
{
	return -1;
}

int32 WebModule::OnService_LoginOutUrlTraveler(ServiceValue lService, param wparam)
{
	return -1;
}

int32 WebModule::OnService_ReportUserInfo(ServiceValue lService, param wparam)
{
	Web_ReportUserInfoService* pService = (Web_ReportUserInfoService*)wparam;
	if( pService == NULL || pService->serviceId  != SERVICE_VALUE_WEB_REPORT_USERINFO)
	{
		ASSERT(0);
		return -1;
	}

	string strMachinedId = StringHelper::UnicodeToANSI(pService->szMachinedId);
	string strOsVersion = StringHelper::UnicodeToANSI(pService->szOSVersion);

	POST_PARAMS httpArgument ;
	httpArgument.add_param("machineId", strMachinedId.c_str(), strMachinedId.size() + 1, PARAM_TEXT);
	httpArgument.add_param("osVersion", strOsVersion.c_str(), strOsVersion.size() + 1, PARAM_TEXT);

	uint32 nSeqNo = m_pRequestManager->PushUrl(pService->serviceId,0,
		L"http://1.urltraveler.sinaapp.com/userinfo.php",NULL);
	if( nSeqNo != INVALID_SEQNO)
	{
		m_mapSeqNo2ModuleId[nSeqNo] = pService->srcMId;
	}
	return nSeqNo;
}

int32	WebModule::OnService_CheckUpdateConfig(ServiceValue lService, param wparam)
{
	Web_CheckUpdateConfigService* pService = (Web_CheckUpdateConfigService*)wparam;
	if( pService == NULL || pService->serviceId  != SERVICE_VALUE_WEB_CHECK_UPDATE_CONFIG)
	{
		ASSERT(0);
		return -1;
	}
	
	uint32 nSeqNo = m_pRequestManager->PushUrl(pService->serviceId,0,
		L"http://urltraveler.sinaapp.com/getversion.php?type=copy&clientVersion=1.1",NULL);
	if( nSeqNo != INVALID_SEQNO)
	{
		m_mapSeqNo2ModuleId[nSeqNo] = pService->srcMId;
	}
	return nSeqNo;
}

int32	WebModule::OnService_DownloadUpdateFile(ServiceValue lService, param wparam)
{
	Web_DownloadUpdateFileService* pService = (Web_DownloadUpdateFileService*)wparam;
	if( pService == NULL || pService->serviceId  != SERVICE_VALUE_WEB_DOWNLOAD_UPDATE_FILE)
	{
		ASSERT(0);
		return -1;
	}

	string strDownloadUrl = StringHelper::UnicodeToANSI(pService->szUpdateFileUrl);

	uint32 nSeqNo = m_pRequestManager->PushUrl(pService->serviceId,0,
		StringHelper::ANSIToUnicode(strDownloadUrl).c_str(),NULL,ONCE_FILE, pService->szSavePath);
	if( nSeqNo != INVALID_SEQNO)
	{
		m_mapSeqNo2ModuleId[nSeqNo] = pService->srcMId;
	}

	return nSeqNo;
}

int32 WebModule::OnService_QueryDownloadUpdateFileProcess(ServiceValue lService, param wparam)
{
	Web_QueryDownFileProcessService* pService = (Web_QueryDownFileProcessService*)wparam;
	if( pService == NULL || pService->serviceId  != SERVICE_VALUE_WEB_QUERY_DOWNLOAD_FILE_PROESS)
	{
		ASSERT(0);
		return -1;
	}

	int nSeqNo = pService->nSeqNo;
	uint32 ret = m_pRequestManager->QueryDownloadProcess( pService->nSeqNo, &pService->uPercent, 
		&pService->uTotalLength, &pService->uFinishedLength, &pService->uSpeed, &pService->uRemainedTime);
	if( ret == false)
	{
		return INVALID_SEQNO;
	}

	return READY_SEQNO;
}


void WebModule::OnMessage_CycleTrigged(Message* pMessage)
{
	ProcessGetResponse();
	ProcessSendUrl();
}