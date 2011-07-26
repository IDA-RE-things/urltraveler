#include "TipWnd.h"

void CTipWnd::Init(CControlUI* pOwner) 
{
	if( pOwner == NULL ) return;
	m_pOwner = pOwner;

	CRect rc;

	rc.left = 0;
	rc.top = 0;
	rc.right = 0;
	rc.bottom = 0;

	Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, NULL, rc);
	HWND hWndParent = m_hWnd;
	while( ::GetParent(hWndParent) != NULL ) 
		hWndParent = ::GetParent(hWndParent);

	m_hParent = hWndParent;
}

const TImageInfo *CTipWnd::AddIcon16(LPCTSTR szIconName, HICON hIcon)
{
	return m_pm.AddIcon16(szIconName, hIcon);
}

bool CTipWnd::RemoveImage(LPCTSTR bitmap)
{
	return m_pm.RemoveImage(bitmap);
}

void CTipWnd::ShowTips(int nDelayMilliseconds, CStdString strTip, CStdString strIcon)
{
	if (IsWindow(m_hWnd) == FALSE)
	{
		return;
	}

	m_strTip = strTip;
	m_strIcon = strIcon;

	if (nDelayMilliseconds == 0)
	{
		POINT pt1;
		GetCursorPos(&pt1);

		CRect rc;
		rc.left = pt1.x + 10;
		rc.top = pt1.y + 10;
		rc.right = pt1.x + 400;
		rc.bottom = pt1.y + 100;

		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CRect rcWork = oMonitor.rcWork;
		int nWidth = rc.GetWidth();
		int nHeight = rc.GetHeight();
		if( rc.bottom > rcWork.bottom ) 
		{
			if( nHeight >= rcWork.GetHeight() ) 
			{
				rc.top = 0;
				rc.bottom = nHeight;
			}
			else 
			{
				rc.bottom = rcWork.bottom;
				rc.top = rc.bottom - nHeight;
			}
		}
		if( rc.right > rcWork.right ) 
		{
			if( nWidth >= rcWork.GetWidth() ) 
			{
				rc.left = 0;
				rc.right = nWidth;
			}
			else 
			{
				rc.right = rcWork.right;
				rc.left = rc.right - nWidth;
			}
		}

		SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 
			SWP_NOACTIVATE | SWP_SHOWWINDOW);

		if (m_pTip)
		{
			m_pTip->SetText(m_strTip);
		}

		if (m_pIcon)
		{
			m_pIcon->SetText(m_strIcon);
		}

		SIZE sz = {0, 0};

		CRect rcTip = m_pTip->GetPos();
		sz = m_pTip->EstimateSize(sz);
		this->ResizeClient(sz.cx + rcTip.left + 20, sz.cy);

		return;
	}

	m_nEventId = SetTimer(m_hWnd, POPTIP_TIMEOUT, nDelayMilliseconds, NULL);
}

void CTipWnd::HideTip()
{
	if (m_nEventId != 0)
	{
		KillTimer(m_hWnd, m_nEventId);
		m_nEventId = 0;
	}

	::ShowWindow(m_hWnd, SW_HIDE);
	//::SendMessage(m_hParent, WM_ACTIVATE, TRUE, 0L);
}

void CTipWnd::Notify(TNotifyUI& msg)
{

}

LRESULT CTipWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("tip.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");

	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	m_pm.SetRoundCorner(3, 3);
	m_pIcon = static_cast<CTextUI *>(m_pm.FindControl(_T("icon")));
	m_pTip = static_cast<CTextUI *>(m_pm.FindControl(_T("tips")));

	return 0;
}

LRESULT CTipWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CTipWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == VK_ESCAPE ) Close();
	return 0;
}

LRESULT CTipWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == m_nEventId)
	{
		if (m_nEventId != 0)
		{
			KillTimer(m_hWnd, m_nEventId);
			m_nEventId = 0;
		}

		ShowTips(0, m_strTip, m_strIcon);
	}

	return FALSE;
}

LRESULT CTipWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CTipWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
			case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
			case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
			case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
			case WM_MOUSEWHEEL:    break;
			case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
			case WM_TIMER:         lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
			default:
				bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}