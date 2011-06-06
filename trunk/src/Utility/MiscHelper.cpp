#include "stdafx.h"
#include "MiscHelper.h"
#include "XString.h"

MiscHelper::MiscHelper()
{

}

MiscHelper::~MiscHelper()
{

}

// 从给定的URL中获取域名地址，比如http://www.baidu.com/index.html
// 返回http://www.baidu.com
wchar_t*	MiscHelper::GetDomainFromUrl(const wchar_t*	pszUrl)
{
	if( pszUrl == NULL)
		return NULL;

	// 检查前面七个字符是否是http://
	String strUrl = pszUrl;
	if( strUrl.Left(7) != L"http://" && strUrl.Left(8) != L"https://")
		return NULL;

	String strDomain = L"";
	if( strUrl.Left(7) == L"http://")
		strDomain = strUrl.SubStr(7, strUrl.GetLength() - 1);
	else if( strUrl.Left(8) == L"https://")
		strDomain = strUrl.SubStr(8, strUrl.GetLength() - 1);

	int nIndex = strDomain.FindFirstCharOf(L"\\/");
	if( nIndex == String::NPOS)
		return wcsdup((wchar_t*)strUrl.GetData());

	if( strUrl.Left(7) == L"http://")
		nIndex += 7;
	else if( strUrl.Left(8) == L"https://")
		nIndex += 8;

	strDomain = strUrl.SubStr(0, nIndex);
	return wcsdup((wchar_t*)strDomain.GetData());
}


wchar_t*	MiscHelper::GetTopDomainUrl(const wchar_t* pszUrl)
{
	if( pszUrl == NULL)
		return NULL;

	String	strUrl = pszUrl;
	strUrl = strUrl.ToLower();

	if( strUrl.Left(7) == L"http://")
		strUrl = strUrl.SubStr(7,strUrl.GetLength() - 7);
	else if( strUrl.Left(8) == L"https://")
		strUrl = strUrl.SubStr(8,strUrl.GetLength() - 8);

	// .com.cn|.net.cn|.org.cn|.gov.cn|.com|.net|.cn|.org|.cc|.me|.tel|.mobi|.asia|.biz|.info|.name|.tv|.hk|.公司|.中国|.网络
	wchar_t szDomain[][MAX_PATH] = 
	{
		L".com.cn",
		L".net.cn",
		L".org.cn",
		L".gov.cn",
		L".com",
		L".net",
		L".cn",
		L".org",
		L".cc",
		L"me",
		L".tel",
		L".mobile",
		L".asia",
		L".biz",
		L".info",
		L".name",
		L".tv",
		L".hv",
		L".公司",
		L".中国",
		L".网络",
	};

	// 找到第一个.




	return NULL;
}