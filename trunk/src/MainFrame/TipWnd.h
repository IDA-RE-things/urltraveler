#pragma  once

#include "stdafx.h"
#include "UIlib.h"

using namespace std;
using namespace DuiLib;

class CTipWnd : public CWindowWnd, public INotifyUI
{
public:
    CTipWnd() : m_pOwner(NULL) { };
	~CTipWnd()
	{
		int i = 0;
	}
    void Init(CControlUI* pOwner, CRect rc, CStdString strTip) {
        if( pOwner == NULL ) return;
        m_pOwner = pOwner;

        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CRect rcWork = oMonitor.rcWork;
        int nWidth = rc.GetWidth();
        int nHeight = rc.GetHeight();
        if( rc.bottom > rcWork.bottom ) {
            if( nHeight >= rcWork.GetHeight() ) {
                rc.top = 0;
                rc.bottom = nHeight;
            }
            else {
                rc.bottom = rcWork.bottom;
                rc.top = rc.bottom - nHeight;
            }
        }
        if( rc.right > rcWork.right ) {
            if( nWidth >= rcWork.GetWidth() ) {
                rc.left = 0;
                rc.right = nWidth;
            }
            else {
                rc.right = rcWork.right;
                rc.left = rc.right - nWidth;
            }
        }

		m_strTip = strTip;

        Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, rc);
        HWND hWndParent = m_hWnd;
        while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
        ::ShowWindow(m_hWnd, SW_SHOW);
        ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
    }

    LPCTSTR GetWindowClassName() const { return _T("TipWnd"); };
    void OnFinalMessage(HWND /*hWnd*/) { delete this; };

    void Notify(TNotifyUI& msg)
    {
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("tip.xml"), (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");

        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        m_pm.SetRoundCorner(3, 3);

		CTextUI *pTip = static_cast<CTextUI *>(m_pm.FindControl(_T("tips")));
		pTip->SetText(m_strTip);

        return 0;
    }

    LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( m_hWnd != (HWND) wParam ) PostMessage(WM_CLOSE);
        return 0;
    }

    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( wParam == VK_ESCAPE ) Close();
        return 0;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch( uMsg ) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
        case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
        case WM_MOUSEWHEEL:    break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
        }
        if( bHandled ) return lRes;
        if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

public:
    CPaintManagerUI m_pm;
    CControlUI* m_pOwner;
    bool bFlag; // 菜单Notify中尽量不要调用MessageBox函数，如果确实需要调用，使用此变量修正
	CStdString m_strTip;
};
