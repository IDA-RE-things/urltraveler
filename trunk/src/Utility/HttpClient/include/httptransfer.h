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
#ifndef _HTTPTRANSFERMGR_H
#define _HTTPTRANSFERMGR_H

/*********ʹ�ñض�*****************************/
/*********����˵��*****************************
*********����ģ����ƺ�˵��*******************
***** STATIC_COMPILE   --- �綨��˺꣬��Ϊ���뾲̬��汾��
* ����Ĭ��Ϊ��̬��汾

***** HTTPTRANSFER_RELEASE --- �綨��˺꣬������LOGģ�飬
* ����Ҫ����LOGģ�����õ�utils-mt-gd.lib�⣬
* Ҳ�������Զ�����Ӧ����д�ļ�����
**********************************************
**********************************************
**********************************************/


#ifdef WIN32

#ifndef STATIC_COMPILE
#ifdef HTTPCLIENT_EXPORTS
#define HTTPCLIENT_EXPORTS __declspec(dllexport)
#else
#define HTTPCLIENT_EXPORTS __declspec(dllimport)
#endif
#else
#define HTTPCLIENT_EXPORTS
#endif
#else
#define HTTPCLIENT_EXPORTS 
#endif

#ifdef WIN32
#include <process.h>
//#include <windows.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <list>


#include "Types.h"


using namespace std;

//#define MAX_TASKQUE_SIZE 1024

typedef vector<char> CHAR_VECTOR;		//���ڶ������������洢

/******************************************************************************
* TASK_TYPE - ������������
* ע:
* TASK_DOWNLOAD �����ֻ��httpͷ����������
* TASK_QUERY �� TASK_UPLOAD Ϊʹ��Multi-parts��ʽ��������
* TASK_BINARY Ϊʹ��2�������ݷ�ʽ��������, ����ֵ�洢��POST_PARAMS�У�
  ֻ����һ���������Ҳ������ͱ���Ϊ PARAM_BINARY, ������Ϊanonymous
* DESCRIPTION: - 
***/
typedef enum
{
	TASK_DOWNLOAD = 0,		//ʹ��Get�ӷ����������ļ�
	TASK_QUERY    = 1,		//ʹ��Post���������ѯ�������
	TASK_UPLOAD   = 2,		//ʹ��Post��������ϴ��ļ�
	TASK_BINARY   = 3,		//ʹ��Post�����������2���Ʋ�������---��Ҫ���⴦��
	TASK_HEAD     = 4,		//ʹ��HEAD���������ȡhttp��Ӧͷ
	TASK_BUTT = 255,
}TASK_TYPE;

/******************************************************************************
* ProxyType - ����ʹ�õĴ�������
* DESCRIPTION: - 
***/
typedef enum 
{	
	HTTPDOWN_PROXY_HTTP = 0,			//ʹ��http����
	HTTPDOWN_PROXY_SOCK4 = 1,		//ʹ��sock4����
	HTTPDOWN_PROXY_SOCK5 = 2,		//ʹ��sock5����	
	HTTPDOWN_PROXY_BUTT = 255,
}HTTPDOWN_ProxyType;


/******************************************************************************
* Proxy_t - �������������Ϣ���ݲ�����
* DESCRIPTION: - 
***/
typedef struct _Proxy_t
{
	HTTPDOWN_ProxyType	type;		//����
	string		host;		//����,�����������ַ
	string		user;		//ѡ��
	string		pwd;		//ѡ��
	string		domain;		//ѡ��
	int			port;		//����
}Proxy_t;

class THttpMsg;

typedef struct _TaskInfo
{
		//TaskStatus status;		//����ǰ״̬
		//UINT32 taskid;			//�������
		TASK_TYPE	taskType;		//��������
		LONG64		contentLength;	//Ŀ���ļ���С --- -1����δ֪��С
		ULONG64		doneSize;		//��������ɵ��ֽ���
		ULONG64		transferSpeed; 	//�������ʣ���ByteΪ��λ
		LONG64		remainTime;		//���Ƶ�ʣ��ʱ�� --- -1����δ֪��С
		ULONG64		addTime;			//�������ʱ��
		ULONG64		startTime;		//����ʼʱ��
		ULONG64		endTime;		   	//�������ʱ��
		INT32		errcode;			//����Ĵ����� --- ���ڵ��ô���ص���������Ч
		INT32		httpCode;		//http ��Ӧ��
		THttpMsg*	htrequest;	//��Ӧͷ��Ϣ		
		THttpMsg*	htresponse;	//��Ӧͷ��Ϣ
		char		infomsg[1024];  	//������ı���Ϣ
		CHAR_VECTOR* respData;	//��Ӧ������ 
}TaskInfo;

