// ���������
#ifndef _HTTPCONTEXT_H_
#define _HTTPCONTEXT_H_

#include <string>
#include "WebDefine.h"
#include "time.h"
#include "httpmanager.h"

#define SUCCESS	1
#define FAILED	0

enum RequestType
{
	/**
	*	���յ���������ȫ�ǻ���������Щ���ݽ�һ�����ύ��Parser����
	*/
	ONCE_BUFFER = 0,

	/**
	*	���ܵ���������ȫ�ǻ���������Щ���ݲ���һ�����ύ��parser�����Ƿֺö���ύ
	*/
	FRAG_BUFFER,

	/**
	*	���յ����������ļ����ݣ���Щ�����ڶ�ȡ����ʱ��ͻ�д���ļ���
	*/
	ONCE_FILE,
};

#define		INVALID_RESP	0			//	��Ч����Ӧ
#define		REPEATED_REQ	1			//	��������ʧ�ܣ����ظ����д���
#define		SUCCESS_RESP	2			//	��Ӧ�ɹ�


// ÿһ�������������
typedef struct HTTPCONTEXT
{
	HTTPCONTEXT(string& url)
	{
		uOpPolicy				=	SUCCESS_RESP;			//	�Ը������Ĵ���Ĳ���

		nEventValue			=	0;						//	IN ��ǰ��Ӧ���¼�ֵ
		strURL				=	url;						//	IN ��ǰ�����URL
		eReqType				=	ONCE_BUFFER;				//	IN ���������
		pArgument			=	NULL;					//	IN ��������Ҫ��Post����
		nTimeout				=	0;						//	IN ��ʱ
		wstrWritedFileName		=	L"";						//	IN д����ļ�������
		uParam0				=	0;						//	����Ĳ���
		uParam1				=	0;						//	����Ĳ���
		uParam2				=	0;						//	����Ĳ���

		/**
		*	��������
		*/
		nReqSeq				=	0;						//	OUT ��ǰ��������к�
		nTaskId				=	0;						//	OUT ��ǰ���кŶ�Ӧ���ڲ�ID
		uAddTime				=	time(NULL);				//	OUT ������е�ʱ��
		uEndTime				=	time(NULL);				//	OUT	�������ʱ��
		nFailedTime			=	0;						//	OUT ����ʧ�ܵĴ���
		nSpeed				=	0;						//	OUT	�������ʣ�xxK/��
		nRemainedTime		=	0;						//	OUT	���Ƶ�ʣ��ʱ��

		uResult				=	SUCCESS;					//	OUT ���β����Ľ�����SUCCESS OR FAILED	
		nErrorCode			=	0;						//	OUT ����Ĵ�����
		nHttpCode			=	200;						//	OUT HTTP��Ӧ��

		hFileHandle			=	NULL;					//	OUT �ļ��ľ��
		nContentLength		=	0;						//	OUT �������ĵĴ�С
		nFinishedLength		=	0;						//	OUT �Ѿ���ȡ�ı��ĵĳ���
		strContentData			=	"";						//	OUT ���ζ�ȡ�����ݣ�����FRAG_BUFFER���ԣ�����һ�ζ�ȡ�����ݣ�����ȫ��
		bAllowRepeated		=	TRUE;
	}

	HTTPCONTEXT()
	{
		uOpPolicy				=	SUCCESS_RESP;			//	�Ը������Ĵ���Ĳ���

		nEventValue			=	0;						//	IN ��ǰ��Ӧ���¼�ֵ
		strURL				=	"";						//	IN ��ǰ�����URL
		eReqType				=	ONCE_BUFFER;			//	IN ���������
		pArgument			=	NULL;					//	IN ��������Ҫ��Post����
		nTimeout				=	30;						//	IN ��ʱ
		wstrWritedFileName		=	L"";					//	IN д����ļ�������
		uParam0				=	0;						//	����Ĳ���
		uParam1				=	0;						//	����Ĳ���
		uParam2				=	0;						//	����Ĳ���

		/**
		*	��������
		*/
		nReqSeq				=	0;						//	OUT ��ǰ��������к�
		nTaskId				=	0;						//	OUT ��ǰ���кŶ�Ӧ���ڲ�ID
		uAddTime				=	time(NULL);				//	OUT ������е�ʱ��
		uEndTime				=	time(NULL);				//	OUT	�������ʱ��
		nFailedTime			=	0;						//	OUT ����ʧ�ܵĴ���
		nSpeed				=	0;						//	OUT	�������ʣ�xxK/��
		nRemainedTime		=	0;						//	OUT	���Ƶ�ʣ��ʱ��

		uResult				=	SUCCESS;				//	OUT ���β����Ľ�����SUCCESS OR FAILED	
		nErrorCode			=	0;						//	OUT ����Ĵ�����
		nHttpCode			=	200;					//	OUT HTTP��Ӧ��

		hFileHandle			=	NULL;					//	OUT �ļ��ľ��
		nContentLength		=	0;						//	OUT �������ĵĴ�С
		nFinishedLength		=	0;						//	OUT �Ѿ���ȡ�ı��ĵĳ���
		strContentData			=	"";						//	OUT ���ζ�ȡ�����ݣ�����FRAG_BUFFER���ԣ�����һ�ζ�ȡ�����ݣ�����ȫ��
		bAllowRepeated		=	TRUE;
	}

	HTTPCONTEXT( HTTPCONTEXT* pContext)
	{
		uOpPolicy				=	pContext->uOpPolicy;			//	�Ը������Ĵ���Ĳ���
		nEventValue			=	pContext->nEventValue;			//	IN ��ǰ��Ӧ���¼�ֵ
		strURL				=	pContext->strURL;				//	IN ��ǰ�����URL
		eReqType				=	pContext->eReqType;				//	IN ���������
		nTimeout				=	pContext->nTimeout;				//	IN ��ʱ
		wstrWritedFileName		=	pContext->wstrWritedFileName;	//	IN д����ļ�������
		uParam0				=	pContext->uParam0;				//	����Ĳ���
		uParam1				=	pContext->uParam1;				//	����Ĳ���
		uParam2				=	pContext->uParam2;				//	����Ĳ���

		/**
		*	��������
		*/
		nReqSeq				=	pContext->nReqSeq;				//	OUT ��ǰ��������к�
		nTaskId				=	pContext->nTaskId;				//	OUT ��ǰ���кŶ�Ӧ���ڲ�ID
		uAddTime				=	pContext->uAddTime;				//	OUT ������е�ʱ��
		uEndTime				=	pContext->uEndTime;				//	OUT	�������ʱ��
		nFailedTime			=	pContext->nFailedTime;			//	OUT ����ʧ�ܵĴ���
		nSpeed				=	pContext->nSpeed;				//	OUT	�������ʣ�xxK/��
		nRemainedTime		=	pContext->nRemainedTime;		//	OUT	���Ƶ�ʣ��ʱ��

		uResult				=	pContext->uResult;				//	OUT ���β����Ľ�����SUCCESS OR FAILED	
		nErrorCode			=	pContext->nErrorCode;			//	OUT ����Ĵ�����
		nHttpCode			=	pContext->nHttpCode;			//	OUT HTTP��Ӧ��

		hFileHandle			=	pContext->hFileHandle;			//	OUT �ļ��ľ��
		nContentLength		=	pContext->nContentLength;		//	OUT �������ĵĴ�С
		nFinishedLength		=	pContext->nFinishedLength;		//	OUT �Ѿ���ȡ�ı��ĵĳ���
		strContentData			=	pContext->strContentData;		//	OUT ���ζ�ȡ�����ݣ�����FRAG_BUFFER���ԣ�����һ�ζ�ȡ�����ݣ�����ȫ��
		if( pContext->pArgument == NULL)
			pArgument = NULL;
		else
			pArgument = new POST_PARAMS( *pContext->pArgument);

		bAllowRepeated		=	pContext->bAllowRepeated;

	}

	~HTTPCONTEXT()
	{
	}

	/**
	*	������������
	*/
	// ������������Ľ��
	uint32			uOpPolicy;			//	�Ը������Ĵ���Ĳ���

	EventValue		nEventValue;		//	IN ��ǰ��Ӧ���¼�ֵ
	string			strURL;				//	IN ��ǰ�����URL
	RequestType		eReqType;			//	IN ���������
	POST_PARAMS*	pArgument;			//	IN ��������Ҫ��Post����
	UINT			nTimeout;			//	IN ��ʱ
	wstring			wstrWritedFileName;	//	IN д����ļ�������
	bool			bAllowRepeated;		//	IN �Ƿ�����������ͬ��URL
	int64			uParam0;			//	����Ĳ���
	int64			uParam1;			//	����Ĳ���
	int64			uParam2;			//	����Ĳ���

	/**
	*	��������
	*/
	uint32			nReqSeq;			//	OUT ��ǰ��������к�
	uint32			nTaskId;			//	OUT ��ǰ���кŶ�Ӧ���ڲ�ID
	time_t			uAddTime;			//	OUT ������е�ʱ��
	time_t			uEndTime;			//	OUT	�������ʱ��
	uint8			nFailedTime;		//	OUT ����ʧ�ܵĴ���
	uint32			nSpeed;				//	OUT	�������ʣ�xxK/��
	uint32			nRemainedTime;		//	OUT	���Ƶ�ʣ��ʱ��

	uint8			uResult;			//	OUT ���β����Ľ�����SUCCESS OR FAILED	
	uint32			nErrorCode;			//	OUT ����Ĵ�����
	uint32			nHttpCode;			//	OUT HTTP��Ӧ��

	HANDLE			hFileHandle;		//	OUT �ļ��ľ��
	int32			nContentLength;		//	OUT �������ĵĴ�С
	int32			nFinishedLength;	//	OUT �Ѿ���ȡ�ı��ĵĳ���
	string			strContentData;		//	OUT ���ζ�ȡ�����ݣ�����FRAG_BUFFER���ԣ�����һ�ζ�ȡ�����ݣ�����ȫ��

} HTTPCONTEXT;

#endif