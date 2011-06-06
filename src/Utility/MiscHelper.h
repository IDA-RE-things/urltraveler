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

	// 从给定的URL中获取域名地址，比如http://www.baidu.com/index.html
	// 返回http://www.baidu.com
	static wchar_t*	GetDomainFromUrl(const wchar_t*	pszUrl);

	// 获取给定域名的顶级域名。比如www.baidu.com，返回baidu.com
	// weiba.baidu.com，则返回baidu.com
	// 传入的pszDomainUrl必须为 "www.baidu.com"格式，不能包含http
	//	尾部也不能包含"/"或者"\"
	static wchar_t*	GetTopDomainUrl(const wchar_t* pszDomainUrl);
};