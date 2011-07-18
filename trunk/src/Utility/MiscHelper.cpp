#include "stdafx.h"
#include "PathHelper.h"
#include "MiscHelper.h"
#include "XString.h"
#include <atlbase.h>
#include "StringHelper.h"
#include "UrlTravelerHeader.h"
#include "XString.h"
#include "TxConfig.h"
#include "FileHelper.h"


MiscHelper::MiscHelper()
{

}

MiscHelper::~MiscHelper()
{

}

// 从给定的URL中获取域名地址，比如http://www.baidu.com/index.html
// 返回http://www.baidu.com
wchar_t*	MiscHelper::GetDomainFromUrl(const wchar_t*	pszUrl, BOOL bIncludePrefix)
{
	if( pszUrl == NULL)
		return L"";

	// 检查前面七个字符是否是http://
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

	// .com.cn|.net.cn|.org.cn|.gov.cn|.com|.net|.cn|.org|.cc|.me|.tel|.mobi|.asia|.biz|.info|.name|.tv|.hk|.公司|.中国|.网络
	wchar_t szDomain[][MAX_PATH] = 
	{
		L".com.at",			//	奥地利
		L".com.au",		//	澳大利亚
		L".com.be",		//	比利时
		L".com.dk",		//	丹麦
		L".com.es",			//	西班牙
		L".com.us",
		L".com.de",
		L".com.uk",
		L".com.fr",
		L".com.eu",
		L".com.gb",
		L".com.ie",			//	爱尔兰
		L".com.il",			//	以色列
		L".com.in",			//	印度
		L".com.it",			//	意大利
		L".com.jp",			//	日本
		L".com.kp",		//	朝鲜
		L".com.kr",			//	韩国
		L".com.se",			//	瑞典
		L".com.vn",			//	越南
		L".com.cn",
		L".com.hk",
		L".com.mo",
		L".com.tw",

		L".net.at",			//	奥地利
		L".net.au",		//	澳大利亚
		L".net.be",		//	比利时
		L".net.dk",		//	丹麦
		L".net.es",			//	西班牙
		L".net.us",
		L".net.de",
		L".net.uk",
		L".net.fr",
		L".net.eu",
		L".net.gb",
		L".net.ie",			//	爱尔兰
		L".net.il",			//	以色列
		L".net.in",			//	印度
		L".net.it",			//	意大利
		L".net.jp",			//	日本
		L".net.kp",		//	朝鲜
		L".net.kr",			//	韩国
		L".net.se",			//	瑞典
		L".net.vn",			//	越南
		L".net.cn",
		L".net.hk",
		L".net.mo",
		L".net.tw",

		L".org.at",			//	奥地利
		L".org.au",		//	澳大利亚
		L".org.be",		//	比利时
		L".org.dk",		//	丹麦
		L".org.es",			//	西班牙
		L".org.us",
		L".org.de",
		L".org.uk",
		L".org.fr",
		L".org.eu",
		L".org.gb",
		L".org.ie",			//	爱尔兰
		L".org.il",			//	以色列
		L".org.in",			//	印度
		L".org.it",			//	意大利
		L".org.jp",			//	日本
		L".org.kp",		//	朝鲜
		L".org.kr",			//	韩国
		L".org.se",			//	瑞典
		L".org.vn",			//	越南
		L".org.cn",
		L".org.hk",
		L".org.mo",
		L".org.tw",

		L".gov.at",			//	奥地利
		L".gov.au",		//	澳大利亚
		L".gov.be",		//	比利时
		L".gov.dk",		//	丹麦
		L".gov.es",			//	西班牙
		L".gov.us",
		L".gov.de",
		L".gov.uk",
		L".gov.fr",
		L".gov.eu",
		L".gov.gb",
		L".gov.ie",			//	爱尔兰
		L".gov.il",			//	以色列
		L".gov.in",			//	印度
		L".gov.it",			//	意大利
		L".gov.jp",			//	日本
		L".gov.kp",		//	朝鲜
		L".gov.kr",			//	韩国
		L".gov.se",			//	瑞典
		L".gov.vn",			//	越南
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
		L".公司",
		L".中国",
		L".网络",
	};

	// 找到第一个.
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

BOOL	MiscHelper::SaveTextToClipboard(const char*	lpszText)
{
	if( lpszText == NULL)
		return FALSE;

	int nlen = strlen(lpszText);
	if (nlen == 0)
		return FALSE;

	// 将szText中的内容复制到剪贴板
	HGLOBAL hglbCopy;
	LPTSTR  lptstrCopy;
	if (!::OpenClipboard(NULL))
		return FALSE;

	hglbCopy = GlobalAlloc(GMEM_DDESHARE, 
		(nlen + 1) * sizeof(char)); 
	if (hglbCopy == NULL) 
	{ 
		CloseClipboard(); 
		return FALSE; 
	} 

	EmptyClipboard();
	lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, lpszText, nlen);
	lptstrCopy[nlen] = (TCHAR) 0;    // null character 
	GlobalUnlock(lptstrCopy); 

	SetClipboardData(CF_TEXT, hglbCopy);
	CloseClipboard();

	return TRUE;
}

