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

// 定时器相关
#define IDT_HIDDEN		0
#define IDT_APPEARING		1
#define IDT_WAITING		2
#define IDT_DISAPPEARING	3

class CUpdateHintWnd : public CWindowWnd, public INotifyUI
{
public:
	CUpdateHintWnd();
	~CUpdateHintWnd();
	LPCTSTR GetWindowClassName() const { return _T("UpdateHintWnd"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void OnInit();
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
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	void SetDownloadUrl(String strDownloadUrl);
	void	SetSavePath(String strSavePath);
	void	SetVersion(String	strVersion);
	void	SetSize(float fSize);
	void	AddUpdateDetail(String strDetail);
	void	BeginToShowUpdateInfoWnd();

public:
	CPaintManagerUI m_pm;

	// 更新内容相关
	String	m_strDownloadUrl;
	String	m_strSavePath;
	String	m_strVersion;
	float		m_fSize;
	std::vector<wstring>	m_vUpdateDetail;

	// 位置相关
	CRect	m_UpdateTipWindowRect;

	int m_nStartPosX;
	int m_nStartPosY;
	int m_nCurrentPosX;
	int m_nCurrentPosY;
	int m_nTaskbarPlacement;
	int m_nIncrement;
};
