#pragma once

#include "StdAfx.h"
#include "SndaBase.h"
#include <string>
#include <vector>

using namespace  std;

class UTILITY_API MiscHelper
{
private:
	MiscHelper(void);
	~MiscHelper(void);

public:

	// �Ӹ�����URL�л�ȡ������ַ������http://www.baidu.com/index.html
	// ����http://www.baidu.com����www.baidu.com
	//	bIncludePrefix ���ؽ�����Ƿ�Ҫ����http://����https://��Ĭ���Ƿ���
	static wchar_t*	GetDomainFromUrl(const wchar_t*	pszUrl, BOOL bIncludePrefix = TRUE);

	// ��ȡ���������Ķ�������������www.baidu.com������baidu.com
	// weiba.baidu.com���򷵻�baidu.com
	// �����pszDomainUrl����Ϊ "www.baidu.com"��ʽ�����ܰ���http
	//	β��Ҳ���ܰ���"/"����"\"
	static wchar_t*	GetTopDomainUrl(const wchar_t* pszDomainUrl);

	static BOOL	SaveTextToClipboard(const char*	lpszText);
};