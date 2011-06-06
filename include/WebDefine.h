#pragma once

// Web�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace web
{
	/**
	* �������������Ӧ���
	*/
	#define INVALID_SEQNO	0		// ��Ч�����к�
	#define READY_SEQNO		1		// �����Ѿ�����������Ҫȥ��վ����
	#define NOT_READY_SEQNO	2		// ����Ҫ������վ���������ݲ�����

	// Web���⹫�����¼�
	enum E_WebEventValue
	{
		// Favicon��Ӧ��ͼ��
		EVENT_VALUE_WEB_GET_FAVICON = EVENT_VALUE_WEB_BEGIN,		// ֪ͨȥ��ȡFavoriteͼ��
	};

	// Web�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_WebMessageValue
	{
	};

	// Web�ܹ������ֱ�ӵ���
	enum E_WebServiceValue
	{
		SERVICE_VALUE_WEB_FAVICON_LOAD = SERVICE_VALUE_WEB_BEGIN,					//	�����ض���URL���ղؼ�ͼ��
	};

	// ���ظ��ϲ�����ߵĴ�����Ӧ��Ϣ
	enum E_WebRespCode
	{
		WEB_RET_SUCCESS						=		5000,			// �ɹ���ȡ���û�����Ϣ
		WEB_RET_INNER_USED					=		5002,
		WEB_RET_DOWNLOAD_ERROR				=		5003,			// ����ʧ�ܣ���������

		// ������ص���Ϣ
		WEB_RET_NET_ERROR_URL				=		900,	// �����URL����
		WEB_RET_NET_ERROR_CONNECT			=		901,	// �޷�����Զ�̷�����
		WEB_RET_NET_ERROR_OPEN_REQUEST		=		902,	// �޷�������
		WEB_RET_NET_ERROR_SETCALLBACK		=		903,	// ����Callback����ʧ��
		WEB_RET_NET_ERROR_SENDREQUEST		=		904,	// ��������ʧ��
		WEB_RET_NET_ERROR_SET_OPTION		=		905,	// ����ѡ�����
		WEB_RET_NET_ERROR_QUERY_HEADER		=		906,	// ��ѯHTTPͷʧ��
		WEB_RET_NET_ERROR_QUERY_BODY_DATA	=		907,	// ��ѯ����������ʧ��
		WEB_RET_NET_ERROR_READ_DATA			=		908,	// ��ȡ����������ʧ��
		WEB_RET_NET_ERROR_RECEIVE_BODY_DATA	=		909,	// ���ܱ���������ʧ��
		WEB_RET_NET_ERROR_TIMEOUT			=		910,	// ���ܱ��ĳ�ʱ
		WEB_RET_NET_ERROR_ADDHEADER			=		911,	// ����HTTPͷʧ��
		WEB_RET_NET_ERROR_OPEN_SESSION		=		912,	// ��HTTP�Ựʧ��

		// ͨ�ô�������
		WEB_RET_COMMON_NO_URL				=		1000,	// �����URL������
		WEB_RET_COMMON_METHOD_NOT_SUPPORT	=		1001,	// �ͻ�������ķ�����֧��
		WEB_RET_COMMON_NO_PERMIT			=		1002,	// �ͻ����������Դ��Ȩ�޷���
		WEB_RET_COMMON_NO_AUTH				=		1003,	// �ͻ���û��ͨ����Ȩ
		WEB_RET_COMMON_SERVER_INTERNAL_ERROR=		1004,	// �������ڲ�����
		WEB_RET_COMMON_SERVER_NOT_AVAIL		=		1005,	// ��������ʱ������
		WEB_RET_COMMON_SERVER_NOT_IMPL		=		1006,	// ��������δʵ�����󷽷�
		WEB_RET_COMMON_PROXY_NEED_AUTH		=		1007,	// �����������Ҫ��֤

		WEB_RET_COMMON_NO_BODY_DATA			=		1020,	// û�н��յ�����������
		WEB_RET_COMMON_NO_WEB_DATA			=		1021,	
		WEB_RET_COMMON_NOT_LOGIN			=		1022,	// �û���δ��¼
		WEB_RET_COMMON_ERROR_PARAM			=		1023,	// ����Ĳ�������
		WEB_RET_COMMON_ERROR_TOKEN			=		1024,	// �����TOKEN����
		WEB_RET_COMMON_ERROR_AUTH			=		1025,	// �����auth����
		WEB_RET_COMMON_TOKEN_TIMEOUT		=		1026,	// Token��ʱ
		WEB_RET_COMMON_ERROR_INTERNAL		=		1030,	// XML�ڲ�������
		WEB_RET_COMMON_WRITE_FILE_ERROR		=		1031,	// д���ļ�ʧ��
	};

	//============================================================================//
	//                   Web����ʹ�õ������ݽṹ			                  //
	//============================================================================//
	// ���صĴ�����Ϣ
	enum	ProxyType
	{
		PROXY_HTTP   =0,	// ʹ��HTTP����
		PROXY_SOCK5,		// ʹ��SOCK5����
		PROXY_IE,			// ʹ��IE����
		PROXY_NONE,			// ��ʹ�ô���
	};

	// ÿһ�������������
	typedef struct _ProxyDetail
	{
		ProxyType	 eType;
		std::wstring strServer;
		std::wstring strPort;
		std::wstring strUserName;
		std::wstring strPassword;
		std::wstring strDomain;			// ����

	} PROXYDETAIL, *PPROXYDETAIL;

	// ���صĴ���������Ϣ
	typedef struct _ProxyData
	{
		ProxyType	eLastUsed;			// ���һ��ʹ�õĴ�������
		PROXYDETAIL	arrDetail[2];		// Sock3, HTTP��������IE�����������Ϣ

	}PROXYDATA, *PPROXYDATA;

	//============================================================================//
	//                   Web����ʹ�õ���event�ṹ  			              //
	//============================================================================//
	struct WebEvent	:	public ExtraInfo
	{
		WebEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_WEB;

			m_pstExtraInfo = this;
		}
	};

	// �����������ղؼ�ICONͼ��
	struct Web_GetFavIconEvent	:	public WebEvent
	{
		Web_GetFavIconEvent()
		{
			eventValue	=	 EVENT_VALUE_WEB_GET_FAVICON;
			ZeroMemory(szFavoriteUrl, MAX_PATH);
		}

		// �ղؼ�ͼ���URL
		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
	};
};