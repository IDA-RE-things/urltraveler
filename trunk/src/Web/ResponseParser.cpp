#include "StdAfx.h"
#include "WebDefine.h"
#include "ResponseParser.h"
#include "RequestManager.h"
#include <vector>
#include "WebModule.h"
#include "assert.h"


#define NEW_RESP( RESP, pE) \
	RESP* pE = new RESP;  \
	pE->param0 = WEB_RET_SUCCESS; \
	pE->param1 = pContext->nSpeed; \
	std::map<uint32, uint32>::iterator itr = g_WebModule->m_mapSeqNo2ModuleId.find(pContext->nReqSeq);	\
	if( itr != g_WebModule->m_mapSeqNo2ModuleId.end()) \
	{ \
	pE->desMId = itr->second;	\
	} \
	pE->param2 = pContext->nReqSeq; \



CResponseParser::CResponseParser(void)
{
	m_pRequestManager = NULL;
}

CResponseParser::~CResponseParser(void)
{
}

BEGIN_HANDLER_MAP(CResponseParser)
	ON_HANDLER(EVENT_VALUE_WEB_GET_FAVICON, ParseGetFavIcon)
END_HANDLER_MAP(CResponseParser)

string 
CResponseParser::ConvertCharVector( CHAR_VECTOR* pVector)
{
	string strData;
	strData.append( (*pVector).begin(), (*pVector).end());
	return strData;
}


void	
CResponseParser::SetReuqestManager( CRequestManager*	pRequestManager)
{
	m_pRequestManager = pRequestManager;
}

void CResponseParser::ParseGetFavIcon( HTTPCONTEXT* pContext)
{
}

/**
* Function		根据给定的EventValue值对返回的响应进行解析
* @param		nEventValue 对应的事件值
* @param		lpszContentData 对应的内容
* @param		dwContentSize 读取的报文的长度
* @return 
**/
void	
CResponseParser::ParseResponse( HTTPCONTEXT* pContext )
{
	if( pContext == NULL)
		return;

	if( pContext == NULL)
		return;

	const ParserHandlerMapEntries* pEntry = GetThisEventMap();   
	while( pEntry)   
	{   
		if(  pEntry->nEventValue == EVENT_VALUE_INVALID ||   
			pEntry->nEventValue == 0)   
			break;   

		if( pEntry->nEventValue == pContext->nEventValue   
			&& pEntry->pfEventHandler != NULL)   
		{   
			(this->*pEntry->pfEventHandler)(pContext);   
			return;   
		}   
		++pEntry;   
	}
}