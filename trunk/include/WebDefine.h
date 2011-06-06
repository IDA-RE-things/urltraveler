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
		// Favicon对应的图标
		EVENT_VALUE_WEB_GET_FAVICON = EVENT_VALUE_WEB_BEGIN,		// 通知去拉取Favorite图标
	};

	// Web能够可能对外发送的广播消息
	enum E_WebMessageValue
	{
	};

	// Web能够处理的直接调用
	enum E_WebServiceValue
	{
		SERVICE_VALUE_WEB_FAVICON_LOAD = SERVICE_VALUE_WEB_BEGIN,					//	加载特定的URL的收藏夹图标
	};

	// 返回给上层调用者的错误响应信息
	enum E_WebRespCode
	{
		WEB_RET_SUCCESS						=		5000,			// 成功获取到用户的信息
		WEB_RET_INNER_USED					=		5002,
		WEB_RET_DOWNLOAD_ERROR				=		5003,			// 下载失败，重新下载

		// 网络相关的信息
		WEB_RET_NET_ERROR_URL				=		900,	// 请求的URL错误
		WEB_RET_NET_ERROR_CONNECT			=		901,	// 无法连接远程服务器
		WEB_RET_NET_ERROR_OPEN_REQUEST		=		902,	// 无法打开请求
		WEB_RET_NET_ERROR_SETCALLBACK		=		903,	// 设置Callback函数失败
		WEB_RET_NET_ERROR_SENDREQUEST		=		904,	// 发送请求失败
		WEB_RET_NET_ERROR_SET_OPTION		=		905,	// 设置选项错误
		WEB_RET_NET_ERROR_QUERY_HEADER		=		906,	// 查询HTTP头失败
		WEB_RET_NET_ERROR_QUERY_BODY_DATA	=		907,	// 查询报文体数据失败
		WEB_RET_NET_ERROR_READ_DATA			=		908,	// 读取报文体数据失败
		WEB_RET_NET_ERROR_RECEIVE_BODY_DATA	=		909,	// 接受报文体数据失败
		WEB_RET_NET_ERROR_TIMEOUT			=		910,	// 接受报文超时
		WEB_RET_NET_ERROR_ADDHEADER			=		911,	// 增加HTTP头失败
		WEB_RET_NET_ERROR_OPEN_SESSION		=		912,	// 打开HTTP会话失败

		// 通用错误类型
		WEB_RET_COMMON_NO_URL				=		1000,	// 请求的URL不存在
		WEB_RET_COMMON_METHOD_NOT_SUPPORT	=		1001,	// 客户端请求的方法不支持
		WEB_RET_COMMON_NO_PERMIT			=		1002,	// 客户端请求的资源无权限访问
		WEB_RET_COMMON_NO_AUTH				=		1003,	// 客户端没有通过授权
		WEB_RET_COMMON_SERVER_INTERNAL_ERROR=		1004,	// 服务器内部错误
		WEB_RET_COMMON_SERVER_NOT_AVAIL		=		1005,	// 服务器暂时不可用
		WEB_RET_COMMON_SERVER_NOT_IMPL		=		1006,	// 服务器尚未实现请求方法
		WEB_RET_COMMON_PROXY_NEED_AUTH		=		1007,	// 代理服务器需要验证

		WEB_RET_COMMON_NO_BODY_DATA			=		1020,	// 没有接收到报文体数据
		WEB_RET_COMMON_NO_WEB_DATA			=		1021,	
		WEB_RET_COMMON_NOT_LOGIN			=		1022,	// 用户尚未登录
		WEB_RET_COMMON_ERROR_PARAM			=		1023,	// 传入的参数错误
		WEB_RET_COMMON_ERROR_TOKEN			=		1024,	// 传入的TOKEN错误
		WEB_RET_COMMON_ERROR_AUTH			=		1025,	// 传入的auth错误
		WEB_RET_COMMON_TOKEN_TIMEOUT		=		1026,	// Token超时
		WEB_RET_COMMON_ERROR_INTERNAL		=		1030,	// XML内部解析错
		WEB_RET_COMMON_WRITE_FILE_ERROR		=		1031,	// 写入文件失败
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
	struct WebEvent	:	public ExtraInfo
	{
		WebEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_WEB;

			m_pstExtraInfo = this;
		}
	};

	// 向网络请求收藏夹ICON图标
	struct Web_GetFavIconEvent	:	public WebEvent
	{
		Web_GetFavIconEvent()
		{
			eventValue	=	 EVENT_VALUE_WEB_GET_FAVICON;
			ZeroMemory(szFavoriteUrl, MAX_PATH);
		}

		// 收藏夹图标的URL
		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
	};
};