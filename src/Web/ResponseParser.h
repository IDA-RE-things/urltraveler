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
#include "ximage.h"
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


class CWebData;
class CRequestManager;
class CResponseParser
{
public:
	CResponseParser(void);
	~CResponseParser(void);

	static string	ConvertCharVector( CHAR_VECTOR* pVector);

	void			SetWebData( CWebData* pWebData);
	CWebData*	GetWebData( );

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
	//static void	ParseGetUpdateConfigXml( CResponseParser* pParser, HTTPCONTEXT* pContext);

private:

	CRequestManager*	m_pRequestManager;		// �������ڻ�ȡ��ͼƬURL��ʱ����Ҫ����CReuqestManager��
												// �����䷵��ͼƬ����
	CWebData*			m_pWebData;				// ��������

	// ������ָ��
	typedef void (*ParseHandler)( CResponseParser* pParser, HTTPCONTEXT* pContext );
	typedef struct _HanderTable
	{
		EventValue		m_nEventValue;		//	��Ӧ���¼�ֵ
		ParseHandler	m_hHandler;			//	������

	} HandlerTableEntry;
	static HandlerTableEntry m_tableDriven[];
};
