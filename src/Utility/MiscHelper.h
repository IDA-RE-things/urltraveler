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
	// ����http://www.baidu.com
	static wchar_t*	GetDomainFromUrl(const wchar_t*	pszUrl);
};