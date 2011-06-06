#include "stdafx.h"
#include "HttpClient.h"
#include "stdio.h"
#include "StringHelper.h"
#include "assert.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <sstream>
#include "httpmanager.h"
#include "HttpHandler.h"
#include "HttpContext.h"
#include "WebModule.h"


using namespace web;

#define	END_FLAG_LENGTH		256

CHttpClient* CHttpClient::Singleton = 0;

CHttpClient::CHttpClient( )
{
	m_pTransferManager = NULL;
	m_pTransferManager = TranferManager::getInstance();

	if( m_pTransferManager)
	{
		//m_pTransferManager->setLogPath( const_cast<char*>(string_helper::from(g_WebModule->GetHttpClientLogPath(GetCurInfo().unUIN)).c_str()));

		INT32 nRet = m_pTransferManager->start();
		if( nRet != 0 )
		{
			m_pTransferManager = NULL;
		}
		else
		{
			m_Callback.handle_init = handle_init;
			m_Callback.handle_headcomplete = handle_headcomplete;
			m_Callback.handle_process = handle_process;
			m_Callback.handle_finish = handle_finish;
			m_Callback.handle_error = handle_error;
		}
	}

	Singleton = this;
}

void	
CHttpClient::SetProxy( PROXYDATA* pProxyData)
{
	if( pProxyData)
	{
		Proxy_t PROXYDATA;

		int nIndex = 0;
		if( pProxyData->arrDetail[0].eType != pProxyData->eLastUsed)	
			nIndex = 1;

		if( pProxyData->eLastUsed == HTTPDOWN_PROXY_HTTP)
		{
			PROXYDATA.type = HTTPDOWN_PROXY_HTTP;
			PROXYDATA.host = StringHelper::UnicodeToANSI(pProxyData->arrDetail[nIndex].strServer);
			PROXYDATA.port = StringHelper::ConvertToInt(StringHelper::UnicodeToANSI(pProxyData->arrDetail[nIndex].strPort));
			PROXYDATA.user = StringHelper::UnicodeToANSI(pProxyData->arrDetail[nIndex].strUserName);
			PROXYDATA.pwd = StringHelper::UnicodeToANSI( pProxyData->arrDetail[nIndex].strPassword);
			PROXYDATA.domain = StringHelper::UnicodeToANSI( pProxyData->arrDetail[nIndex].strDomain);
		}
		else if( pProxyData->eLastUsed == PROXY_SOCK5)
		{
			PROXYDATA.type = HTTPDOWN_PROXY_SOCK5;
			PROXYDATA.host = StringHelper::UnicodeToANSI(pProxyData->arrDetail[nIndex].strServer);
			PROXYDATA.port = StringHelper::ConvertToInt(StringHelper::UnicodeToANSI(pProxyData->arrDetail[nIndex].strPort));
			PROXYDATA.user = StringHelper::UnicodeToANSI(pProxyData->arrDetail[nIndex].strUserName);
			PROXYDATA.pwd = StringHelper::UnicodeToANSI( pProxyData->arrDetail[nIndex].strPassword);
		}
		else if( pProxyData->eLastUsed == PROXY_IE)
		{
/*
			PROXYDATA.type = HTTPDOWN_PROXY_HTTP;

			WINHTTP_PROXY_INFO winHttpProxy;
			BOOL bRet = DetectIEProxySettings(&winHttpProxy);
			if(bRet == TRUE)
			{
				vector<wstring> vProxyString = wstring_helper::split_string_by_delimiter( winHttpProxy.lpszProxy,':');
				PROXYDATA.host = string_helper::from(vProxyString[0]);
				PROXYDATA.port = wstring_helper::from_string(vProxyString[1]);
				::GlobalFree( winHttpProxy.lpszProxy);
				::GlobalFree( winHttpProxy.lpszProxyBypass);

				PROXYDATA.user		= string_helper::from(pProxyData->arrDetail[PROXY_HTTP].strUserName);
				PROXYDATA.pwd		= string_helper::from(pProxyData->arrDetail[PROXY_HTTP].strPassword);
				PROXYDATA.domain	= string_helper::from(pProxyData->arrDetail[PROXY_HTTP].strDomain);
			}
			else
			{
				PROXYDATA.host = "";
				PROXYDATA.port = 0;
			}
*/
		}

		if( ( pProxyData->eLastUsed == PROXY_HTTP || pProxyData->eLastUsed == PROXY_SOCK5)
			|| ( pProxyData->eLastUsed == PROXY_IE &&( PROXYDATA.host != "" && PROXYDATA.port != 0)))
		{
			m_pTransferManager = TranferManager::getInstance();
			m_pTransferManager->setProxy( PROXYDATA);
		}
		else
		{
			m_pTransferManager = TranferManager::getInstance();
			PROXYDATA.type = HTTPDOWN_PROXY_BUTT;
			m_pTransferManager->setProxy(PROXYDATA);
		}
	}
}


