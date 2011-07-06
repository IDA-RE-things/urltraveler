#pragma once

// Web的对外的头文件信息

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace web
{
	/**
	* 定义请求处理的响应结果
	*/
	#define INVALID_SEQNO	0		// 无效的序列号
	#define READY_SEQNO		1		// 数据已经就绪，不需要去网站访问
	#define NOT_READY_SEQNO	2		// 不需要访问网站，但是数据不存在

	// Web对外公开的事件
	enum E_WebEventValue
	{
		EVENT_VALUE_WEB_DOWNLOAD_FILE_RESP	=	EVENT_VALUE_WEB_BEGIN,		
		EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP,
		EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP,
		EVENT_VALUE_WEB_GET_FAVICON_RESP,						// 获取到图标的响应事件
	};

	// Web能够可能对外发送的广播消息
	enum E_WebMessageValue
	{
	};

	// Web能够处理的直接调用
	enum E_WebServiceValue
	{
		// Favicon对应的图标
		SERVICE_VALUE_WEB_GET_FAVICON = SERVICE_VALUE_WEB_BEGIN,	// 通知去拉取Favorite图标
		SERVICE_VALUE_WEB_OPEN_URLTRAVELER,						// 打开URLTRAVELER软件
		SERVICE_VALUE_WEB_CLOSE_URLTRAVELER,						// 关闭URLTRAVELER软件
		SERVICE_VALUE_WEB_LOGININ_URLTRAVELER,					// 注册登录用户
		SERVICE_VALUE_WEB_LOGINOUT_URLTRAVELER,					// 登出用户
		SERVICE_VALUE_WEB_REPORT_USERINFO,						// 上报用户信息
		SERVICE_VALUE_WEB_GET_VISITED_URL,							// 从服务器拉取需要访问的URL列表
		SERVICE_VALUE_WEB_DOWNLOAD_FILE,						// 下载给定的文件

		SERVICE_VALUE_WEB_CHECK_UPDATE_CONFIG,					// 获取更新配置信息
		SERVICE_VALUE_WEB_DOWNLOAD_UPDATE_FILE,					// 下载需要更新的文件

		SERVICE_VALUE_WEB_FAVICON_LOAD ,							//	加载特定的URL的收藏夹图标
		SERVICE_VALUE_WEB_QUERY_DOWNLOAD_FILE_PROESS,			// 查询正在下载的文件的进度
	};

	// 返回给上层调用者的错误响应信息
	enum E_WebRespCode
	{
		WEB_RET_SUCCESS						=		0,			// 成功获取到用户的信息
		WEB_RET_INNER_USED					=		1,
		WEB_RET_DOWNLOAD_ERROR			=		2,			// 下载失败，重新下载
		WEB_RET_UNKNOWN					=		3,	// 未知的错误

		// 网络相关的信息
		WEB_RET_NET_ERROR_URL				=		100,	// 请求的URL错误
		WEB_RET_NET_ERROR_CONNECT			=		101,	// 无法连接远程服务器
		WEB_RET_NET_ERROR_OPEN_REQUEST		=		102,	// 无法打开请求
		WEB_RET_NET_ERROR_SETCALLBACK		=		103,	// 设置Callback函数失败
		WEB_RET_NET_ERROR_SENDREQUEST		=		104,	// 发送请求失败
		WEB_RET_NET_ERROR_SET_OPTION			=		105,	// 设置选项错误
		WEB_RET_NET_ERROR_QUERY_HEADER		=		106,	// 查询HTTP头失败
		WEB_RET_NET_ERROR_QUERY_BODY_DATA	=		107,	// 查询报文体数据失败
		WEB_RET_NET_ERROR_READ_DATA			=		108,	// 读取报文体数据失败
		WEB_RET_NET_ERROR_RECEIVE_BODY_DATA	=		109,	// 接受报文体数据失败
		WEB_RET_NET_ERROR_TIMEOUT			=		110,	// 接受报文超时
		WEB_RET_NET_ERROR_ADDHEADER			=		111,	// 增加HTTP头失败
		WEB_RET_NET_ERROR_OPEN_SESSION		=		112,	// 打开HTTP会话失败

		// 通用错误类型
		WEB_RET_COMMON_NO_URL					=		200,	// 请求的URL不存在
		WEB_RET_COMMON_METHOD_NOT_SUPPORT	=		201,	// 客户端请求的方法不支持
		WEB_RET_COMMON_NO_PERMIT				=		202,	// 客户端请求的资源无权限访问
		WEB_RET_COMMON_NO_AUTH				=		203,	// 客户端没有通过授权
		WEB_RET_COMMON_SERVER_INTERNAL_ERROR=		204,	// 服务器内部错误
		WEB_RET_COMMON_SERVER_NOT_AVAIL		=		205,	// 服务器暂时不可用
		WEB_RET_COMMON_SERVER_NOT_IMPL		=		206,	// 服务器尚未实现请求方法
		WEB_RET_COMMON_PROXY_NEED_AUTH		=		207,	// 代理服务器需要验证

		WEB_RET_COMMON_NO_BODY_DATA			=		220,	// 没有接收到报文体数据
		WEB_RET_COMMON_NO_WEB_DATA			=		221,	
		WEB_RET_COMMON_NOT_LOGIN				=		222,	// 用户尚未登录
		WEB_RET_COMMON_ERROR_PARAM			=		223,	// 传入的参数错误
		WEB_RET_COMMON_ERROR_TOKEN			=		224,	// 传入的TOKEN错误
		WEB_RET_COMMON_ERROR_AUTH				=		225,	// 传入的auth错误
		WEB_RET_COMMON_TOKEN_TIMEOUT			=		226,	// Token超时
		WEB_RET_COMMON_ERROR_INTERNAL			=		230,	// XML内部解析错
		WEB_RET_COMMON_WRITE_FILE_ERROR			=		231,	// 写入文件失败
	};

	//============================================================================//
	//                   Web中所使用到的数据结构			                  //
	//============================================================================//
	// 返回的代理信息
	enum	ProxyType
	{
		PROXY_HTTP   =0,	// 使用HTTP代理
		PROXY_SOCK5,		// 使用SOCK5代理
		PROXY_IE,			// 使用IE设置
		PROXY_NONE,			// 不使用代理
	};

	// 每一个代理项的描述
	typedef struct _ProxyDetail
	{
		ProxyType	 eType;
		std::wstring strServer;
		std::wstring strPort;
		std::wstring strUserName;
		std::wstring strPassword;
		std::wstring strDomain;			// 域名

	} PROXYDETAIL, *PPROXYDETAIL;

	// 返回的代理描述信息
	typedef struct _ProxyData
	{
		ProxyType	eLastUsed;			// 最近一次使用的代理类型
		PROXYDETAIL	arrDetail[2];		// Sock3, HTTP，不包括IE代理的描述信息

	}PROXYDATA, *PPROXYDATA;

	//============================================================================//
	//                   Web中所使用到的event结构  			              //
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

	// 网络中返回的响应数据
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

	// 检查当前是否有更新信息
#define MAX_UPDATE_XML_LENGTH	10240
	struct Web_CheckUpdateConfigRespEvent	:	public WebRespEvent
	{
		Web_CheckUpdateConfigRespEvent()
		{
			eventValue	=	 EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP;
			ZeroMemory(szUpdateXml, MAX_UPDATE_XML_LENGTH * sizeof(wchar_t));
		}

		// 返回的xml报文，最大为10240长度
		wchar_t	szUpdateXml[MAX_UPDATE_XML_LENGTH];
	};

	struct Web_DownloadUpdateFileRespEvent	:	public WebRespEvent
	{
		Web_DownloadUpdateFileRespEvent()
		{
			eventValue = EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP;
		}

		int nId;	//	已经下载完毕的更新文件的ID
	};

	//===========================================//
	//                   Web中所使用到的Service						      //
	//===========================================//
	struct Web_Service	:	public	Service
	{
		Web_Service()
		{
			srcMId = MODULE_ID_INVALID;

			bAllowOffline	=	FALSE;
			bAllowRepeated	=	FALSE;
		}

		int	srcMId;	//	源模块

		bool	bAllowOffline;		//	是否允许离线操作, TRUE为允许，FALSE为不允许
		bool	bAllowRepeated;		//	该URL是否允许重复
	};


	// 向网络请求收藏夹ICON图标
	struct Web_GetFavIconqService	:	public Web_Service
	{
		Web_GetFavIconqService()
		{
			serviceId	=	 SERVICE_VALUE_WEB_GET_FAVICON;
			ZeroMemory(szFavoriteUrl, MAX_PATH * sizeof(wchar_t));
		}

		// 收藏夹图标的URL
		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
	};

	// 打开浏览器的时候通知服务器增加计数
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

	// 向网络传送当前用户的信息
	struct Web_ReportUserInfoService	:	public Web_Service
	{
		Web_ReportUserInfoService()
		{
			serviceId	=	 SERVICE_VALUE_WEB_REPORT_USERINFO;
			ZeroMemory(szMachinedId, 64 * sizeof(wchar_t));
			ZeroMemory(szOSVersion, 64 * sizeof(wchar_t));
		}

		// 收藏夹图标的URL
		wchar_t	szMachinedId[64];			//	机器ID
		wchar_t	szOSVersion[64];			//	操作系统版本
	};

	struct Web_CheckUpdateConfigService	:	public Web_Service
	{
		Web_CheckUpdateConfigService()
		{
			serviceId	=	 SERVICE_VALUE_WEB_CHECK_UPDATE_CONFIG;
		}
	};

	// 下载需要更新的文件
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
		//	需要下载的文件的URL
		wchar_t	szUpdateFileUrl[MAX_PATH];
		wchar_t	szSavePath[MAX_PATH];	//	下载存放的路径
	};

	struct Web_QueryDownFileProcessService	:	public Web_Service
	{
		Web_QueryDownFileProcessService()
		{
			serviceId	=	SERVICE_VALUE_WEB_QUERY_DOWNLOAD_FILE_PROESS;
		}

		uint32	nSeqNo;						//	当前下载任务的序列号

		uint32	uPercent;						//	下载进度， 50 表示 完成50%
		uint32	uTotalLength;					//	下载的总长度
		uint32	uFinishedLength;				//	已经下载完毕的总长度
		uint32	uSpeed;						//	速率，
		int32	uRemainedTime;				//	
	};

};