int MiscHelper::GetOsVer()
{
	typedef struct _OSVERSIONINFOEXP
	{
		DWORD dwOSVersionInfoSize;
		DWORD dwMajorVersion;
		DWORD dwMinorVersion;
		DWORD dwBuildNumber;
		DWORD dwPlatformId;
		TCHAR szCSDVersion[ 128 ];
		WORD wServicePackMajor;
		WORD wServicePackMinor;
		WORD wSuiteMask;
		BYTE wProductType;
		BYTE wReserved;
	} OSVERSIONINFOEXP;

	OSVERSIONINFOEXP osVerInfo = {0};

	osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXP);

	if(!GetVersionEx((OSVERSIONINFO *) &osVerInfo))
	{
		osVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if(!GetVersionEx( (OSVERSIONINFO *) &osVerInfo)) 
		{
			return OS_UNKNOWN;
		}
	}

	typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

	SYSTEM_INFO si = {0};
	PGNSI pGNSI;

	pGNSI = (PGNSI)GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");

	if(NULL != pGNSI)
	{
		pGNSI(&si);
	}
	else
	{
		GetSystemInfo(&si);
	}

	int nSystemVersion = OS_UNKNOWN;

	DWORD dwPlatId = osVerInfo.dwPlatformId;
	DWORD majorVer = osVerInfo.dwMajorVersion;
	DWORD minorVer = osVerInfo.dwMinorVersion;

	if(dwPlatId == VER_PLATFORM_WIN32_NT) 
	{
		//Windows NT/2000
		if( majorVer <= 4 )
		{
			nSystemVersion = OS_WINNT;
		}
		else if( majorVer == 5 && minorVer == 0)
		{
			nSystemVersion = OS_WIN2000;
		}
		else if( majorVer == 5 && minorVer == 1)
		{
			nSystemVersion = OS_WINXP;
		}
		else if( majorVer == 5 && minorVer == 2)
		{
			if(osVerInfo.wProductType == 1 &&//VER_NT_WORKSTATION
				si.wProcessorArchitecture == 9)//PROCESSOR_ARCHITECTURE_AMD64
			{
				nSystemVersion = OS_WINXP64;
			}
			else
			{
				nSystemVersion = OS_WIN2003;
			}
		}
		else if( majorVer == 6 && minorVer == 0)
		{
			nSystemVersion = OS_VISTA;
		}
		else if(majorVer == 6 && minorVer == 1)
		{
			nSystemVersion = OS_WIN7;
		}
		else
		{
			nSystemVersion = OS_UNKNOWN;
		}
	}
	else if(dwPlatId == VER_PLATFORM_WIN32_WINDOWS)
	{
		//Windows 9x/ME
		if( majorVer == 4 && minorVer ==0)
		{
			nSystemVersion = OS_WIN95;
		}
		else if( majorVer == 4 && minorVer == 10)
		{
			if( osVerInfo.szCSDVersion[1] == 'A' || osVerInfo.szCSDVersion[1] == 'B')
			{
				nSystemVersion = OS_WIN98SE;
			}
			else
			{
				nSystemVersion = OS_WIN98;
			}
		}
		else if( majorVer == 4 && minorVer == 90 )
		{
			nSystemVersion = OS_WINME;
		}
		else
		{
			nSystemVersion = OS_UNKNOWN;
		}
	}
	else
	{
		nSystemVersion = OS_UNKNOWN;
	}

	return nSystemVersion;
}

// 返回更新文件保存的路径
wchar_t*	MiscHelper::GetUpdatePath()
{
	wstring wstrUpdate = PathHelper::GetAppDataDir() + L"\\urltraveler\\update\\";
	PathHelper::CreateMultipleDirectory(wstrUpdate);

	return (wchar_t*)wcsdup(wstrUpdate.c_str());
}

wchar_t*	MiscHelper::GetUnpackagePath()
{
	wstring wstrUpdate = PathHelper::GetTempDir() + L"\\urltraveler\\unpackage\\";
	PathHelper::CreateMultipleDirectory(wstrUpdate);
	return (wchar_t*)wcsdup(wstrUpdate.c_str());
}

void	 MiscHelper::DeleteUnpackagePath()
{
	/*C:\Documents and Settings\zhangzhongqing.SNDA\Local Settings\Temp\urltraveler\unpackage*/
	wstring wstrUpdate = PathHelper::GetTempDir() + L"urltraveler\\unpackage";
	PathHelper::RemoveDir(wstrUpdate, TRUE);
}

