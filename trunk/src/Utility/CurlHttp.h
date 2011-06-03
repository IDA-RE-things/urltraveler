#pragma once

/************************************************************************/
/* @function 对libCurl的包装，单例模式                                  */
/* @author	zhangzhongqing												*/
/************************************************************************/
#include <string>
#include <map>
#include "curl/curl.h"
#include "Sigleton.h"
#include "IdwDefine.h"

using namespace std;

class CurlHttp : public CSingleton<CurlHttp>
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
	BOOL SetProxy(idw::PROXYINFO stProxyInfo);
	BOOL DisableProxy();

	/* 存储不同请求的响应报文*/
	std::map<int, string>	m_mapResponse;

private:

	CURL*	m_pCurl;

	std::map<wstring, wstring>	m_mapParams;	//	Post参数

	/* 标识不同的Http请求*/
	int	m_nCookie;		
};

