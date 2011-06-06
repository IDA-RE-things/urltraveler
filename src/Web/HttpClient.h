#pragma once

#include "string"
#include <list>
#include <map>
#include <vector>
#include "WebDefine.h"
#include "XSync.h"
#include "httpmanager.h"
#include "RequestManager.h"

using namespace std;
using namespace web;

class HttpClient  
{
public:
	virtual ~HttpClient( );

	void	SetProxy( ProxyInfo* pProxyInfo);

	static	HttpClient*	Instance();

	/**
	*	���úͻ�ȡĳһ���е�������
	*/
	HTTPCONTEXT*	GetContext( unsigned int nSeqNo);

	/**
	* Function		ʹ��Get���������ض���URL		
	* @param		strUrl ����Ҫ�����URL������"http://www.51.com"
	* @param		nSeqNo �Ǹ������Ӧ�����к�
	* @return		0, �ɹ�, ���� <0 ʧ�ܣ����صĴ�����
	**/
	int32	ReqURL( HTTPCONTEXT* pContext);
	int32	ReReqURL( HTTPCONTEXT* pContext);

	/**
	* Function		����HTTP Post����
	* @param		strUrl ����Ҫ�����URL	
	* @param		nSeqNo �Ǹ������Ӧ��
	* @param		rArgument ��post����
	* @return 
	**/
	int32	ReqPostUrl( HTTPCONTEXT* pContext);
	int32	ReReqPostUrl( HTTPCONTEXT* pContext);

	/**
	* Function		ȡ���ض�������
	* @param		nSeqNo	��Ҫȡ����SeqNo
	*/
	void	CancelTask( uint32 nSeqNo);

	/**
	* Function		�ӷ��ض�����ȡ��һ����Ӧ����
	* @param		stContext ȡ������Ӧ�����ģ����а����˴ӷ��������صı�������
	* @return 
	**/
	HTTPCONTEXT	RetriveResp( );

	/**
	* Function		ͨ������taskid��ȡ�������Ӧ���ڲ����к�
	* @param		
	*/
	int32 GetSeqNo( int32	nTaskId);
	void	ClearSeqIdMap( uint32 nSeqNo);

	void	Cleanup( HTTPCONTEXT* pSession);
	void	DeleteSeqNo( uint32 nSeqNo);
	void	CleanAll();


private:
	HttpClient( );
	static HttpClient*	Singleton;

private:

	map<uint32, HTTPCONTEXT*>		m_mapRequest;
	map<int32,	uint32>				m_mapTaskid2Seqno;		//	������id���ڲ����кŵ�ӳ��
	map<uint32, int32>				m_mapSeqNo2TaskId;		//	���ڲ����кŵ�����ID��ӳ��

	list<HTTPCONTEXT>				m_lstResult;
	CXCriticalSection				m_Lock;
	TranferManager*					m_pTransferManager;
	CALL_BACK_S						m_Callback;
};