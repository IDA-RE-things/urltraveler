#include "stdafx.h"
#include "AddFavoriteWnd.h"

#include "MiscHelper.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "XString.h"

#include "DataCenterDefine.h"
#include "MainFrameDefine.h"
#include "MainFrameModule.h"
#include "AddFavoriteFoldNameWnd.h"

using namespace datacenter;
using namespace mainframe;

CAddFavoriteFoldNameWnd::CAddFavoriteFoldNameWnd()
{
	m_nParentId = 0;
	m_pNameEdit	=	NULL;
}

CAddFavoriteFoldNameWnd::~CAddFavoriteFoldNameWnd()
{
}

void CAddFavoriteFoldNameWnd::OnPrepare(TNotifyUI& msg) 
{ 
	m_pNameEdit= static_cast<CEditUI*>(m_pm.FindControl(_T("EditFav")));
	m_pNameEdit->SetFocus();
}

void	CAddFavoriteFoldNameWnd::OnAddFavoriteFoldName()
{
	if( m_pNameEdit->GetText() == L"")
	{
		MessageBox(NULL, L"�������ղؼ�����", L"��ʾ", MB_OK);
		m_pNameEdit->SetFocus();
		return;
	}

	DataCenter_AddFavoriteFoldEvent* pEvent = new DataCenter_AddFavoriteFoldEvent();
	pEvent->srcMId = MODULE_ID_MAINFRAME;
	pEvent->desMId = MODULE_ID_DATACENTER;
	pEvent->nParentFavoriteId = m_nParentId;
	STRNCPY(pEvent->szTitle, m_pNameEdit->GetText());
	g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
}

void CAddFavoriteFoldNameWnd::Notify(TNotifyUI& msg)
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
		else if( msg.pSender->GetName() == L"AddBtn" ) 
		{
			OnAddFavoriteFoldName();
		}
	}
	else if( msg.sType == _T("return") )
	{
		CControlUI* pControl = m_pm.GetFocus();	
		if( pControl == NULL)
			return;

		if( pControl->GetName() == L"EditFav")
		{
			OnAddFavoriteFoldName();
			return;
		}
	}
}

LRESULT CAddFavoriteFoldNameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("addfold.xml"), (UINT)0,  NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	return 0;
}

LRESULT CAddFavoriteFoldNameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CAddFavoriteFoldNameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CAddFavoriteFoldNameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CAddFavoriteFoldNameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CAddFavoriteFoldNameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAddFavoriteFoldNameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAddFavoriteFoldNameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CAddFavoriteFoldNameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// ��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
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

LRESULT CAddFavoriteFoldNameWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int wmId    = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);

	switch (wmId)
	{
		// ������¼�Ի���
	case IDM_LOGIN:
		{
		}

		break;

	default:
		break;
	}

	return 0;
}

LRESULT CAddFavoriteFoldNameWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == VK_ESCAPE ) 
	{
		Close();
	}

	return 0;
}


LRESULT CAddFavoriteFoldNameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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