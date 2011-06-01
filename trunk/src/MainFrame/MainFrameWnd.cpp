#include "stdafx.h"
#include "MainFrameWnd.h"

CFrameWnd::CFrameWnd()
{
	m_pLoginFrame = NULL;

	m_nFavoriteNum = 0;
	m_pFavoriteData	= NULL;
}

void CFrameWnd::OnPrepare() 
{ 

}

void CFrameWnd::LoadFavoriteTree(FAVORITELINEDATA*	pFavoriteData, int nNum)
{	 
	m_nFavoriteNum = nNum;
	m_pFavoriteData = pFavoriteData;

	TreeListUI* pFavoriteTree = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritelist")));

	for( int i=0; i<m_nFavoriteNum; i++)
	{
		FAVORITELINEDATA* pData = &m_pFavoriteData[i];

		// 根目录结点
		if( pData != NULL && pData->bFolder == true && pData->nPid == 0)
		{
			wstring wstrText = L"{x 4}{x 4}";
			wstrText += pData->szTitle;

			TreeListUI::Node* pNode = pFavoriteTree->AddNode(wstrText.c_str());
			m_mapIdNode[pData->nId] = pNode;
			m_mapNodeId[pNode] = pData->nId;
		}
		// 如果不是根结点
		else if( pData != NULL && pData->bFolder == true)
		{
			// 找到当前结点的父节点
			int nPid = pData->nPid;
			std::map<int, TreeListUI::Node*>::iterator itr = m_mapIdNode.find(nPid);
			if( itr != m_mapIdNode.end())
			{
				TreeListUI::Node* pParentNode = itr->second;
				if( pParentNode != NULL)
				{
					wstring wstrText = L"{x 4}{x 4}";
					wstrText += pData->szTitle;

					TreeListUI::Node* pNode  = pFavoriteTree->AddNode(wstrText.c_str(), pParentNode);
					m_mapIdNode[pData->nId] = pNode;
					m_mapNodeId[pNode] = pData->nId;
				}
			}
		}
	}


    CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
    pUserList->SetTextCallback(this);      
    for( int i = 0; i < 10; i++ ) {
        CListTextElementUI* pListElement = new CListTextElementUI;
        pUserList->Add(pListElement);
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
	    TreeListUI* pFavoriteTree = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritelist")));
	    if( pFavoriteTree->GetItemIndex(msg.pSender) != -1 )
	    {
			if( _tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0 ) 
			{
			    TreeListUI::Node* node = (TreeListUI::Node*)msg.pSender->GetTag();

			    POINT pt = { 0 };
			    ::GetCursorPos(&pt);
			    ::ScreenToClient(m_pm.GetPaintWindow(), &pt);
			    pt.x -= msg.pSender->GetX();
			    SIZE sz = pFavoriteTree->GetExpanderSizeX(node);
			    if( pt.x >= sz.cx && pt.x < sz.cy )                     
					pFavoriteTree->SetChildVisible(node, !node->data()._child_visible);

				// 得到该结点对应的nId
				std::map<TreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(node);
				if( itr != m_mapNodeId.end())
				{
					int nId = itr->second;

					m_vFavoriteNode.clear();
					
					for( int i=0; i<m_nFavoriteNum; i++)
					{
						FAVORITELINEDATA* pData = &m_pFavoriteData[i];

						// 叶子结点
						if( pData->nPid == nId && pData->bFolder == false)
						{
							m_vFavoriteNode.push_back(pData);
						}
					}

					CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
					if( pUserList)
					{
						pUserList->SetItemTextStyle(DT_LEFT);
						pUserList->Invalidate();
					}
				}
			}
		}
	}
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

// 提供表单中的数据
LPCTSTR CFrameWnd::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
    if( pControl->GetParent()->GetParent()->GetName() == _T("favoritefilelist") ) 
	{
		if( m_vFavoriteNode.size() == 0)
			return L"";

		if( iIndex <= m_vFavoriteNode.size() -1)
		{
			if( iSubItem == 0 ) return _T("<i vip.png>");
			if( iSubItem == 1 ) return m_vFavoriteNode[iIndex]->szTitle;
			if( iSubItem == 2 ) return m_vFavoriteNode[iIndex]->szUrl;
		}

    }

    return _T("");
}