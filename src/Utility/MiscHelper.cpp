#include "stdafx.h"
#include "MiscHelper.h"
#include "XString.h"

MiscHelper::MiscHelper()
{

}

MiscHelper::~MiscHelper()
{

}

// �Ӹ�����URL�л�ȡ������ַ������http://www.baidu.com/index.html
// ����http://www.baidu.com
wchar_t*	MiscHelper::GetDomainFromUrl(const wchar_t*	pszUrl, BOOL bIncludePrefix)
{
	if( pszUrl == NULL)
		return L"";

	// ���ǰ���߸��ַ��Ƿ���http://
	String strUrl = pszUrl;
	if( strUrl.Left(7) != L"http://" && strUrl.Left(8) != L"https://")
		return L"";

	String strDomain = L"";
	if( strUrl.Left(7) == L"http://")
		strDomain = strUrl.SubStr(7, strUrl.GetLength() - 1);
	else if( strUrl.Left(8) == L"https://")
		strDomain = strUrl.SubStr(8, strUrl.GetLength() - 1);

	int nIndex = strDomain.FindFirstCharOf(L"\\/");
	if( nIndex == String::NPOS)
	{
		if( bIncludePrefix == TRUE)
		{
			if( strUrl.Left(7) == L"http://")
				strDomain = String(L"http://") + strDomain;
			else if( strDomain.Left(8) == L"https://")
				strDomain = String(L"https://") + strDomain;
		}

		return wcsdup((wchar_t*)strDomain.GetData());
	}

	strDomain = strDomain.SubStr(0, nIndex);

	if( bIncludePrefix == TRUE)
	{
		if( strUrl.Left(7) == L"http://")
			strDomain = String(L"http://") + strDomain;
		else if( strDomain.Left(8) == L"https://")
			strDomain = String(L"https://") + strDomain;
	}

	return wcsdup((wchar_t*)strDomain.GetData());
}


wchar_t*	MiscHelper::GetTopDomainUrl(const wchar_t* pszUrl)
{
	if( pszUrl == NULL)
		return L"";

	wchar_t* szUrl = GetDomainFromUrl(pszUrl);
	if( szUrl == NULL)
		return L"";

	String	strUrl = szUrl;
	strUrl = strUrl.ToLower();

	if( strUrl.Left(7) == L"http://")
		strUrl = strUrl.SubStr(7,strUrl.GetLength() - 7);
	else if( strUrl.Left(8) == L"https://")
		strUrl = strUrl.SubStr(8,strUrl.GetLength() - 8);

	// .com.cn|.net.cn|.org.cn|.gov.cn|.com|.net|.cn|.org|.cc|.me|.tel|.mobi|.asia|.biz|.info|.name|.tv|.hk|.��˾|.�й�|.����
	wchar_t szDomain[][MAX_PATH] = 
	{
		L".com.at",			//	�µ���
		L".com.au",		//	�Ĵ�����
		L".com.be",		//	����ʱ
		L".com.dk",		//	����
		L".com.es",			//	������
		L".com.us",
		L".com.de",
		L".com.uk",
		L".com.fr",
		L".com.eu",
		L".com.gb",
		L".com.ie",			//	������
		L".com.il",			//	��ɫ��
		L".com.in",			//	ӡ��
		L".com.it",			//	�����
		L".com.jp",			//	�ձ�
		L".com.kp",		//	����
		L".com.kr",			//	����
		L".com.se",			//	���
		L".com.vn",			//	Խ��
		L".com.cn",
		L".com.hk",
		L".com.mo",
		L".com.tw",

		L".net.at",			//	�µ���
		L".net.au",		//	�Ĵ�����
		L".net.be",		//	����ʱ
		L".net.dk",		//	����
		L".net.es",			//	������
		L".net.us",
		L".net.de",
		L".net.uk",
		L".net.fr",
		L".net.eu",
		L".net.gb",
		L".net.ie",			//	������
		L".net.il",			//	��ɫ��
		L".net.in",			//	ӡ��
		L".net.it",			//	�����
		L".net.jp",			//	�ձ�
		L".net.kp",		//	����
		L".net.kr",			//	����
		L".net.se",			//	���
		L".net.vn",			//	Խ��
		L".net.cn",
		L".net.hk",
		L".net.mo",
		L".net.tw",

		L".org.at",			//	�µ���
		L".org.au",		//	�Ĵ�����
		L".org.be",		//	����ʱ
		L".org.dk",		//	����
		L".org.es",			//	������
		L".org.us",
		L".org.de",
		L".org.uk",
		L".org.fr",
		L".org.eu",
		L".org.gb",
		L".org.ie",			//	������
		L".org.il",			//	��ɫ��
		L".org.in",			//	ӡ��
		L".org.it",			//	�����
		L".org.jp",			//	�ձ�
		L".org.kp",		//	����
		L".org.kr",			//	����
		L".org.se",			//	���
		L".org.vn",			//	Խ��
		L".org.cn",
		L".org.hk",
		L".org.mo",
		L".org.tw",

		L".gov.at",			//	�µ���
		L".gov.au",		//	�Ĵ�����
		L".gov.be",		//	����ʱ
		L".gov.dk",		//	����
		L".gov.es",			//	������
		L".gov.us",
		L".gov.de",
		L".gov.uk",
		L".gov.fr",
		L".gov.eu",
		L".gov.gb",
		L".gov.ie",			//	������
		L".gov.il",			//	��ɫ��
		L".gov.in",			//	ӡ��
		L".gov.it",			//	�����
		L".gov.jp",			//	�ձ�
		L".gov.kp",		//	����
		L".gov.kr",			//	����
		L".gov.se",			//	���
		L".gov.vn",			//	Խ��
		L".gov.cn",
		L".gov.hk",
		L".gov.mo",
		L".gov.tw",

		L".com",
		L".net",
		L".cn",
		L".org",
		L".cc",
		L".me",
		L".tel",
		L".mobile",
		L".asia",
		L".biz",
		L".info",
		L".name",
		L".tv",
		L".hv",
		L".��˾",
		L".�й�",
		L".����",
	};

	// �ҵ���һ��.
	for(int i=0; i<sizeof(szDomain)/2/MAX_PATH; i++)
	{
		if( strUrl.EndWith(szDomain[i]) == TRUE)
		{
			String strDomainName = strUrl.SubStr(0, strUrl.GetLength() - wcslen(szDomain[i]));
			int nIndex = strDomainName.FindLastCharOf(L'.', String::NPOS);
			strDomainName = strDomainName.SubStr(nIndex+1, strDomainName.GetLength() - nIndex);
			strDomainName += szDomain[i];

			return wcsdup(strDomainName.GetData());
		}
	}

	return L"";
}