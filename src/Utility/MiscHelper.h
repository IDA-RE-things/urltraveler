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
};