/******************************************************************************
 * task.h - �����������ã����ݣ����ȵ���Ϣ
 * 
 * Copyright 1998-2008 51 Co.,Ltd.
 * 
 * DESCRIPTION: - 
 * modification history
 * --------------------
 * 01a, 13nov2008, Davidfan written
 * --------------------
 ******************************************************************************/
#ifndef _TASK_H
#define _TASK_H

#include <string>

#include "httptransfer.h"
//#include "httpmanager.h"


using namespace std;
//#define ShowMsg(str) printf("\n%s\n", str)

#define MAX_REDIRECTIONS 5
#define MAX_RETRY_CONNECT 3
#define MAX_CONNECT_TIMEOUT 30		//unit-seconds


class URL;
class POST_PARAMS;
class HttpHdrs;
class THttpCli;
//cocurrent Task status
typedef enum 
{
	TSK_IDLE,
	TSK_CONNECT,
	TSK_CONNECTWAIT,
	TSK_AUTH,
	TSK_START,				// Get File Size 
	TSK_RUNNING,			
	TSK_FINISHED,			
	TSK_ERROR,
	TSK_UNKNOWN
}TaskStatus; 

typedef enum
{
	CONNECT_SUCCESS = 1,		//�������ӳɹ����ȴ��շ�����
	CONNECT_ASYNCWAIT = 2,		//�첽connect���ȴ�socket����
	CONNECT_ERROR = 3,			//connectʧ��
	CONNECT_PROXYAUTH = 4,		//proxy���ӳɹ�����֤��
}CONNECT_STATUS;

class TTask
{

public:
	TTask(string url, TASK_TYPE task_type);
	TTask(string url, TASK_TYPE task_type, Proxy_t proxyInfo);
	~TTask();

	INT32 sendHttpReq();
	int sendHttpReqContinue();
	
	int processHttpResp();
	INT32 processHttpRespHead();	
	INT32 getRealData(); 
	INT32 tryConnect();	
	INT32 tryAuth();	
	int onAuthResp();
	int onConnected(); 
	INT32 checkConnectTimeout();
	void updateTaskInfo();
	INT32 dumpDataToFile();
	void checkTimeOut();

	TASK_TYPE task_type;
	//NV_MAP* 	  cookies;
	//NV_MAP* 	  postParams;
	//FILENAMES*	  fileNames;
	POST_PARAMS*	  postParams;
	HttpHdrs * httpHdrs;

	handle_func_t handle_init;
	handle_func_t handle_process;
	handle_func_t handle_headcomplete;
	handle_func_t handle_finish;
	handle_func_t handle_error;
	
	CHAR_VECTOR* respData;
	CHAR_VECTOR  respDataContent;
	//CHAR_VECTOR  postContent;
	ULONG64 postDataSize;


	
	THttpCli* hcp;	
	URL * url;
	TaskStatus taskState;
	int errcode;
	TaskInfo taskInfo;
	
	int timeoutlimit;
	ULONG64 last_active_time;
	ULONG64	startSize;			//���ζ�ȡhttp��Ӧ�����ʼ�ֽڣ���Ϊ0���������
	ULONG64	endSize;			//���ζ�ȡhttp��Ӧ��Ľ����ֽڣ���Ϊ0���������
	bool dataClean;			//�Ƿ���ÿ�λص�֮�����յ���������գ��ѽ�ʡ�ڴ�

	int connectTimes;
	int redirectionTimes;
	bool shouldGetFileInfo;
	bool del;
	
	bool respHeadCompleted;			//��Ӧ��Ϣͷ�Ƿ�������
	bool respContentCompleted;		//��Ӧ�������Ƿ�������
	bool dataChanged;				//�ϴ������������Ƿ��б仯 

private:
	void init();	
	int setHttpHeader();
	int generateSendDataBuffer();
	
	bool useProxy;
	
	ULONG64 requestSize;			//ʵ��Ҫ���͵���������С
	ULONG64 requestBufferSize;		//����ķ��ͻ������Ĵ�С
	char* requestBuffer;
	ULONG64 requestSentSize;		//�Ѿ����͵����ݵĴ�С	
};
#endif  /* _TASK_H */