CHttpClient::~CHttpClient()
{
	if( m_pTransferManager)
	{
		m_pTransferManager->stop();
	}

	map<unsigned int, HTTPCONTEXT*>::iterator iterMap;
	for(iterMap = m_mapRequest.begin(); iterMap != m_mapRequest.end(); ++iterMap)
	{
		if( iterMap->second)
		{
			delete iterMap->second;
			iterMap->second = NULL;
		}
	}
	m_mapRequest.clear();

	m_mapTaskid2Seqno.clear();
	m_mapSeqNo2TaskId.clear();
	m_lstResult.clear();

	TranferManager::delInstance();
	Singleton = NULL;
}

CHttpClient*	CHttpClient::Instance()
{
	if( Singleton == NULL)
		Singleton = new CHttpClient();

	assert( Singleton != 0);

	return Singleton;
}

void	
CHttpClient::ClearSeqIdMap( uint32 nSeqNo)
{
	std::map< uint32, int32>::iterator itr = m_mapSeqNo2TaskId.find( nSeqNo);
	if( itr != m_mapSeqNo2TaskId.end())
	{
		m_mapTaskid2Seqno.erase(itr->second);

		TaskInfo taskInfo;
		m_pTransferManager->cancelTask( itr->second, taskInfo);
	}
	m_mapSeqNo2TaskId.erase( nSeqNo);
}

void    
CHttpClient::DeleteSeqNo( uint32 nSeqNo)
{
	ClearSeqIdMap( nSeqNo);

	map<uint32, HTTPCONTEXT*>::iterator itr = m_mapRequest.find( nSeqNo);
	if( itr != m_mapRequest.end() && itr->second)
	{
		if( itr->second->pArgument)
		{
			delete itr->second->pArgument;
			itr->second->pArgument = NULL;
		}

		delete itr->second;
		itr->second = NULL;
	}

	m_mapRequest.erase( nSeqNo);
}

void	
CHttpClient::CleanAll()
{
	map<uint32, HTTPCONTEXT*>::iterator itr = m_mapRequest.begin();
	for( ;itr != m_mapRequest.end(); itr++)
	{
		if( itr->second)
		{
			if( itr->second->pArgument)
			{
				delete itr->second->pArgument;
				itr->second->pArgument = NULL;
			}

			delete itr->second;
			itr->second = NULL;
		}
	}

	m_mapRequest.clear();

	m_mapTaskid2Seqno.clear();
	m_mapSeqNo2TaskId.clear();
	m_lstResult.clear();
}


HTTPCONTEXT*	
CHttpClient::GetContext( unsigned int nSeqNo)
{
	std::map< unsigned int, HTTPCONTEXT*>::iterator itr = m_mapRequest.find( nSeqNo);
	if( itr == m_mapRequest.end())
		return NULL;

	return itr->second;
}

