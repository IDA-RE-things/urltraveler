#include "stdafx.h"
#include "TipWnd.h"
#include "XString.h"
#include "UIHelper.h"

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
		POINT ptShowTip;
		GetCursorPos(&ptShowTip);

		// 根据m_strTip获得标题和链接的文字
		String	strTip = m_strTip.GetData();
		int nIndex = strTip.Find(L"<y 20>");
		String	strTitle = strTip.SubStr(0, nIndex-1);

		// 剔除<y 20><a>头部和</a>尾部
		String	strUrl = strTip.SubStr(nIndex+6 + 3, strTip.GetLength() - 4);

		SIZE szTitle = {0,0};
		::GetTextExtentPoint32W(::GetDC(m_hWnd), strTitle.GetData(), strTitle.GetLength(), &szTitle);

		SIZE szUrl = {0,0};
		::GetTextExtentPoint32W(::GetDC(m_hWnd), strUrl.GetData(), strUrl.GetLength(), &szUrl);

		int nLength = max(szTitle.cx, szUrl.cx);
		if( nLength > 700)
			nLength = 700;

		if( szTitle.cx > nLength)
		{
			UINT nEndIndex = strTitle.GetLength() - 1;
			UIHelper::GetFittableStringFromFront(GetDC(m_hWnd), NULL, strTitle.GetData(), nLength,0,nEndIndex);
			strTitle = strTitle.SubStr(0, nEndIndex);
			strTitle += L"...";
		}

		if( szUrl.cx > nLength)
		{
			UINT nEndIndex = strUrl.GetLength() - 1;
			UIHelper::GetFittableStringFromFront(GetDC(m_hWnd), NULL, strUrl.GetData(), nLength,0,nEndIndex);
			strUrl = strUrl.SubStr(0, nEndIndex);
			strUrl += L"...";
		}

		m_strTip = strTitle;
		m_strTip += L"\n<y 20><a>";
		m_strTip += strUrl.GetData();
		m_strTip += L"</a>";

		CRect rc;
		rc.left = ptShowTip.x + 10;
		rc.top = ptShowTip.y + 10;
		rc.right = ptShowTip.x + nLength + 10;
		rc.bottom = ptShowTip.y + 50;

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
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) 
	{
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