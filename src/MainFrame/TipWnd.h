#pragma  once

#include "stdafx.h"
#include "UIlib.h"
#include "ThreadObject_i.h"

using namespace DuiLib;

#define WM_TIPCLOSE    (WM_USER + 1)
#define POPTIP_TIMEOUT 1

class CTipWnd : public CWindowWnd, public INotifyUI
{
public:
	CTipWnd() : m_pOwner(NULL), m_nEventId(0),m_pTip(NULL), m_pIcon(NULL) { };
	~CTipWnd()
	{
		::SendMessage(m_pOwner->GetManager()->GetPaintWindow(), WM_TIPCLOSE, 0, 0);
	}

	void Init(CControlUI* pOwner) ;

	const TImageInfo *AddIcon16(LPCTSTR szIconName, HICON hIcon) ;
	bool RemoveImage(LPCTSTR bitmap) ;
	void ShowTips(int nDelayMilliseconds, CStdString strTip, CStdString strIcon) ;
	void HideTip() ;
	LPCTSTR GetWindowClassName() const { return _T("TipWnd"); };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Notify(TNotifyUI& msg) ;

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) ;

public:
	CPaintManagerUI m_pm;
	CControlUI* m_pOwner;
	HWND        m_hParent;
	bool bFlag; // 菜单Notify中尽量不要调用MessageBox函数，如果确实需要调用，使用此变量修正
	CStdString m_strTip;
	CStdString m_strIcon;
	int m_nEventId;
	CRect       m_rc;
	CTextUI    *m_pTip;
	CTextUI    *m_pIcon;
};
