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
		EVENT_VALUE_WEB_DOWNLOAD_FILE_RESP	=	EVENT_VALUE_WEB_BEGIN,		
		EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP,
		EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP,
		EVENT_VALUE_WEB_GET_FAVICON_RESP,						// ��ȡ��ͼ�����Ӧ�¼�
	};

	// Web�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_WebMessageValue
	{
	};

	// Web�ܹ������ֱ�ӵ���
	enum E_WebServiceValue
	{
		// Favicon��Ӧ��ͼ��
		SERVICE_VALUE_WEB_GET_FAVICON = SERVICE_VALUE_WEB_BEGIN,	// ֪ͨȥ��ȡFavoriteͼ��
		SERVICE_VALUE_WEB_OPEN_URLTRAVELER,						// ��URLTRAVELER���
		SERVICE_VALUE_WEB_CLOSE_URLTRAVELER,						// �ر�URLTRAVELER���
		SERVICE_VALUE_WEB_LOGININ_URLTRAVELER,					// ע���¼�û�
		SERVICE_VALUE_WEB_LOGINOUT_URLTRAVELER,					// �ǳ��û�
		SERVICE_VALUE_WEB_REPORT_USERINFO,						// �ϱ��û���Ϣ
		SERVICE_VALUE_WEB_GET_VISITED_URL,							// �ӷ�������ȡ��Ҫ���ʵ�URL�б�
		SERVICE_VALUE_WEB_DOWNLOAD_FILE,						// ���ظ������ļ�

		SERVICE_VALUE_WEB_CHECK_UPDATE_CONFIG,					// ��ȡ����������Ϣ
		SERVICE_VALUE_WEB_DOWNLOAD_UPDATE_FILE,					// ������Ҫ���µ��ļ�

		SERVICE_VALUE_WEB_FAVICON_LOAD ,							//	�����ض���URL���ղؼ�ͼ��
		SERVICE_VALUE_WEB_QUERY_DOWNLOAD_FILE_PROESS,			// ��ѯ�������ص��ļ��Ľ���
	};

	// ���ظ��ϲ�����ߵĴ�����Ӧ��Ϣ
	enum E_WebRespCode
	{
		WEB_RET_SUCCESS						=		0,			// �ɹ���ȡ���û�����Ϣ
		WEB_RET_INNER_USED					=		1,
		WEB_RET_DOWNLOAD_ERROR			=		2,			// ����ʧ�ܣ���������
		WEB_RET_UNKNOWN					=		3,	// δ֪�Ĵ���

		// ������ص���Ϣ
		WEB_RET_NET_ERROR_URL				=		100,	// �����URL����
		WEB_RET_NET_ERROR_CONNECT			=		101,	// �޷�����Զ�̷�����
		WEB_RET_NET_ERROR_OPEN_REQUEST		=		102,	// �޷�������
		WEB_RET_NET_ERROR_SETCALLBACK		=		103,	// ����Callback����ʧ��
		WEB_RET_NET_ERROR_SENDREQUEST		=		104,	// ��������ʧ��
		WEB_RET_NET_ERROR_SET_OPTION			=		105,	// ����ѡ�����
		WEB_RET_NET_ERROR_QUERY_HEADER		=		106,	// ��ѯHTTPͷʧ��
		WEB_RET_NET_ERROR_QUERY_BODY_DATA	=		107,	// ��ѯ����������ʧ��
		WEB_RET_NET_ERROR_READ_DATA			=		108,	// ��ȡ����������ʧ��
		WEB_RET_NET_ERROR_RECEIVE_BODY_DATA	=		109,	// ���ܱ���������ʧ��
		WEB_RET_NET_ERROR_TIMEOUT			=		110,	// ���ܱ��ĳ�ʱ
		WEB_RET_NET_ERROR_ADDHEADER			=		111,	// ����HTTPͷʧ��
		WEB_RET_NET_ERROR_OPEN_SESSION		=		112,	// ��HTTP�Ựʧ��

		// ͨ�ô�������
		WEB_RET_COMMON_NO_URL					=		200,	// �����URL������
		WEB_RET_COMMON_METHOD_NOT_SUPPORT	=		201,	// �ͻ�������ķ�����֧��
		WEB_RET_COMMON_NO_PERMIT				=		202,	// �ͻ����������Դ��Ȩ�޷���
		WEB_RET_COMMON_NO_AUTH				=		203,	// �ͻ���û��ͨ����Ȩ
		WEB_RET_COMMON_SERVER_INTERNAL_ERROR=		204,	// �������ڲ�����
		WEB_RET_COMMON_SERVER_NOT_AVAIL		=		205,	// ��������ʱ������
		WEB_RET_COMMON_SERVER_NOT_IMPL		=		206,	// ��������δʵ�����󷽷�
		WEB_RET_COMMON_PROXY_NEED_AUTH		=		207,	// �����������Ҫ��֤

		WEB_RET_COMMON_NO_BODY_DATA			=		220,	// û�н��յ�����������
		WEB_RET_COMMON_NO_WEB_DATA			=		221,	
		WEB_RET_COMMON_NOT_LOGIN				=		222,	// �û���δ��¼
		WEB_RET_COMMON_ERROR_PARAM			=		223,	// ����Ĳ�������
		WEB_RET_COMMON_ERROR_TOKEN			=		224,	// �����TOKEN����
		WEB_RET_COMMON_ERROR_AUTH				=		225,	// �����auth����
		WEB_RET_COMMON_TOKEN_TIMEOUT			=		226,	// Token��ʱ
		WEB_RET_COMMON_ERROR_INTERNAL			=		230,	// XML�ڲ�������
		WEB_RET_COMMON_WRITE_FILE_ERROR			=		231,	// д���ļ�ʧ��
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
	struct WebReqEvent	:	public ExtraInfo
	{
		WebReqEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_WEB;
		}
	};

	struct WebRespEvent	:	public ExtraInfo
	{
		WebRespEvent()
		{
			srcMId	=	MODULE_ID_WEB;
			desMId	=	MODULE_ID_INVALID;
			param0	=	WEB_RET_SUCCESS;
		}
	};

	// �����з��ص���Ӧ����
	struct Web_GetFavIconRespEvent	:	public WebRespEvent
	{
		Web_GetFavIconRespEvent()
		{
			eventValue	=	 EVENT_VALUE_WEB_GET_FAVICON_RESP;

			nIconSize = 0;
			pIconData = NULL;
		}

		~Web_GetFavIconRespEvent()
		{
			if( pIconData != NULL)
			{
				nIconSize = 0;

				delete pIconData;
				pIconData = NULL;
			}
		}

		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
		int nIconSize;
		char*	pIconData;
	};

	// ��鵱ǰ�Ƿ��и�����Ϣ
