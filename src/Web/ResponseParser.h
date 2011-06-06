#pragma once

/****************************************************************************
* ClassName:   CResponseParser												*
* Author:	   Zhang Z.Q													*
* Using:	   ���ฺ�����Ӧ�ı������ݽ���XML������Ȼ���䱣�浽��������	*
*																			*
* Verion:																	*
*		Created			2007,12,12											*
*****************************************************************************/
#include "WebDefine.h"
#include <string>
#include <map>
#include "httpmanager.h"
#include "HttpContext.h"

using namespace std;
using namespace web;

#define	RET_ERROR_XML_FILE		1			// ���ص�XML�ļ������޷�����
#define	RET_ERROR_RESPONSE		2			// ���������ʱ��������ķ��ر��ģ�ͨ����ִ�в��ɹ�
#define RET_ERROR_NO_WEBDATA	3			// ����������û��ָ���������ݲֿ�
#define RET_ERROR_NO_HANDLER	4			// û���ҵ���Ӧ�Ľ�������
#define RET_INNER_USED			5			// �ý��������ڲ�ʹ�ã�����Ҫ�����ṩ
											// ��������û������յĻ�ȡ��Ӧ�÷��ظ�ֵ
#define RET_SUCCESS				7			// ִ�гɹ������ݽ�������뵽��������

// Event����ӳ�亯��
#define	DECLEAR_HANDLER_MAP(ModuleClass)  \
private:   \
	typedef void (ModuleClass##::*PParserHandler)(HTTPCONTEXT* pContext );     \
	typedef struct _EventHanderMapEntries   \
	{   \
		EventValue		nEventValue;   \
		PParserHandler		pfEventHandler;   \
	} ParserHandlerMapEntries;   \
	static ParserHandlerMapEntries m_eventMap[];  \
	\
	static const ParserHandlerMapEntries* GetThisEventMap()  \
	{   \
		return &m_eventMap[0];  \
	};

#define	BEGIN_HANDLER_MAP( ModuleClass ) \
	ModuleClass##::ParserHandlerMapEntries ModuleClass##::m_eventMap[] ={ \

#define	ON_HANDLER( event_value, event_handler)  \
	{ event_value,  (PParserHandler)&event_handler },

#define	END_HANDLER_MAP() \
	{ 0, (PParserHandler)NULL}};



#define PROCESS_HANDLER(evt) \
	EventValue ev = evt.eventValue;   \
	ASSERT( ev != HANDLER_VALUE_INVALID);    \
	\
	const ParserHandlerMapEntries* pEntry = GetThisEventMap();   \
	while( pEntry)   \
	{   \
		if(  pEntry->nEventValue == HANDLER_VALUE_INVALID ||   \
			pEntry->nEventValue == 0)   \
			break;   \
			\
		if( pEntry->nEventValue == ev   \
			&& pEntry->pfEventHandler != NULL)   \
		{   \
			(this->*pEntry->pfEventHandler)(&evt);   \
			return;   \
		}   \
		++pEntry;   \
	}


class CRequestManager;
class CResponseParser
{
	DECLEAR_HANDLER_MAP(CResponseParser)

public:
	CResponseParser(void);
	~CResponseParser(void);

	static string	ConvertCharVector( CHAR_VECTOR* pVector);

	void				SetReuqestManager( CRequestManager*	pRequestManager);
	CRequestManager*	GetRequestManager() { return m_pRequestManager;}


	/**
	 * Function		���ݸ�����EventValueֵ�Է��ص���Ӧ���н���
	 * @param		nEventValue ��Ӧ���¼�ֵ
	 * @param		lpszContentData ��Ӧ������
	 * @param		dwContentSize ��ȡ�ı��ĵĳ���
	 * @param		nEltsNum ���ڻ�ȡ�����б���ԣ���Ҫ����һ��ֵ��ֻ�ǵ�ǰ�ĺ��ѵ���Ŀ���Ա�����߷����ڴ�ռ�
	 * @return 
	 **/
	void	ParseResponse( HTTPCONTEXT* pContext);

private:

	//==========================================================
	//              �ڲ�ʹ�õĸ�������
	//==========================================================
	void	ParseGetFavIcon( HTTPCONTEXT* pContext);

private:

	CRequestManager*	m_pRequestManager;		// �������ڻ�ȡ��ͼƬURL��ʱ����Ҫ����CReuqestManager��
};
