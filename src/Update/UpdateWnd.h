// App.cpp : Defines the entry point for the application.
//
#pragma  once

#include "stdafx.h"
#include <objbase.h>
#include "UIlib.h"
#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "resource.h"
#include "PlugIn.h"
#include <map>
#include <vector>
#include "UICommonControls.h"
#include "XString.h"

using namespace std;
using namespace DuiLib;

extern HMODULE	g_hModule;

class CUpdateWnd : public CWindowWnd, public INotifyUI
{
public:
	CUpdateWnd();
	~CUpdateWnd();
	LPCTSTR GetWindowClassName() const { return _T("UIUpdateWnd"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Init(){} ;
	void OnPrepare(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  ;
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void	SetHintMsg(String strMsg);
	void SetDownLoadProgress( UINT nPercent);
	void	SetNewVersion(int nNewVersion) ;
	void	SetCurrentVersion(int nCurrentVersion);

public:
	CPaintManagerUI m_pm;
	CProgressUI*	m_pProcess;
	
	int	m_nHighestVersion;
	int	m_nCurrentVersion;
	String	m_strHintMsg;
};