long MiscHelper::GetCurrentVersion()
{
	wstring wstrConfig = MiscHelper::GetConfig();
	CTxConfig txConfig;
	BOOL bRet = txConfig.ParseConfig(StringHelper::UnicodeToANSI(wstrConfig));
	if( bRet == FALSE)
	{
		return CLIENT_VERSION;
	}

	string strVersion = txConfig.GetValue(L"version");
	if( strVersion == "")
	{
		return CLIENT_VERSION;
	}

	return StringHelper::ConvertToInt(strVersion);
}

// 将1.0.0.0格式的字符传变成整数
long	MiscHelper::GetVersionFromString(const char*	pszVersion)
{
	if( pszVersion == NULL)
		return 0;

	std::vector<std::string> vecResutl = StringHelper::Split(pszVersion, '.');
	if( vecResutl.size() == 0)
		vecResutl.push_back(pszVersion);

	int nNum = vecResutl.size() > 4 ? 4 : vecResutl.size();

	int i =0;
	for( ; i<nNum; i++)
	{
		string&	szSubVersion = vecResutl[i];
		if( StringHelper::IsNumberic(szSubVersion) == false)
			break;
	}

	int nVersion = 0;
	for( int j=0; j<nNum; j++)
	{
		nVersion += StringHelper::ConvertToInt(vecResutl[j]) << ((3-j)*8);
	}

	return nVersion;
}

// 将给定的整数转换为3.0.0.0格式的字符串
wchar_t* MiscHelper::GetStringFromVersion(int nVersion)
{
	if( nVersion <= 0)
		return NULL;

	String wstrVerson = L"";
	for( int i=0; i<4; i++)
	{
		int n = nVersion/(1 << ((3-i)*8));
		wstrVerson += String::ValueOf(n);
		wstrVerson += L".";

		nVersion = nVersion%(1 << ((3-i)*8));
	}

	wstrVerson.Trim(L'.');
	return wcsdup(wstrVerson.GetData());
}

BOOL MiscHelper::ExistProcess(LPCTSTR pszClassName, LPCTSTR pszWindowTitle)
{
	HANDLE hProcessHandle;  
	ULONG nProcessID;
	HWND TheWindow;

	TheWindow = ::FindWindow( NULL, pszWindowTitle );
	::GetWindowThreadProcessId( TheWindow, &nProcessID );
	hProcessHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE, nProcessID );
	if( hProcessHandle != 0)
		return TRUE;

	return FALSE;
}

BOOL MiscHelper::KillProcess(LPCTSTR pszClassName, LPCTSTR pszWindowTitle)
{
	HANDLE hProcessHandle;  
	ULONG nProcessID;
	HWND TheWindow;

	TheWindow = ::FindWindow( NULL, pszWindowTitle );
	::GetWindowThreadProcessId( TheWindow, &nProcessID );
	hProcessHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE, nProcessID );
	return ::TerminateProcess( hProcessHandle, 4 );
}

// 获取EverFav的配置文件路径
wchar_t* MiscHelper::GetConfig()
{
	/*C:\Documents and Settings\zhangzhongqing.SNDA\Local Settings\Temp\urltraveler\unpackage*/
	wstring wstrUpdate = PathHelper::GetAppDataDir() + L"\\urltraveler\\";
	if( PathHelper::IsDirExist(wstrUpdate) == FALSE)
		PathHelper::CreateMultipleDirectory(wstrUpdate);

	wstrUpdate += L"EverFav.ini";
	if( PathHelper::IsFileExist(wstrUpdate) == FALSE)
	{
		FileHelper::CreateFile(wstrUpdate.c_str());
		if( PathHelper::IsFileExist(wstrUpdate) == TRUE)
		{
			CTxConfig txConfig;
			txConfig.MakeConfig(StringHelper::UnicodeToANSI(wstrUpdate));
		}		
	}

	return wcsdup(wstrUpdate.c_str());
}

void MiscHelper::SetVersionInConfig(int nVersion)
{
	MiscHelper::SaveKeyValueConfig(L"version", StringHelper::ConvertFromInt(nVersion).c_str());
}

void	 MiscHelper::SaveKeyValueConfig(LPCTSTR pszKeyName, LPCSTR pszValue)
{
	if( pszKeyName == NULL)
		return;

	wchar_t* pConfig = MiscHelper::GetConfig();
	ASSERT(pConfig != NULL);

	CTxConfig txConfig;
	txConfig.ParseConfig(StringHelper::UnicodeToANSI(pConfig).c_str());

	if( pszValue == NULL)
		txConfig.SetValue(pszKeyName, "");
	else
		txConfig.SetValue(pszKeyName, pszValue);

	txConfig.MakeConfig(StringHelper::UnicodeToANSI(pConfig));
}
