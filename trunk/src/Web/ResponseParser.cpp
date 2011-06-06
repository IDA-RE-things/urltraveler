#include "StdAfx.h"
#include "WebDefine.h"
#include "ResponseParser.h"
#include "RequestManager.h"
#include <vector>
#include "WebModule.h"
#include "assert.h"
#include "StringHelper.h"


#define NEW_RESP( Response, pEvent) \
	Response* pEvent = new Response;  \
	pEvent->param0 = WEB_RET_SUCCESS; \
	pEvent->param1 = pContext->nSpeed; \
	std::map<uint32, uint32>::iterator itr = g_WebModule->m_mapSeqNo2ModuleId.find(pContext->nReqSeq);	\
	if( itr != g_WebModule->m_mapSeqNo2ModuleId.end()) \
	{ \
	pEvent->desMId = itr->second;	\
	} \
	pEvent->param2 = pContext->nReqSeq; \



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
	NEW_RESP(Web_GetFavIconRespEvent, pEvent);
	STRNCPY(pEvent->szFavoriteUrl, StringHelper::ANSIToUnicode(pContext->strURL).c_str());
	pEvent->nIconSize = pContext->strContentData.size();
	pEvent->pIconData = (char*)pContext->strContentData.c_str();

	g_WebModule->GetModuleManager()->PushEvent(*pEvent);
}

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