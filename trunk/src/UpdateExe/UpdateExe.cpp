#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "resource.h"
#include "ControlEx.h"
#include "XUnzip.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "FileHelper.h"
#include "PathHelper.h"
#include "atlconv.h"
#include <string>
#include "json/json.h"
#include "shellapi.h"
#include "UpdateThread.h"
#include "UpdateProcessWnd.h"

using namespace std;

CUpdateProcessWnd* pUpdateExeWnd = NULL;
BOOL	g_bRestart = TRUE;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE  hPrevInstance , LPSTR  lpCmdLine, int nCmdShow)
{
	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (szArgList == NULL)
		return -1;

	if( argCount < 3)
		return -1;

	wstring strBinPath = szArgList[1];
	wstring strVersion = szArgList[2];
	LocalFree(szArgList);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("\\skin\\UrlTraveler"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	pUpdateExeWnd = new CUpdateProcessWnd();
	if( pUpdateExeWnd == NULL ) return 0;
	pUpdateExeWnd->SetInstance(hInstance);
	pUpdateExeWnd->Create(NULL, _T("EverFav 云端收藏夹更新安装"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pUpdateExeWnd->CenterWindow();
	::ShowWindow(*pUpdateExeWnd, SW_SHOW);

	// 启动线程
	CUpdateThread* pUpdateThread = new CUpdateThread();
	IThreadObject*      	m_pThreadObj = CreateThreadObject();
	m_pThreadObj->CreateThread(static_cast<IThreadEvent *>(pUpdateThread));

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

	if (m_pThreadObj)
	{
		m_pThreadObj->ShutdownThread(0);
		m_pThreadObj->Release();
	}

	// 是否需要重新启动
	if( g_bRestart == TRUE)
	{
		// 启动主程序
		ShellExecuteW(NULL, _T("open"), strBinPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		MiscHelper::SetVersionInConfig(StringHelper::ConvertToInt(StringHelper::UnicodeToANSI(strVersion)));
	}

	return 0;
}