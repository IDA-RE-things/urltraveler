#pragma once

/************************************************************************/
/* @function 对libCurl的包装，单例模式                                  */
/* @author	zhangzhongqing												*/
/************************************************************************/
#include <string>
#include <map>
#include "curl/curl.h"
#include "Singleton.h"
#include "stdafx.h"

using namespace std;

//代理类型
enum EIdwProxyType
{
	PT_HTTP = 0,
	PT_SOCK4,
	PT_SOCK5,
};

typedef struct ProxyInfo
{
	EIdwProxyType eProxyType;//代理服务器类型
	wchar_t       szAddress[16];//代理服务器地址
	short         uPort;//代理服务器端口
	bool          bAuth;//是否需要认证用户名和密码
	wchar_t       szUsername[32];//用户名
	wchar_t       szPassword[32];//密码
}PROXYINFO;

class UTILITY_API CurlHttp : public CSingleton<CurlHttp>
{
public:
	
	CurlHttp();
	virtual ~CurlHttp();

	/**
	* @function 增加http header参数
	* @param	strKey	参数key
	* @param	strValue	参数value
	*/
	void AddHeader(string Key, string strValue);

	/**
	* @function 增加http 请求参数
	* @param	strKey	参数key
	* @param	strValue	参数value
	*/
	void AddParam(wstring strKey, wstring strValue);

	/**
	* @function 增加http 请求参数
	* @param	strKey	参数key
	* @param	lValue	参数value
	*/
	void AddParam(wstring strKey, long lValue);

	/**
	* @function 删除某个参数
	* @param	strKey	参数key
	*/
	void ClearParam(wstring strKey);

	/**
	* @function 删除所有参数
	*/
	void ClearAllParams();

	/**
	* @function 发送http请求，请求为Get
	* @param	strUrl	http请求访问的URL
	* @return	返回响应报文
	*/
	string RequestGet(wstring strUrl);

	/**
	* @function 发送http请求，请求为Get
	* @param	strUrl	http请求访问的URL
	* @return	返回响应报文
	*/
	string RequestPost(wstring strUrl);

	/**
	* @function 查找给定请求的响应
	* @param	nCookie	给定的请求
	*/
	string FindResponse(int nCookie);

	/**
	* @function 查找给定请求的响应
	* @param	nCookie	给定的请求
	*/
	BOOL SetProxy(PROXYINFO stProxyInfo);
	BOOL DisableProxy();

	/* 存储不同请求的响应报文*/
	std::map<int, string>	m_mapResponse;

private:

	CURL*	m_pCurl;

	std::map<wstring, wstring>	m_mapParams;	//	Post参数

	/* 标识不同的Http请求*/
	int	m_nCookie;		
};

