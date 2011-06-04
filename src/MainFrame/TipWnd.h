#pragma  once

#include "stdafx.h"
#include "UIlib.h"
#include "uCapture.h"
#include "ThreadObject_i.h"

using namespace std;
using namespace DuiLib;

#define WM_TIPCLOSE    (WM_USER + 1)

class CTipWnd : public CWindowWnd, public INotifyUI, public IThreadEvent
{
public:
    CTipWnd() : m_pOwner(NULL) { };
	~CTipWnd()
	{
		::SendMessage(m_pOwner->GetManager()->GetPaintWindow(), WM_TIPCLOSE, 0, 0);
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
		m_pThis = this;

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
		OutputDebugString(msg.sType);
		if (msg.sType == L"link")
		{
			CAPTURECALLBACK cb;
			cb.fn1 = OnCaptureFinish;
			cb.fn2 = OnQueryCaptureSize;
			Capture(m_pOwner->GetManager()->GetPaintWindow(), L"http://www.baidu.com", &cb);
			m_pThreadObj->CreateThread(this);
		}
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

		m_pThreadObj = CreateThreadObject();

        return 0;
    }

    LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
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
	//! 进入线程虚函数
	virtual void OnThreadEntry()
	{

	}

	//! RUN
	static void OnCaptureFinish(HBITMAP hBitmap)
	{
		CControlUI *pRoot = NULL;
		m_pThis->m_pm.RemoveImage(L"shuoluetu");
		m_pThis->m_pm.AddImage(L"shuoluetu", hBitmap, m_pThis->m_nImageWidth, m_pThis->m_nImageHeight);
		pRoot = m_pThis->m_pm.GetRoot();

		RECT rc = pRoot->GetPos();

		CTextUI *image = static_cast<CTextUI *>(m_pThis->m_pm.FindControl(L"TextUI1"));
		rc.bottom = m_pThis->m_nImageHeight;
		rc.right = m_pThis->m_nImageWidth; 
		//pRoot->SetPos(rc);
		//image->SetPos(rc);
		image->SetBkImage(L"shuoluetu");


	}
	static void OnQueryCaptureSize(long *pnWidth, long *pnHeight)
	{
		m_pThis->m_nImageHeight = *pnHeight;
		m_pThis->m_nImageWidth = *pnWidth;
		m_pThis->ResizeClient(*pnWidth, *pnHeight);
	}

	virtual int Run()
	{
		
		return 0;
	}
	//! 线程退出虚函数
	virtual void OnThreadExit()
	{

	}

	// Event处理函数

public:
    CPaintManagerUI m_pm;
    CControlUI* m_pOwner;
    bool bFlag; // 菜单Notify中尽量不要调用MessageBox函数，如果确实需要调用，使用此变量修正
	CStdString m_strTip;
	IThreadObject*      m_pThreadObj;
	int m_nImageWidth;
	int m_nImageHeight;
	static CTipWnd *m_pThis;
};
