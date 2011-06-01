#include "stdafx.h"
#include "MainFrameWnd.h"

CFrameWnd::CFrameWnd()
{
	m_pLoginFrame = NULL;	
}

void CFrameWnd::OnPrepare() 
{ 

}

void CFrameWnd::LoadFavoriteTree(FAVORITELINEDATA*	pFavoriteData, int nNum)
{	 
	TreeListUI* pGameList = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritelist")));
	TreeListUI::Node* pCategoryNode = NULL;
	TreeListUI::Node* pGameNode = NULL;
	TreeListUI::Node* pServerNode = NULL;
	TreeListUI::Node* pRoomNode = NULL;
	pCategoryNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}推荐游戏"));
	for( int i = 0; i < 4; ++i )
	{
	    pGameNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 10}{x 4}四人斗地主"), pCategoryNode);
	    for( int i = 0; i < 3; ++i )
	    {
		pServerNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 10}{x 4}测试服务器"), pGameNode);
		for( int i = 0; i < 3; ++i )
		{
		    pRoomNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 10}{x 4}测试房间"), pServerNode);
		}
	    }
	}
	pCategoryNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}最近玩过的游戏"));
	for( int i = 0; i < 2; ++i )
	{
	    pGameList->AddNode(_T("三缺一"), pCategoryNode);
	}
	pCategoryNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}棋牌游戏"));
	for( int i = 0; i < 8; ++i )
	{
	    pGameList->AddNode(_T("双扣"), pCategoryNode);
	}
	pCategoryNode = pGameList->AddNode(_T("{x 4}{i gameicons.png 18 3}{x 4}休闲游戏"));
	for( int i = 0; i < 8; ++i )
	{
	    pGameList->AddNode(_T("飞行棋"), pCategoryNode);
	}
}


void CFrameWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if( msg.sType == _T("click") ) {
		if( msg.pSender->GetName() == L"closebtn" ) {
			PostQuitMessage(0);
			return; 
		}
		else if( msg.pSender->GetName() == L"minbtn" ) { 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
		else if( msg.pSender->GetName() == L"maxbtn" ) { 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
		else if( msg.pSender->GetName() == L"restorebtn" ) { 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
		else if( msg.pSender->GetName() == L"menubtn" ) { 

			HMENU	hPopMenu;
			hPopMenu = ::LoadMenuW(g_hModule, MAKEINTRESOURCE(IDR_MENU1)); 
			hPopMenu = ::GetSubMenu(hPopMenu, 0);

			POINT pt;
			GetCursorPos(&pt);
			::TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x - 10,pt.y + 17,0, this->m_hWnd, NULL);
		}

	}
	else if(msg.sType==_T("setfocus"))
	{
		CStdString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
		if(name==_T("examine"))
			pControl->SelectItem(0);
		else if(name==_T("trojan"))
			pControl->SelectItem(1);
		else if(name==_T("plugins"))
			pControl->SelectItem(2);
		else if(name==_T("vulnerability"))
			pControl->SelectItem(3);
		else if(name==_T("rubbish"))
			pControl->SelectItem(4);
		else if(name==_T("cleanup"))
			pControl->SelectItem(5);
		else if(name==_T("fix"))
			pControl->SelectItem(6);
		else if(name==_T("tool"))
			pControl->SelectItem(7);
	}
	else if( msg.sType == _T("itemclick") ) 
	{
	    TreeListUI* pGameList = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritelist")));
	    if( pGameList->GetItemIndex(msg.pSender) != -1 )
	    {
			if( _tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0 ) 
			{
			    TreeListUI::Node* node = (TreeListUI::Node*)msg.pSender->GetTag();

			    POINT pt = { 0 };
			    ::GetCursorPos(&pt);
			    ::ScreenToClient(m_pm.GetPaintWindow(), &pt);
			    pt.x -= msg.pSender->GetX();
			    SIZE sz = pGameList->GetExpanderSizeX(node);
			    if( pt.x >= sz.cx && pt.x < sz.cy )                     
					pGameList->SetChildVisible(node, !node->data()._child_visible);
			}
		}
	}
	/*
else if( msg.sType == _T("itemactivate") ) {
    GameListUI* pGameList = static_cast<GameListUI*>(m_pm.FindControl(_T("gamelist")));
    if( pGameList->GetItemIndex(msg.pSender) != -1 )
    {
	if( _tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0 ) {
	    GameListUI::Node* node = (GameListUI::Node*)msg.pSender->GetTag();
	    pGameList->SetChildVisible(node, !node->data()._child_visible);
	    if( node->data()._level == 3 ) {
		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("roomswitch")));
		if( pControl ) {
		    CHorizontalLayoutUI* pH = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("roomswitchpanel")));
		    if( pH ) pH->SetVisible(true);
		    pControl->SetText(node->parent()->parent()->data()._text);
		    pControl->Activate();
		}
	    }
	}
    }
	*/

}

LRESULT CFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CDialogBuilderCallbackEx cb;
	CControlUI* pRoot = builder.Create(_T("UrlTraveler.xml"), (UINT)0,  &cb, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CFrameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	// 		if( !::IsZoomed(*this) ) {
	// 			RECT rcSizeBox = m_pm.GetSizeBox();
	// 			if( pt.y < rcClient.top + rcSizeBox.top ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
	// 				return HTTOP;
	// 			}
	// 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
	// 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
	// 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
	// 				return HTBOTTOM;
	// 			}
	// 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
	// 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	// 		}

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	return lRes;
}

LRESULT CFrameWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
		int wmId    = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);

		switch (wmId)
		{
			// 弹出登录对话框
		case IDM_LOGIN:
			{
				CLoginFrameWnd* pLoginFrame = new CLoginFrameWnd();
				if( pLoginFrame == NULL ) { Close(); return 0; }
				pLoginFrame->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
				pLoginFrame->CenterWindow();
				ShowModal(*pLoginFrame);
			}

			break;

		case IDM_ABOUT:
			break;

		case IDM_SETTING:
			break;

		default:
			break;
		}

		return 0;
}


LRESULT CFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_COMMAND:   lRes = OnCommand(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}