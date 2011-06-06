/******************************************************************************
 * httptransfer.h - �ⲿӦ��ͨ��TranferMgr��ʹ��HttpTranfer�ĸ���ܡ�
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 *     ���ļ�������TranferMgr�࣬HttpTranfer�������ⲿ�ӿھ�ͨ��TranferMgr�ṩ���ⲿӦ��ʹ��httpDownloaer���Ӧ�Դ��ļ�Ϊ�ӿڱ�׼
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/


#ifndef _HTTPMANAGERMGR_H
#define _HTTPMANAGERMGR_H

#include <string>
#include <vector>

#include "httptransfer.h"

using namespace std;

class TTask;
class TTransferWorker;

class HTTPCLIENT_EXPORTS POST_PARAMS
{
private:
	PARAMS_V postParams;

public:	
	//��paramTypeΪTEXTʱbufferΪparam��ֵ�ַ�����bufferLenΪ�ַ�������+1
	//��paramTypeΪPARAM_BINARYʱbuffer�洢2����������С��bufferLen����
	//��paramTypeΪPARAM_FILENAMEʱbufferΪ�ļ������ַ�����bufferLenΪ�ַ�������+1
	INT32 add_param(const char* paramName,
					const char* buffer,	
					UINT32 bufferLen,
					PARAM_TYPE paramType = PARAM_TEXT);
	
	PARAMS_V* get_params(){	return &postParams; };	

	// ������ϴ�����
	bool	is_uploadtype();

	bool	delete_param( const string strParamName);
	
	POST_PARAMS::~POST_PARAMS();
	POST_PARAMS(POST_PARAMS &in_post_param);
	POST_PARAMS(){};

};

class HTTPCLIENT_EXPORTS THttpHdr
{
private:
    string m_strKey;
    string m_strVal;
public:
    THttpHdr( void );
    THttpHdr( const string& strKey );
    THttpHdr( const string& strKey, const string& strVal );

    const string&  GetKey( void ) const;
    const string&  GetVal( void ) const;
    void  SetVal( const string& strVal );

};


class HTTPCLIENT_EXPORTS THttpMsg
{
private:
    UINT32          m_nHttpVer;         // HTTP version (hiword.loword)
	string 			m_statusLine;
    THttpHdrList    m_listHdrs;
public:
    THttpMsg( void );
    THttpMsg( const THttpMsg& other );
    virtual ~THttpMsg( void );

    // Total header length for key/val pairs (incl. ": " and CRLF)
    // but NOT separator CRLF
		
    size_t		GetAllHdrLen( void ) ;
	size_t		GetHdrLen( UINT nIndex ) ;
	void		ClearAllHdr(void);

    void		GetHttpVer( UINT* puMajor, UINT* puMinor ) const;
    void		SetHttpVer( UINT uMajor, UINT uMinor );

    size_t		GetHdrCount( void ) const;
    string		GetHdr( const string& strKey ) ;
    THttpHdr*	GetHdr( UINT nIndex ) ;
    void		SetHdr( const string& strKey, const string& strVal );
    void		SetHdr( const THttpHdr& hdrNew );
	void		SetHdrDuplicate( const string& strKey, const string& strVal );
	void		SetHdrDuplicate( const THttpHdr& hdrNew );
	
	void		ShowAllHttpHdr(void);

};


//HttpHdrs ά��һ��httpͷ���ϣ������ṩSet,Delete �ӿ�
class HTTPCLIENT_EXPORTS HttpHdrs
{
private:
    THttpHdrList    m_listHdrs;
	
public: 
	//������д��httpͷ����ʹ������httpͷ�ظ�Ҳ����д��
	void		SetHdr( const string& strKey, const string& strVal );
	//����key��ȡhttpͷ��ֵ������ж����key��ֵ���򷵻ص�һ��
	string		GetHdr( const string& strKey);	
	//��ȡ��ͬkey��httpͷ�ĵ�index��Ԫ�أ���cookie
	string		GetHdr( const string& strKey,  int index) ;
	//��ȡ�ڲ�headers����ָ��
	THttpHdrList*	GetHdrs(){return &m_listHdrs;};
	//����keyɾ��httpͷ������ж����key��ֵ����ɾ����һ��ֵ
	void		DeleteHdr( const string& strKey ) ;	
};

class HTTPCLIENT_EXPORTS TranferManager
{
	
public :
	
/******************************************************************************
 * getInstance - ����һ��TranferManagerʵ����ָ�룬���ⲿӦ��ʹ�á���һ�����̽�����һ��TranferManagerʵ��
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	static TranferManager* getInstance();
	static void delInstance(); 
	static TranferManager* instance;

/******************************************************************************
 * setMaxRunningTaskNum - ��������ͬʱ�������ص�������������ﵽ�����������������Ŷ�
 * DESCRIPTION: - 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: ����֮ǰ�����������
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	UINT32 setMaxRunningTaskNum(UINT32 maxRunningTaskNum);

/******************************************************************************
 * setProxy - ���ô����������Ϣ
 * DESCRIPTION: - 
 *	  �ýӿ��û������µ�ȫ�ִ�������������滻֮ǰ�Ĵ�������� 
 *	  ���proxytypeΪHTTPDOWN_PROXY_BUTT����Ϊȡ����ǰ�������ã�������ֻ��֮����ӵ�������Ч
 * Input: 
 * Output: 0�ɹ� <0 ����(��proxyType ���Ͳ�֧��) 1 -�滻֮ǰ�Ĵ�������� 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 setProxy(Proxy_t 	proxy);

/******************************************************************************
 * start - ����HttpTranfer
 * DESCRIPTION: - 
 *	  N/A 
 * Input: 
 * Output: 
 * Returns: 0�ɹ� <0 ����
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 start();

/******************************************************************************
 * addTask - ���һ�����񣬲��ṩ�ص�����
 * DESCRIPTION: 
 * Input:  
  \param url Ŀ�ĵ�ַ(����ʱΪҪ���ص��ļ����ϴ�ʱΪҪ�ϴ����ĵ�ַ)
  \param task_type �������ͣ�upload/download
  \param postParams  ���ڴ洢Post�����б�, ���ϲ�Ӧ�ô��룬httptransfer�������ȿ�����
  				�ϲ����addTask��������ͷ�
  \param httpHdrs    ���ڴ洢httpͷ�б�, ���ϲ�Ӧ�ô��룬httptransfer�����ͷ�
  \param callbacks	 �ص�������
  \param OPTIONS	 ѡ���
 * Output: 
 * Returns: 
 * 
 >0 --- ���񴴽��ɹ������ظ����������ֵ
 <0 --- ���񴴽�ʧ�ܣ����ش�����,�ο�enum����ERR_TASK 
 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 addTask(string url, 
					TASK_TYPE	  task_type,
					POST_PARAMS*	  postParams,
					CALL_BACK_S   callbacks,
					OPTIONS 	  options,
					HttpHdrs * httpHdrs = NULL);


/******************************************************************************
 * clear - ���HttpTranfer�е���������
 * DESCRIPTION: - �˴�����ָ������ɾ�������λ�����ʵ��������û��ֹͣ��
 * 	����ֹͣ�����ɹ����߳�ɨ���������ʱ���. 
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 0�ɹ� <0 ����
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 clear();

/******************************************************************************
 * stop - ֹͣhttptransfer
 * DESCRIPTION: - 
 *	  ֹͣ�����̣߳�ֹͣ�����е���������ֹͣ������Ҫһ��ʱ�������ɣ�
	  �����������������ء�������stop������start�����ܻ�ʧ�� 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 14nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 stop();

/******************************************************************************
 * cancelTask - ɾ��ָ������ͬʱ��ȡָ���������Ϣ
 * DESCRIPTION: - �˴�����ָ������ɾ�������λ�����ʵ��������û��ֹͣ��
 * 	����ֹͣ�����ɹ����߳�ɨ���������ʱ���
 *    N/A 
 * Input: 
 * Output: 
 * Returns: 0 -- �ɹ�  ERR_TASK_NOT_EXIST ---ָ�����񲻴���(�����ѱ�ɾ��)
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 cancelTask(UINT32 taskID, TaskInfo& taskinfo);



/******************************************************************************
 * getTaskInfo - ��ȡָ��task����Ϣ
 * DESCRIPTION: - 
 *    n/a 
 * Input: 
 * Output: 
 * Returns: 0��ȡ�ɹ� <0 ��ȡ����
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 getTaskInfo(UINT32 taskID, TaskInfo& taskinfo);


/******************************************************************************
 * setLogPath - ������־·��
 * DESCRIPTION: - 
 *	  local_log_path Ϊ��־�ļ��ľ���·����httptransfer���ԸĲ�����Ч��(·���Ƿ���ڵ�)
       ���м��, ��windows��local_log_path����Ϊ���¸�ʽ
       "c:\\log\\httptransfer" ---��Ҫ��˫��б��
 * Input: 
 * Output: 
 * 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	void setLogPath(char* local_log_path);
	

	bool mgrStarted;
private:
	TTask** taskQue;
	Proxy_t proxyInfo;
	TTransferWorker* pWorker;

	UINT32 lastTaskQuePos;

	UINT32 waitingTaskCount;
	UINT32 runningTaskCount;
	UINT32 finshedTaskCount;
	UINT32 errorTaskCount;

	UINT32 maxRunTaskNum;
	bool useProxy;

	TranferManager();
	~TranferManager();
	
/******************************************************************************
 * TranferManager.taskParamCheck - �����������Ƿ����������Ҫ��
 * DESCRIPTION: - 
 *	  n/a 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 01a, 24nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
	INT32 taskParamCheck(string url, 
				TASK_TYPE	  task_type,
				POST_PARAMS*  postParams,
				CALL_BACK_S	  callbacks,
				OPTIONS		  options, 
				HttpHdrs * httpHdrs,
				ULONG64& postDataSize);
	
};


#endif  /* _HTTPMANAGERMGR_H */

