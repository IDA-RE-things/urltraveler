#include "StdAfx.h"
#include "WebDefine.h"
#include "ResponseParser.h"
#include "RequestManager.h"
#include <vector>
#include "WebModule.h"
#include "assert.h"

#pragma comment(lib,"shlwapi.lib")


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


CResponseParser::HandlerTableEntry CResponseParser::m_tableDriven[] =
{
	//{ event_value_web_get_update_config_xml_req,				CResponseParser::ParseGetUpdateConfigXml },
	{ 0, NULL}
};

CResponseParser::CResponseParser(void)
{
	m_pRequestManager = NULL;
	m_pWebData = NULL;
}

CResponseParser::~CResponseParser(void)
{
}

string 
CResponseParser::ConvertCharVector( CHAR_VECTOR* pVector)
{
	string strData;
	strData.append( (*pVector).begin(), (*pVector).end());

	return strData;
}


void 
CResponseParser::SetWebData( CWebData* pWebData)
{
	m_pWebData = pWebData;
}

CWebData*	
CResponseParser::GetWebData( )
{
	return m_pWebData;
}

void	
CResponseParser::SetReuqestManager( CRequestManager*	pRequestManager)
{
	m_pRequestManager = pRequestManager;
}

void	
CResponseParser::ParseGetUpdateConfigXml(CResponseParser* pParser, HTTPCONTEXT* pContext)
{
	LOG_RESP();
	NEW_RESP(get_update_config_xml_event_resp, pE);
	pE->strUpdateXml = string_helper::from_utf8( pContext->strContentData);
	pE->m_pstExtraInfo = (ExtraInfo*)pE;

	PUSH_EVENT(*pE);
}