/**
* Function 请求处理过程中失败的时候进行的清理工作
* @return 
**/
void 
CHttpClient::Cleanup( HTTPCONTEXT* pContext)
{
	m_Lock.Lock();
	m_lstResult.push_back( *pContext);
	m_Lock.Unlock();
}

int32	
CHttpClient::ReqURL( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
	{
		assert(0);
		return -1;
	}

	if( m_pTransferManager == NULL)
	{
		return -1;
	}

	m_Lock.Lock();
	m_mapRequest[pContext->nReqSeq] = pContext;
	m_Lock.Unlock();

	m_Callback.handle_init = handle_init;
	m_Callback.handle_headcomplete = handle_headcomplete;
	m_Callback.handle_process = handle_process;
	m_Callback.handle_finish = handle_finish;
	m_Callback.handle_error = handle_error;

	OPTIONS  stOpts;
	stOpts.tasktimeout = pContext->nTimeout;
	if( pContext->eReqType == ONCE_FILE || pContext->eReqType == FRAG_BUFFER)
		stOpts.dataClean = true;

	int32 ret = m_pTransferManager->addTask( 
		pContext->strURL, 
		TASK_DOWNLOAD, 
		NULL, 
		m_Callback, 
		stOpts
		);

	if( ret < 0)
		return ret;

	pContext->nTaskId = ret;
	m_mapTaskid2Seqno[ret] = pContext->nReqSeq;
	m_mapSeqNo2TaskId[pContext->nReqSeq] = ret;

	return 0;
}

int32 
CHttpClient::ReReqURL( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
		return -1;

	if( m_pTransferManager == NULL)
	{
		return -1;
	}

	HTTPCONTEXT* pCxt = GetContext( pContext->nReqSeq);
	if( pCxt == NULL)
		return -1;

	m_Lock.Lock();
	pCxt->nFailedTime = pContext->nFailedTime;
	m_Lock.Unlock();

	m_Callback.handle_init = handle_init;
	m_Callback.handle_headcomplete = handle_headcomplete;
	m_Callback.handle_process = handle_process;
	m_Callback.handle_finish = handle_finish;
	m_Callback.handle_error = handle_error;

	OPTIONS  stOpts;
	stOpts.tasktimeout = pContext->nTimeout;	
	if( pContext->eReqType == ONCE_FILE || pContext->eReqType == FRAG_BUFFER)
		stOpts.dataClean = true;

	int32 ret = m_pTransferManager->addTask( 
		pContext->strURL, 
		TASK_DOWNLOAD, 
		NULL, 
		m_Callback, 
		stOpts
		);

	if( ret < 0)
		return ret;

	pContext->nTaskId = ret;
	m_mapTaskid2Seqno[ret] = pContext->nReqSeq;
	m_mapSeqNo2TaskId[pContext->nReqSeq] = ret;

	return 0;
}

int32	
CHttpClient::ReqPostUrl( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
		return -1;

	if( m_pTransferManager == NULL)
	{
		return -1;
	}

	/** 对Map进行操作的时候需要加锁*/
	m_Lock.Lock();
	m_mapRequest[pContext->nReqSeq] = pContext;
	m_Lock.Unlock();

	m_Callback.handle_init = handle_init;
	m_Callback.handle_headcomplete = handle_headcomplete;
	m_Callback.handle_process = handle_process;
	m_Callback.handle_finish = handle_finish;
	m_Callback.handle_error = handle_error;

	OPTIONS  stOpts;
	stOpts.tasktimeout = pContext->nTimeout;
	if( pContext->eReqType == ONCE_FILE || pContext->eReqType == FRAG_BUFFER)
		stOpts.dataClean = true;

	int ret = 0;
	if( pContext->pArgument->is_uploadtype())
	{
		ret = m_pTransferManager->addTask( 
			pContext->strURL, 
			TASK_UPLOAD, 
			pContext->pArgument, 
			m_Callback, 
			stOpts
			);
	}
	else
	{
		ret = m_pTransferManager->addTask( 
			pContext->strURL, 
			TASK_QUERY, 
			pContext->pArgument, 
			m_Callback, 
			stOpts
			);
	}

	if( ret < 0 )
	{
		assert(0);
		return ret;
	}

	m_mapTaskid2Seqno[ret] = pContext->nReqSeq;
	m_mapSeqNo2TaskId[pContext->nReqSeq] = ret;
	pContext->nTaskId = ret;

	return 0;
}