#define MAX_UPDATE_XML_LENGTH	10240
	struct Web_CheckUpdateConfigRespEvent	:	public WebRespEvent
	{
		Web_CheckUpdateConfigRespEvent()
		{
			eventValue	=	 EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP;
			ZeroMemory(szUpdateXml, MAX_UPDATE_XML_LENGTH * sizeof(wchar_t));
		}

		// ���ص�xml���ģ����Ϊ10240����
		wchar_t	szUpdateXml[MAX_UPDATE_XML_LENGTH];
	};

	struct Web_DownloadUpdateFileRespEvent	:	public WebRespEvent
	{
		Web_DownloadUpdateFileRespEvent()
		{
			eventValue = EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP;
		}

		int nId;	//	�Ѿ�������ϵĸ����ļ���ID
	};

	//===========================================//
	//                   Web����ʹ�õ���Service						      //
	//===========================================//
	struct Web_Service	:	public	Service
	{
		Web_Service()
		{
			srcMId = MODULE_ID_INVALID;

			bAllowOffline	=	FALSE;
			bAllowRepeated	=	FALSE;
		}

		int	srcMId;	//	Դģ��

		bool	bAllowOffline;		//	�Ƿ��������߲���, TRUEΪ����FALSEΪ������
		bool	bAllowRepeated;		//	��URL�Ƿ������ظ�
	};


	// �����������ղؼ�ICONͼ��
	struct Web_GetFavIconqService	:	public Web_Service
	{
		Web_GetFavIconqService()
		{
			serviceId	=	 SERVICE_VALUE_WEB_GET_FAVICON;
			ZeroMemory(szFavoriteUrl, MAX_PATH * sizeof(wchar_t));
		}

		// �ղؼ�ͼ���URL
		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
	};

	// ���������ʱ��֪ͨ���������Ӽ���
	struct Web_OpenTravelerService	:	public Web_Service
	{
		Web_OpenTravelerService()
		{
			serviceId	=	 SERVICE_VALUE_WEB_OPEN_URLTRAVELER;
		}
	};

	struct Web_CloseTravelerService	:	public Web_Service
	{
		Web_CloseTravelerService()
		{
			serviceId	=	 SERVICE_VALUE_WEB_CLOSE_URLTRAVELER;
		}
	};

	// �����紫�͵�ǰ�û�����Ϣ
	struct Web_ReportUserInfoService	:	public Web_Service
	{
		Web_ReportUserInfoService()
		{
			serviceId	=	 SERVICE_VALUE_WEB_REPORT_USERINFO;
			ZeroMemory(szMachinedId, 64 * sizeof(wchar_t));
			ZeroMemory(szOSVersion, 64 * sizeof(wchar_t));
		}

		// �ղؼ�ͼ���URL
		wchar_t	szMachinedId[64];			//	����ID
		wchar_t	szOSVersion[64];			//	����ϵͳ�汾
	};

	struct Web_CheckUpdateConfigService	:	public Web_Service
	{
		Web_CheckUpdateConfigService()
		{
			serviceId	=	 SERVICE_VALUE_WEB_CHECK_UPDATE_CONFIG;
		}
	};

	// ������Ҫ���µ��ļ�
	struct Web_DownloadUpdateFileService	:	public Web_Service
	{
		Web_DownloadUpdateFileService()
		{
			serviceId = SERVICE_VALUE_WEB_DOWNLOAD_UPDATE_FILE;

			nId = 0;
			ZeroMemory(szUpdateFileUrl, MAX_PATH * sizeof(wchar_t));
			ZeroMemory(szSavePath, MAX_PATH * sizeof(wchar_t));
		}

		int	nId;
		//	��Ҫ���ص��ļ���URL
		wchar_t	szUpdateFileUrl[MAX_PATH];
		wchar_t	szSavePath[MAX_PATH];	//	���ش�ŵ�·��
	};

	struct Web_QueryDownFileProcessService	:	public Web_Service
	{
		Web_QueryDownFileProcessService()
		{
			serviceId	=	SERVICE_VALUE_WEB_QUERY_DOWNLOAD_FILE_PROESS;
		}

		uint32	nSeqNo;						//	��ǰ������������к�

		uint32	uPercent;						//	���ؽ��ȣ� 50 ��ʾ ���50%
		uint32	uTotalLength;					//	���ص��ܳ���
		uint32	uFinishedLength;				//	�Ѿ�������ϵ��ܳ���
		uint32	uSpeed;						//	���ʣ�
		int32	uRemainedTime;				//	
	};

};