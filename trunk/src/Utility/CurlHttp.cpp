#include "CurlHttp.h"
#include "assert.h"
#include "StringHelper.h"


/**  
 * @brief libcurl接收到数据时的回调函数  
 *  
 * 将接收到的数据保存到本地文件中，同时显示在控制台上。  
 *  
 * @param [in] buffer 接收到的数据所在缓冲区  
 * @param [in] size 数据长度  
 * @param [in] nmemb 数据片数量  
 * @param [in/out] 用户自定义指针  
 * @return 获取的数据长度  
 */ 
size_t ReadData(void *pBuffer, size_t size, size_t nmemb, void *pUuserp)
{  
	if( pUuserp == NULL)
		return 0;

	CurlHttp* pClient = CurlHttp::GetInstance();

	int nCookie = *(int*)(pUuserp);
	
	string strResponse = pClient->FindResponse(nCookie);
	strResponse.append((char*)pBuffer, nmemb);
	pClient->m_mapResponse[nCookie] = strResponse;
	return 0;
}

CurlHttp::CurlHttp()
{
	curl_global_init(CURL_GLOBAL_ALL);
	
	// Init libCurl
	m_pCurl = curl_easy_init();

	int timeout = 5;
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, timeout);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, ReadData); 

	m_nCookie = 0;
}

CurlHttp::~CurlHttp()
{
	/* always cleanup */      
	curl_easy_cleanup(m_pCurl);  
	curl_global_cleanup();

	m_mapParams.clear();
}

/**
* @function 查找给定请求的响应
* @param	nCookie	给定的请求
*/
string CurlHttp::FindResponse(int nCookie)
{
	std::map<int, string>::iterator itr = m_mapResponse.find(nCookie);
	if( itr == m_mapResponse.end())
		return "";

	return itr->second;
}

void CurlHttp::AddParam(wstring strKey, wstring strValue)
{
	m_mapParams[strKey] = strValue;
}

/**
* @function 增加http 请求参数
* @param	strKey	参数key
* @param	lValue	参数value
*/
void CurlHttp::AddParam(wstring strKey, long lValue)
{
	wstring strValue = StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(lValue));
	m_mapParams[strKey] = strValue;
}

/**
* @function 删除某个参数
* @param	strKey	参数key
*/
void CurlHttp::ClearParam(wstring strKey)
{
	m_mapParams.erase(strKey);
}

/**
* @function 删除所有参数
*/
void CurlHttp::ClearAllParams()
{
	m_mapParams.clear();
}

/**
* @function 发送http请求，请求为Get
* @param	strUrl	http请求访问的URL
* @return	返回响应报文
*/
string CurlHttp::RequestGet(wstring strUrl)
{
	struct curl_httppost *pHttpPostArgument = NULL;
	
	wstring strResultUrl = L"";

	for(std::map<wstring,wstring>::iterator itr = m_mapParams.begin(); 
	itr != m_mapParams.end(); itr++){
		strResultUrl += itr->first + wstring(L"=") + itr->second + wstring(L"&");		
	}

	if( strResultUrl != L"")
	{
		strResultUrl = strUrl + wstring(L"?") + strResultUrl;
	}
	else
	{
		strResultUrl = strUrl;
	}
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, NULL);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, StringHelper::UnicodeToANSI(strResultUrl.c_str()).c_str()); 
	
	int nCookie = m_nCookie;
	m_mapResponse[m_nCookie] = "";
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &nCookie);  
	m_nCookie ++;
	
	/* send the request */
	CURLcode httpres;
	httpres = curl_easy_perform(m_pCurl); 
	
	/* then cleanup the formpost chain */      
	curl_formfree(pHttpPostArgument); 
	
	/* get all the response */	
	string strResponse = FindResponse(nCookie);
	return strResponse;

}

/**
* @function 发送http请求，请求为Get
* @param	strUrl	http请求访问的URL
* @return	返回响应报文
*/
string CurlHttp::RequestPost(wstring strUrl)
{
	struct curl_httppost *pHttpPostArgument = NULL;
	struct curl_httppost *pLastHttpPostArgumentPtr = NULL;

	for(std::map<wstring,wstring>::iterator itr = m_mapParams.begin(); 
	itr != m_mapParams.end(); itr++){

		// add app id params
		curl_formadd(&pHttpPostArgument,              
			&pLastHttpPostArgumentPtr,             
			CURLFORM_COPYNAME, itr->first,
			CURLFORM_COPYCONTENTS, itr->second.c_str(),             
			CURLFORM_END);
	}

	curl_easy_setopt(m_pCurl, CURLOPT_HTTPPOST, pHttpPostArgument);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, strUrl.c_str()); 

	int nCookie = m_nCookie;
	m_mapResponse[m_nCookie] = "";
    curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &nCookie);  
	m_nCookie ++;

	/* send the request */
	CURLcode httpres;
	httpres = curl_easy_perform(m_pCurl); 
	
	/* then cleanup the formpost chain */      
	curl_formfree(pHttpPostArgument); 

	/* get all the response */	
	string strResponse = FindResponse(nCookie);
	return strResponse;
}

BOOL CurlHttp::SetProxy( idw::PROXYINFO stProxyInfo )
{
	/*
	 *   - http://[username:password@]proxy:port
	 *   - socks://[username:password@]proxy:port
	 *   - socks5://[username:password@]proxy:port
	 *   - socks4://[username:password@]proxy:port
	 *   - <procotol>://[username:password@]proxy:port
	*/
	char szProxy[MAX_PATH] = {0};
	char *pszProxyType;

	switch(stProxyInfo.eProxyType)
	{
	case idw::PT_HTTP:
		pszProxyType = "http";
		curl_easy_setopt(m_pCurl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP); 
		break;
	case idw::PT_SOCK4:
		pszProxyType = "socks4";
		curl_easy_setopt(m_pCurl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4); 
		break;
	case idw::PT_SOCK5:
		pszProxyType = "socks5";
		curl_easy_setopt(m_pCurl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5); 
		break;
	default:
		pszProxyType = NULL;
		break;
	}

	if (pszProxyType != NULL)
	{
		if (stProxyInfo.bAuth == TRUE)
		{
			sprintf_s(szProxy, "%s://%s:%s@%s:%d", pszProxyType, StringHelper::UnicodeToANSI(stProxyInfo.szUsername).c_str(),
				StringHelper::UnicodeToANSI(stProxyInfo.szPassword).c_str(),
				StringHelper::UnicodeToANSI(stProxyInfo.szAddress).c_str(),
				stProxyInfo.uPort);
		}
		else
		{
			sprintf_s(szProxy, "%s://%s:%d", pszProxyType,
				StringHelper::UnicodeToANSI(stProxyInfo.szAddress).c_str(),
				stProxyInfo.uPort);
		}

		int nRet = curl_easy_setopt(m_pCurl, CURLOPT_PROXY, szProxy); 

		return nRet == CURLE_OK;
	}

	return FALSE;
}

BOOL CurlHttp::DisableProxy()
{
	int nRet = curl_easy_setopt(m_pCurl, CURLOPT_PROXY, ""); 

	return nRet == CURLE_OK;
}