typedef enum
{
	ERR_TASK_OK = 0,
	ERR_TASK_FULL,
	ERR_TASK_PROXY_CONNECT_FAIL,
	ERR_TASK_PROXY_AUTH_FAIL,
	ERR_TASK_THREAD_START_FAIL,
	ERR_TASK_MGR_NOT_START,		//---5

	ERR_TASK_ALREADY_STARTED,	//�ظ��������󣬿�����֮ǰ��stop������δ��ɵ���
	ERR_TASK_SOCKET_INIT_FAIL,
	ERR_TASK_SOCKET_WRONG,
	ERR_TASK_UNKNOWN_STATUS,
	ERR_TASK_NOT_EXIST,			//---10
								
	ERR_TASK_SOCKET_SEND,
	ERR_TASK_SOCKET_RECV,
	ERR_TASK_CONNECT_FAIL,
	ERR_TASK_REDIRECTION_EXCEED,
	ERR_TASK_REDIRECTION_NOTARGET,	//---15
	
	ERR_TASK_PARSE_RESQ_FAIL,
	ERR_TASK_READ_RESQ_FAIL,

	
	ERR_TASK_PARAM_URL,				//URL ���Ϸ�
	ERR_TASK_PARAM_RESPDATA,		//������������Ϊ��
	ERR_TASK_PARAM_CALLBACK,		//ȱ�ٻص�����  ---20
	
	ERR_TASK_PARAM_POSTPARAM_NONE,	//POST����Ϊ��
	ERR_TASK_PARAM_POSTPARAM_EXCEED,//POST������������
	ERR_TASK_PARAM_POSTPARAM_NAME,	//Post �����Ĳ��������Ϸ�
	ERR_TASK_PARAM_POSTPARAM_LEN,	//Post �����Ĳ���ֵ���Ȳ��Ϸ�
	ERR_TASK_PARAM_POSTPARAM_VALUE, //Post �����Ĳ���ֵ���Ϸ�					---25

	ERR_TASK_PARAM_POSTPARAM_FILENOTEXIST,	//Post ������ָ�����ļ�������		
	ERR_TASK_POSTPARAM_TYPE_INVALID,	//Post �����Ĳ������Ͳ��Ϸ�
	ERR_TASK_OPTION_SIZE_INVALID,
	ERR_TASK_NO_FILE,		
	ERR_TASK_NO_MEMORY,
	ERR_TASK_FILE_READ,							//---30

	ERR_TASK_FILE_CANNOT_OPEN,			
	
	ERR_TASK_FILE_CHANGED,
	ERR_TASK_TIMEOUT,

	//�������ݿ����������
	ERR_TASK_WRITE_FILE_ERROR,			
}ERR_TASK;


typedef enum
{
	PARAM_TEXT = 0,
	PARAM_BINARY = 1,
	PARAM_FILENAME = 2,
}PARAM_TYPE;

/******************************************************************************
* handle_func_t ����ص��������Ͷ��壬�ϲ�Ӧ��Ӧ�ð��˶����е�Լ��ʵ�ֻص�����
* DESCRIPTION: - �ص�����Ӧ�þ����򵥣�Ѹ�ٷ���, �ұ�֤��ȫ�ԣ�tranferMgr���Ե��ûص������е����⸺��
* param1: INT --- Ϊ����ID  param2: TaskInfo* --- Ϊ������ϸ��Ϣָ��
* ������ص������ж�TaskInfo�����޸�
* ����ֵ:Լ�� 0 ---�ɹ� <0 ---ʧ�� >0 ---�������
***/
typedef INT32     (*handle_func_t)     (INT32, const TaskInfo*);

struct HTTPCLIENT_EXPORTS CALL_BACK_S			//�ص���������
{
	CALL_BACK_S()
	{
		handle_init = NULL;
		handle_headcomplete = NULL;
		handle_process = NULL;
		handle_finish = NULL;
		handle_error = NULL;
	}

	handle_func_t handle_init;			//�����ʼ����ɺ�Ļص�����
	handle_func_t handle_headcomplete;	//�յ�������Ӧͷʱ�Ļص�����
	handle_func_t handle_process;		//��������еĻص�����	
	handle_func_t handle_finish;		//������ɺ�Ļص����� ---�������
	handle_func_t handle_error;			//�������ʱ���õĻص����� ---�������
};

struct HTTPCLIENT_EXPORTS OPTIONS		//����ѡ���
{
	OPTIONS()
	{
		tasktimeout = 0;
		startSize = 0;
		endSize = 0;
		dataClean = 0;
	}

	UINT		  tasktimeout;
	ULONG64		  startSize;			//���ζ�ȡhttp��Ӧ�����ʼ�ֽڣ���Ϊ0���������
	ULONG64		  endSize;				//���ζ�ȡhttp��Ӧ��Ľ����ֽڣ���Ϊ0���������
	bool          dataClean;			//�Ƿ���ÿ�λص�֮�����յ���������գ��ѽ�ʡ�ڴ�
};

const int MAX_TASK_NUM = 1024;


#define MAX_PARAM_NAME_LEN 64
typedef struct
{
	char paramName[MAX_PARAM_NAME_LEN]; 	//�洢������

	//PARAM_TEXT:�˴��洢��param��ֵ
	//PARAM_FILE:�˴��洢�ϴ��ļ����ļ�·��
	//PARAM_BINARY:�˴��洢���ϴ���2��������
	char* paramBuffer;
	
	//PARAM_TEXT:��Ч
	//PARAM_FILE:�˴��洢�ϴ����ļ�ʱ��ƫ�� ---���ó����ļ���С
	//PARAM_BINARY:�˴��洢paramBuffer��2�������ݵĳ���
	UINT32 paramBufferLen;
	PARAM_TYPE paramType;
}POST_PARAM;

typedef vector<POST_PARAM> PARAMS_V;//���ڴ洢Post�Ĳ����б�


class THttpHdr;
typedef std::list<THttpHdr*> THttpHdrList;


#define HTTPTRANSFER_LOG_LEVEL 5

#endif  /* _HTTPTRANSFERMGR_H */

