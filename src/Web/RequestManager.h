#pragma once

#include <string>
#include "WebDefine.h"
#include "HttpClient.h"
#include "ResponseParser.h"
#include "httpmanager.h"
#include "HttpContext.h"

using namespace std;

/**************************************************************
* ClassName:   CRequestManager
* Author:	   Zhang Z.Q
* Using:	   ���ฺ����HTTP���󣬰�������Ĺ���
*				1) �ӿ��ϲ㴫�ݵ�URL��Ȼ���䷢�͸�Web������
*				2) ����Web����������Ӧ����������Ӧ���ݱ��浽ָ������Ӧ������
* Verion:
*		Created			2007,12,12
**************************************************************/
class CHttpClient;
class CRequestManager
{
public:

	CRequestManager( );
	~CRequestManager();

	void		Init( PROXYDATA* pProxyData);
	void		ChangeProxy( PROXYDATA* pProxyData);
	void		Clear();
	static int	GetRetCodeByStatus( int nStatusCode);
	void		SetProxyData( PROXYDATA* pProxyData);

	/**
	 * ��������URL���뵽�ӳٷ������������
	 * @param		nEventValue �������Ӧ��EventValue
	 * @param		nUin		��������Ӧ��nUin
	 * @param		strUrl		�����Ӧ��URL
	 * @param		pArgument	���������post������Ϊpost�������
	 *							����get�������,�ò������뱻����ΪNULL
	 * @param		eReqType	��������
	 * @param		bAllowRepeated	�������URL�Ƿ������ظ�

	 * @return		���ص�ǰ���������к�
	 **/
	uint32		PushUrl(EventValue nEventValue, int64 nParam0, wstring strUrl, 
						POST_PARAMS* pArgument, RequestType eReqType = ONCE_BUFFER, 
						wstring wstrWritedFileName = L"", UINT nTimeout = 30, BOOL bAllowRepeated= TRUE);
	uint32		PushUrl( HTTPCONTEXT* pContext);

	/**
	* �Ӷ����л�ȡһ���µ�����
	* @param		nEventValue �������Ӧ��EventValue
	* @return		���ص�ǰ���������к�
	**/
	uint8		PopUrl( HTTPCONTEXT*& pContext);

	/**
	 * Function		���������URL
	 * @param		pContext	����������
	 * @param		nTimeout	��ʱʱ�䣬Ĭ����10
	 * @return		
	 **/
	void		ReqUrl( HTTPCONTEXT* pContext);
	void		ReReqUrl( string strUrl,HTTPCONTEXT* pContext, POST_PARAMS* pArgument);

	/**
	 * Function		ȡ��������Task
	 * @param		nSeqNo		��Ҫȡ�������к�
	 */
	void		CancelTask( uint32 nSeqNo);

	HTTPCONTEXT		ProcessResp( );

	/**
	 * Function		��ѯ�������������״̬
	 * @param		nSeqNo	��������кţ�ͨ��CallService����
	 * @param		uPercent		���ص��������
	 * @param		uTotalLength	��Ҫ���ص��ܳ���
	 * @param		uFinishedLength	�Ѿ���ɵĳ���
	 * @param		������ҵ���seqno������true�����򷵻�false
					uPercent��uTotalLength�Լ�uFinishedLengthֻ�з���true��ʱ�����Ч
	 */
	bool		QueryDownloadProcess( uint32 nSeqNo, uint32* uPercent, 
		uint32* uTotalLength, uint32* uFinishedLength,uint32* uSpeed, int32* uRemainedTime);


	/**
	* Function		��������URL�Ƿ��Ѿ������������
	*/	
	BOOL		IsExistUrl( string strUrl);

public:

	CHttpClient*		m_pHttpClient;
	CResponseParser	m_ResponseParser;				// ����Ӧ���Ľ��н���

	unsigned int	m_nReqSeq;
	std::vector< HTTPCONTEXT*>	m_vRequestQueue;	//	�������е�����
};
