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

	// 从给定的URL中获取域名地址，比如http://www.baidu.com/index.html
	// 返回http://www.baidu.com或者www.baidu.com
	//	bIncludePrefix 返回结果中是否要包含http://或者https://。默认是返回
	static wchar_t*	GetDomainFromUrl(const wchar_t*	pszUrl, BOOL bIncludePrefix = TRUE);

	// 获取给定域名的顶级域名。比如www.baidu.com，返回baidu.com
	// weiba.baidu.com，则返回baidu.com
	// 传入的pszDomainUrl必须为 "www.baidu.com"格式，不能包含http
	//	尾部也不能包含"/"或者"\"
	static wchar_t*	GetTopDomainUrl(const wchar_t* pszDomainUrl);

	static BOOL	SaveTextToClipboard(const char*	lpszText);

	//获取操作系统的版本

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

	// 返回更新文件保存的路径
	static wchar_t*	GetUpdatePath();

	static wchar_t*	GetUnpackagePath();

	static void	 DeleteUnpackagePath();

	static int GetCurrentVersion();

	// 将1.0.0.0格式的字符传变成整数
	static int	GetVersionFromString(const char*	pszVersion);

	// 将给定的整数转换为3.0.0.0格式的字符串
	static wchar_t* GetStringFromVersion(int nVersion);

	static BOOL ExistProcess(LPCTSTR pszClassName, LPCTSTR pszWindowTitle);

	static BOOL KillProcess(LPCTSTR pszClassName, LPCTSTR pszWindowTitle);
};