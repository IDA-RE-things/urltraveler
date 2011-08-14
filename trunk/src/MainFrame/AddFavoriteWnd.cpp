#include "stdafx.h"
#include "AddFavoriteWnd.h"

#include "MiscHelper.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "XString.h"

#include "DataCenterDefine.h"
#include "MainFrameDefine.h"
#include "MainFrameModule.h"

using namespace datacenter;
using namespace mainframe;

CAddFavoriteWnd::CAddFavoriteWnd()
{
	m_nFavoriteId = 0;

	m_pTitleEdit	=	NULL;
	m_pUrlEdit	=	NULL;
}

CAddFavoriteWnd::~CAddFavoriteWnd()
{
}

void CAddFavoriteWnd::OnPrepare(TNotifyUI& msg) 
{ 
	m_pUrlEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("UrlEdit")));
	m_pTitleEdit= static_cast<CEditUI*>(m_pm.FindControl(_T("TitleEdit")));

	if( m_pTitleEdit == NULL || m_pUrlEdit == NULL)
	{
		ASSERT(0);
	}

	m_pTitleEdit->SetFocus();
}

void	CAddFavoriteWnd::OnAddUrl()
{
	if( m_pTitleEdit->GetText() == L"")
	{
		MessageBox(NULL, L"网址标题不能为空", L"提示", MB_OK);
		m_pTitleEdit->SetFocus();
		return;
	}

	if( m_pUrlEdit->GetText() == L"")
	{
		MessageBox(NULL, L"网址不能为空", L"提示", MB_OK);
		m_pUrlEdit->SetFocus();
		return;
	}

	String	strUrl = m_pUrlEdit->GetText();
	if( strUrl.Left(7) != L"http://" && strUrl.Left(8) != L"https://")
	{
		strUrl = String(L"http://") + strUrl;
	}


	DataCenter_AddFavoriteEvent* pEvent = new DataCenter_AddFavoriteEvent();
	pEvent->srcMId = MODULE_ID_MAINFRAME;
	pEvent->desMId = MODULE_ID_DATACENTER;
	pEvent->nParentFavoriteId = m_nFavoriteId;
	STRNCPY(pEvent->szTitle, m_pTitleEdit->GetText());
	STRNCPY(pEvent->szUrl, strUrl.GetData());
	g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);

	m_pTitleEdit->SetText(L"");
	m_pUrlEdit->SetText(L"");
	m_pTitleEdit->SetFocus();
}

void CAddFavoriteWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) 
	{
		OnPrepare(msg);
	}
	else if( msg.sType == _T("click") )
	{
		if( msg.pSender->GetName() == L"closebtn" ) 
		{
			Close();
		}
		else if( msg.pSender->GetName() == L"CancelBtn" ) 
		{
			Close();
		}
		else if( msg.pSender->GetName() == L"AddBtn" ) 
		{
			OnAddUrl();
		}
	}
	else if( msg.sType == _T("return") )
	{
		CControlUI* pControl = m_pm.GetFocus();	
		if( pControl == NULL)
			return;

		if( pControl->GetName() == L"TitleEdit")
		{
			if( m_pUrlEdit)
				m_pUrlEdit->SetFocus();

			return;
		}

		if( pControl->GetName() == L"UrlEdit")
		{
			OnAddUrl();
			return;
		}
	}
}

LRESULT CAddFavoriteWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("addfavorite.xml"), (UINT)0,  NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	return 0;
}

LRESULT CAddFavoriteWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CAddFavoriteWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CAddFavoriteWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CAddFavoriteWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CAddFavoriteWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0// &&
				/*_tcscmp(pControl->GetClass(), _T("TextUI")) != 0*/ )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CAddFavoriteWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) )
	{
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, 
			rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CAddFavoriteWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CAddFavoriteWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) 
	{
		PostMessageW(WM_CLOSE,0,0);
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CAddFavoriteWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int wmId    = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);

	switch (wmId)
	{
		// 弹出登录对话框
	case IDM_LOGIN:
		{
		}

		break;

	default:
		break;
	}

	return 0;
}

LRESULT CAddFavoriteWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == VK_ESCAPE ) 
	{
		Close();
	}

	return 0;
}


LRESULT CAddFavoriteWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) 
	{
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_COMMAND:       lRes = OnCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_KEYDOWN: lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}