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

			// 在使用中，发现m_mapCookie2Event中会有多个同样的地址，原因尚未明确
			// 未防止崩溃，做预先检查
			delete itr->second;
			itr->second = NULL;
		}
	}
	m_mapCookie2Event.clear();

	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* WebModule::GetModuleName() 
{
	return L"WebModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const WebModule::GetModuleId()
{
	return MODULE_ID_WEB;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void WebModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//名称: ProcessMessage
//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
//参数: 
//		@param	msg			需要处理的广播消息
//----------------------------------------------------------------------------------------
void WebModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lparam			参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 WebModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void WebModule::ProcessGetResponse()
{
	// 处理WinHTTP响应报文
	uint8 uNum = 0;
	while( 1)
	{
		HTTPCONTEXT httpContext  = m_pRequestManager->ProcessResp();
		if( uNum++ > 10)
			break;

		// 没有获取到有效的响应数据
		if( httpContext.uOpPolicy == INVALID_SEQNO  )
		{
			break;
		}

		// 重新请求
		if( httpContext.uOpPolicy == REPEATED_REQ)
		{
			httpContext.strContentData.clear();
			continue;
		}

		// 读取数据成功
		if( httpContext.uOpPolicy == SUCCESS_RESP)
		{
			// 如果是收到的buffer立即交给上层，则上下文不删除，继续利用
			if( httpContext.eReqType == FRAG_BUFFER )
			{
				assert( m_pRequestManager && m_pRequestManager->m_pHttpClient );

                // 如果是最后一次
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

			// 发生了超时等错误或者写入磁盘错误
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