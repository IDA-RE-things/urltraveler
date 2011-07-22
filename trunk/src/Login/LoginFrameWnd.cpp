#include "stdafx.h"
#include "LoginFrameWnd.h"
#include "ControlEx.h"
#include "MainFrameDefine.h"
#include "LoginModule.h"

using namespace mainframe;

void CLoginFrameWnd::Init() 
{
}

void CLoginFrameWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) {
		if( msg.pSender->GetName() == _T("closebtn") ) { Close(); return; }
		else if( msg.pSender->GetName() == _T("loginBtn") ) 
		{ 
			Close(); return; 
		}
		else if( msg.pSender->GetName() == _T("SelectLoginAccountBtn") ) 
		{ 
			Close(); 

			// 强制弹出更新窗口进行升级
			mainframe::MainFrame_GetWndService stGetWndService;
			login::g_LoginModule->GetModuleManager()->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, 
				(param)&stGetWndService);

			CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
			ASSERT(pMainFrameWnd != NULL);

			CLoginPreWnd* pLoginFrame = new CLoginPreWnd();
			if( pLoginFrame == NULL )
				return ;

			pLoginFrame->Create(pMainFrameWnd->GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
			pLoginFrame->CenterWindow();
			//pMainFrameWnd->ShowModal(*pLoginFrame);
		}
	}
	else if( msg.sType == _T("itemselect") ) {
		if( msg.pSender->GetName() == _T("accountcombo") ) {
			CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("accountedit")));
			if( pAccountEdit ) pAccountEdit->SetText(msg.pSender->GetText());
		}
	}
}

LRESULT CLoginFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	CControlUI* pRoot = builder.Create(_T("login.xml"), (UINT)0, &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	SIZE szInitWindowSize = m_pm.GetInitSize();
	if( szInitWindowSize.cx != 0 ) {
		::SetWindowPos(*this, NULL, 0, 0, szInitWindowSize.cx, szInitWindowSize.cy, SWP_NOZORDER | SWP_NOMOVE);
	}

	Init();
	return 0;
}

LRESULT CLoginFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CLoginFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLoginFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CLoginFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CLoginFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CLoginFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
			case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
			case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
			case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
			case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
			case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
			case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
			default:
				bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CLoginFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN ) {
		if( wParam == VK_RETURN ) {
			CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("accountedit")));
			if( pEdit->GetText().IsEmpty() ) pEdit->SetFocus();
			else {
				pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("pwdedit")));
				if( pEdit->GetText().IsEmpty() ) pEdit->SetFocus();
				else Close();
			}
			return true;
		}
		else if( wParam == VK_ESCAPE ) {
			Close();
			return true;
		}

	}
	return false;
}