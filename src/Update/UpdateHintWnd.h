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

class CUpdateHintWnd : public CWindowWnd, public INotifyUI
{
public:
	CUpdateHintWnd();
	~CUpdateHintWnd();
	LPCTSTR GetWindowClassName() const { return _T("UpdateHintWnd"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Init(){} ;
	void OnPrepare(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  ;
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	void	SetVersion(String	strVersion);
	void	SetSize(float fSize);
	void	AddUpdateDetail(String strDetail);

public:
	CPaintManagerUI m_pm;

	String	m_strVersion;
	float		m_fSize;
	std::vector<wstring>	m_vUpdateDetail;
};
