#pragma once

#include "StdAfx.h"
#include "SndaBase.h"
#include <string>
#include <vector>

using namespace  std;

class MiscHelper
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

	//��ȡ����ϵͳ�İ汾

#define OS_UNKNOWN     0
#define OS_WIN95       1
#define OS_WIN98       2
#define OS_WIN98SE     3
#define OS_WINME       4
#define OS_WINNT       5
#define OS_WIN2000     6
#define OS_WINXP       7
#define OS_WIN2003     8
#define OS_WINXP64     9
#define OS_VISTA       10
#define OS_WIN7        11
	static int GetOsVer();

	// ���ظ����ļ������·��
	static wchar_t*	GetUpdatePath();

	static wchar_t*	GetUnpackagePath();

	static void	 DeleteUnpackagePath();

	static int GetCurrentVersion();

	// ��1.0.0.0��ʽ���ַ����������
	static int	GetVersionFromString(const char*	pszVersion);

	// ������������ת��Ϊ3.0.0.0��ʽ���ַ���
	static wchar_t* GetStringFromVersion(int nVersion);

	static BOOL ExistProcess(LPCTSTR pszClassName, LPCTSTR pszWindowTitle);

	static BOOL KillProcess(LPCTSTR pszClassName, LPCTSTR pszWindowTitle);
};