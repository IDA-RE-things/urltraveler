#pragma once

/************************************************************************/
/* @function ��libCurl�İ�װ������ģʽ                                  */
/* @author	zhangzhongqing												*/
/************************************************************************/
#include <string>
#include <map>
#include "curl/curl.h"
#include "Singleton.h"
#include "stdafx.h"

using namespace std;

//��������
enum EIdwProxyType
{
	PT_HTTP = 0,
	PT_SOCK4,
	PT_SOCK5,
};

typedef struct ProxyInfo
{
	EIdwProxyType eProxyType;//�������������
	wchar_t       szAddress[16];//�����������ַ
	short         uPort;//����������˿�
	bool          bAuth;//�Ƿ���Ҫ��֤�û���������
	wchar_t       szUsername[32];//�û���
	wchar_t       szPassword[32];//����
}PROXYINFO;

class UTILITY_API CurlHttp : public CSingleton<CurlHttp>
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
	BOOL SetProxy(PROXYINFO stProxyInfo);
	BOOL DisableProxy();

	/* �洢��ͬ�������Ӧ����*/
	std::map<int, string>	m_mapResponse;

private:

	CURL*	m_pCurl;

	std::map<wstring, wstring>	m_mapParams;	//	Post����

	/* ��ʶ��ͬ��Http����*/
	int	m_nCookie;		
};

