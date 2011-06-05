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
wchar_t*	MiscHelper::GetDomainFromUrl(const wchar_t*	pszUrl)
{
	if( pszUrl == NULL)
		return NULL;

	// ���ǰ���߸��ַ��Ƿ���http://
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