int32 
CHttpClient::ReReqPostUrl( HTTPCONTEXT* pContext)
{
	if( pContext == NULL)
		return -1;

	if( m_pTransferManager == NULL)
	{
		return -1;
	}

	HTTPCONTEXT* pCxt = GetContext( pContext->nReqSeq);
	if( pCxt == NULL)
		return -1;

	m_Lock.Lock();
	pCxt->nFailedTime = pContext->nFailedTime;
	m_Lock.Unlock();

	/** 对Map进行操作的时候需要加锁*/
	m_Callback.handle_init = handle_init;
	m_Callback.handle_headcomplete = handle_headcomplete;
	m_Callback.handle_process = handle_process;
	m_Callback.handle_finish = handle_finish;
	m_Callback.handle_error = handle_error;

	OPTIONS  stOpts;
	stOpts.tasktimeout = pContext->nTimeout;
	if( pContext->eReqType == ONCE_FILE || pContext->eReqType == FRAG_BUFFER)
		stOpts.dataClean = true;

	int ret = 0;
	if( pContext->pArgument && pContext->pArgument->is_uploadtype())
	{
		ret = m_pTransferManager->addTask( 
			pContext->strURL, 
			TASK_UPLOAD, 
			pContext->pArgument, 
			m_Callback, 
			stOpts
			);
	}
	else
	{
		ret = m_pTransferManager->addTask( 
			pContext->strURL, 
			TASK_QUERY, 
			pContext->pArgument, 
			m_Callback, 
			stOpts
			);
	}

	if( ret < 0 )
		return ret;

	m_mapTaskid2Seqno[ret] = pContext->nReqSeq;
	m_mapSeqNo2TaskId[pContext->nReqSeq] = ret;
	pContext->nTaskId = ret;

	return 0;
}

void	
CHttpClient::CancelTask( uint32 nSeqNo)
{
	HTTPCONTEXT* pContext = GetContext(nSeqNo);
	if( pContext && pContext->hFileHandle)
	{
		::CloseHandle( pContext->hFileHandle);
	}

	TaskInfo taskInfo;

	int taskId = 0;
	std::map<uint32, int32>::iterator itr = m_mapSeqNo2TaskId.find( nSeqNo);
	if( itr != m_mapSeqNo2TaskId.end())
	{
		taskId = itr->second;

		if( m_pTransferManager)
			m_pTransferManager->cancelTask( taskId, taskInfo);

		m_mapSeqNo2TaskId.erase( nSeqNo);
		m_mapTaskid2Seqno.erase( taskId);
		m_mapRequest.erase( nSeqNo);
	}
}

/**
* Function 从响应集中获取一个响应
* @param stContext 返回的响应
* @return 返回true 表示返回的是一个有效的响应，false则表示是一个无效的响应
* stContext中的数据是无效数据
**/
HTTPCONTEXT 
CHttpClient::RetriveResp( )
{
	HTTPCONTEXT stContext;
	stContext.uOpPolicy = INVALID_RESP;

	m_Lock.Lock();
	if(m_lstResult.size())
	{
		stContext = m_lstResult.front();
		m_lstResult.pop_front();
	}
	m_Lock.Unlock();

	return stContext;
}

int32	
CHttpClient::GetSeqNo( int32	nTaskId)
{
	std::map< int32, uint32 >::iterator itr = m_mapTaskid2Seqno.find( nTaskId);
	if( itr == m_mapTaskid2Seqno.end())
		return -1;

	return itr->second;
}