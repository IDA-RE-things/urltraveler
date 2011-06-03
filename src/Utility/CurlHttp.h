#pragma once

/************************************************************************/
/* @function ��libCurl�İ�װ������ģʽ                                  */
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
	* @function ����http header����
	* @param	strKey	����key
	* @param	strValue	����value
	*/
	void AddHeader(string Key, string strValue);

	/**
	* @function ����http �������
	* @param	strKey	����key
	* @param	strValue	����value
	*/
	void AddParam(wstring strKey, wstring strValue);

	/**
	* @function ����http �������
	* @param	strKey	����key
	* @param	lValue	����value
	*/
	void AddParam(wstring strKey, long lValue);

	/**
	* @function ɾ��ĳ������
	* @param	strKey	����key
	*/
	void ClearParam(wstring strKey);

	/**
	* @function ɾ�����в���
	*/
	void ClearAllParams();

	/**
	* @function ����http��������ΪGet
	* @param	strUrl	http������ʵ�URL
	* @return	������Ӧ����
	*/
	string RequestGet(wstring strUrl);

	/**
	* @function ����http��������ΪGet
	* @param	strUrl	http������ʵ�URL
	* @return	������Ӧ����
	*/
	string RequestPost(wstring strUrl);

	/**
	* @function ���Ҹ����������Ӧ
	* @param	nCookie	����������
	*/
	string FindResponse(int nCookie);

	/**
	* @function ���Ҹ����������Ӧ
	* @param	nCookie	����������
	*/
	BOOL SetProxy(idw::PROXYINFO stProxyInfo);
	BOOL DisableProxy();

	/* �洢��ͬ�������Ӧ����*/
	std::map<int, string>	m_mapResponse;

private:

	CURL*	m_pCurl;

	std::map<wstring, wstring>	m_mapParams;	//	Post����

	/* ��ʶ��ͬ��Http����*/
	int	m_nCookie;		
};

