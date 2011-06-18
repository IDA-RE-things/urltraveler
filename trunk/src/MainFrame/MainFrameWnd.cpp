#include "stdafx.h"
#include "MainFrameWnd.h"
#include "MainFrameDefine.h"
#include "MainFrameModule.h"
#include "PlugInDefine.h"
#include "DataCenterDefine.h"
#include "WebDefine.h"

#include "ImageHelper.h"
#include "MiscHelper.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "XString.h"

#include "CurlHttp.h"
#include "TipWnd.h"

#include "FavoriteListMenu.h"
#include "TreeListMenu.h"

using namespace datacenter;
using namespace mainframe;

CMainFrameWnd::CMainFrameWnd()
{
	m_pLoginFrame = NULL;

	m_nFavoriteNum = 0;
	m_pFavoriteData	= NULL;
	m_pTipWnd = new CTipWnd();
}

CMainFrameWnd::~CMainFrameWnd()
{
	if (m_pTipWnd)
	{
		m_pTipWnd->SendMessage(WM_CLOSE, 0, 0);
	}
}

void CMainFrameWnd::OnPrepare(TNotifyUI& msg) 
{ 
	m_pTipWnd->Init(msg.pSender); 
}

void CMainFrameWnd::LoadFavoriteTree(FAVORITELINEDATA*	pFavoriteData, int nNum)
{	 
	m_nFavoriteNum = nNum;
	m_pFavoriteData = pFavoriteData;

	TreeListUI* pFavoriteTree = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritetreelist")));

	wstring wstrText = L"{x 4}{x 4}";
	wstrText += L"收藏夹";

	TreeListUI::Node* pRootNode = pFavoriteTree->AddNode(wstrText.c_str());
	m_mapIdNode[0] = pRootNode;
	m_mapNodeId[pRootNode] = 0;

	for( int i=0; i<m_nFavoriteNum; i++)
	{
		FAVORITELINEDATA* pData = &m_pFavoriteData[i];

		if( pData != NULL && pData->bFolder == true)
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
			else
			{
				int i = 0;
			}
		}
	}

	CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
	pUserList->SetTextCallback(this);     

	// 显示根结点下的所有的收藏夹记录
	m_nTreeNodeId = 0;
	m_pCurrentTreeNode	=	pRootNode;
	ShowFavoriteTreeList(0);
}

void	CMainFrameWnd::ShowFavoriteTreeList(int nId)
{
	m_vFavoriteNodeAtTreeNode.clear();

	CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
	pUserList->RemoveAllItems();
	int j = 0;

	for( int i=0; i<m_nFavoriteNum; i++)
	{
		FAVORITELINEDATA* pData = &m_pFavoriteData[i];

		// 叶子结点
		if( pData->nPid == nId && pData->bFolder == false && pData->bDelete == false)
		{
			GetWebSiteFavIcon(pData->szUrl, j);

			CListTextEditElementUI* pListElement = new CListTextEditElementUI;
			pListElement->SetTag((UINT_PTR)pData);
			pUserList->Add(pListElement);
			pListElement->SetColomnEditable(1, TRUE);
			pListElement->SetColomnEditable(2, TRUE);
			m_vFavoriteNodeAtTreeNode.push_back(pData);
			j++;
		}
	}

	CTextUI* pFavoriteNumber = static_cast<CTextUI*>(m_pm.FindControl(_T("FavoriteNum")));
	pFavoriteNumber->SetShowHtml();
	if (pFavoriteNumber)
	{
		TCHAR szFavoriteNum[MAX_PATH] = {0};
		_stprintf_s(szFavoriteNum, MAX_PATH - 1, _T("该文件夹下共有 {b}{c #FF0000}%d{/c}{/b} 个收藏"), j);
		pFavoriteNumber->SetText(szFavoriteNum);
	}

	if( pUserList)
	{						
		pUserList->SetItemTextStyle(pUserList->GetItemTextStyle() & ~ DT_CENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE);
		pUserList->Invalidate();
	}
}

void CMainFrameWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) OnPrepare(msg);
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
		// 点击收藏夹目录的响应代码
		TreeListUI* pFavoriteTree = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritetreelist")));
		if( pFavoriteTree->GetItemIndex(msg.pSender) != -1 )
		{
			if( _tcscmp(msg.pSender->GetClass(), _T("ListLabelElementUI")) == 0 ) 
			{
				TreeListUI::Node* node = (TreeListUI::Node*)msg.pSender->GetTag();

				POINT pt = { 0 };
				::GetCursorPos(&pt);
				::ScreenToClient(m_pm.GetPaintWindow(), &pt);
				pt.x -= msg.pSender->GetX();
				pt.y -= msg.pSender->GetY();
				SIZE sz = pFavoriteTree->GetExpanderSizeX(node);
				if( pt.x >= sz.cx && pt.y < sz.cy )                     
					pFavoriteTree->SetChildVisible(node, !node->data()._child_visible);

				// 得到该结点对应的nId
				std::map<TreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(node);
				if( itr != m_mapNodeId.end())
				{
					int nId = itr->second;
					m_nTreeNodeId = nId;
					m_pCurrentTreeNode	=	itr->first;
					ShowFavoriteTreeList(nId);
				}
			}
		}

		// 点击收藏夹条目的响应代码
		if (msg.pSender->GetParent()->GetParent()->GetName() == L"favoritefilelist")
		{
			if (m_pTipWnd)
			{
				m_pTipWnd->HideTip();
			}
		}
	}
	else if(msg.sType == L"itemhot")
	{
		if (msg.pSender->GetParent()->GetParent()->GetName() == L"favoritefilelist")
		{
			CListElementUI *pItem = (CListElementUI *)msg.pSender;

			if (pItem)
			{
				FAVORITELINEDATA *pData = (FAVORITELINEDATA *)pItem->GetTag();

				CStdString strTips;
				wstring strIcon;

				wstring wstrDomain = MiscHelper::GetDomainFromUrl(m_vFavoriteNodeAtTreeNode[pItem->GetIndex()]->szUrl, FALSE);

				strTips += pData->szTitle;
				strTips += L"\n";
				strTips += L"<y 20><a>";
				strTips += pData->szUrl;
				strTips += L"</a>";

				wstring strIconName;
				strIcon = L"<i ";
				strIcon = strIcon + wstrDomain;
				strIcon += L".ico></i>";

				m_pTipWnd->ShowTips(1000, strTips, strIcon.c_str());
			}
		}
	}
	else if(msg.sType == L"itemunhot")
	{
		if (msg.pSender->GetParent()->GetParent()->GetName() == L"favoritefilelist")
		{
			if (m_pTipWnd)
			{
				m_pTipWnd->HideTip();
			}
		}
	}
	else if(msg.sType == L"menu") 
	{
		if( msg.pSender->GetName() == _T("favoritefilelist") ) 
		{
			CFavoriteListMenu* pMenu = new CFavoriteListMenu(L"list_menu.xml");
			if( pMenu == NULL ) { return; }
			POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
			::ClientToScreen(*this, &pt);
			if (((CListUI *)msg.pSender)->GetCurSel() >= 0)
			{
				pMenu->StoreLanuchPos(msg.ptMouse.x, msg.ptMouse.y);
				pMenu->Init(msg.pSender, CRect(pt.x, pt.y, pt.x + 140, pt.y + 172));
			}
		}
		else 	if( msg.pSender->GetName() == _T("favoritetreelist") ) 
		{
			CTreeListMenu* pMenu = new CTreeListMenu(L"tree_menu.xml");
			if( pMenu == NULL ) { return; }
			POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
			::ClientToScreen(*this, &pt);
			pMenu->Init(msg.pSender, CRect(pt.x, pt.y, pt.x + 140, pt.y + 125));

			TreeListUI* pTree = (TreeListUI*)msg.pSender;
			int nSelIndex = pTree->GetCurSel();
			if( nSelIndex == 0)
			{
				pMenu->Enable(L"menu_Delete", false);
			}
		}
		return;
	}
	else if(msg.sType == L"listeditfinish")
	{
		CControlUI *pControl = msg.pSender;

		if (_tcscmp(pControl->GetName(), _T("favoritefilelist")) == 0)
		{
			CListUI *pListUI = (CListUI *)pControl;

			int nRow = HIWORD(msg.wParam);
			int nColomn = LOWORD(msg.wParam);

			if( nRow > pListUI->GetCount() - 1)
				return;

			FAVORITELINEDATA* pData = m_vFavoriteNodeAtTreeNode[nRow];
			if( pData == NULL)
				return;

			if( nColomn == 1)
			{
				STRNCPY(pData->szTitle, pListUI->GetEditText());
			}
			else if( nColomn == 2)
			{
				String	strUrl = pListUI->GetEditText();
				if( strUrl.Left(7) != L"http://" && strUrl.Left(8) != L"https://")
				{
					strUrl = String(L"http://") + strUrl;
				}
				STRNCPY(pData->szUrl, strUrl.GetData());
				GetWebSiteFavIcon(pData->szUrl, nRow);
			}
			pListUI->Invalidate();

			// 通知DataCenter更新数据
		}
	}
}

LRESULT CMainFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CMainFrameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMainFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0// &&
				/*_tcscmp(pControl->GetClass(), _T("TextUI")) != 0*/ )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CMainFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CMainFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CMainFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

			// 最大化的时候需要修改IconBox的宽度
			CIconBoxUI *pIconBox = (CIconBoxUI *)m_pm.FindControl(L"BrowserBox");
			if (pIconBox)
			{
				pIconBox->SetFixedWidth(880);
				pIconBox->Invalidate();
			}

			// 调整list的宽度
			CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
			if( pUserList)
			{
				CListHeaderUI* pHeader = pUserList->GetHeader();
				RECT rc0 = pHeader->GetItemAt(0)->GetPos();
				rc0.right += 200;
				pHeader->GetItemAt(0)->SetPos(rc0);

				RECT rc1 = pHeader->GetItemAt(1)->GetPos();
				rc1.right -= 200;
				pHeader->GetItemAt(1)->SetPos(rc1);
				pUserList->Invalidate();
			}
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

LRESULT CMainFrameWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


LRESULT CMainFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		case WM_COMMAND:       lRes = OnCommand(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

// 提供表单中的数据
LPCTSTR CMainFrameWnd::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	if( pControl->GetParent()->GetParent()->GetName() == _T("favoritefilelist") ) 
	{
		if( m_vFavoriteNodeAtTreeNode.size() == 0)
			return L"";

		if( iIndex <= m_vFavoriteNodeAtTreeNode.size() -1)
		{
			if( iSubItem == 0 )
			{
				wstring wstrDomain = MiscHelper::GetDomainFromUrl(m_vFavoriteNodeAtTreeNode[iIndex]->szUrl, FALSE);

				wstring strTitle = wstring(L"<x 4><i ") + wstrDomain;
				strTitle += L".ico><x 4>";
				return _wcsdup(strTitle.c_str());
			}
			if( iSubItem == 1 )
			{
				wstring strTitle = m_vFavoriteNodeAtTreeNode[iIndex]->szTitle;
				return _wcsdup(strTitle.c_str());
			}
			if( iSubItem == 2 )
			{
				wstring strUrl;
				strUrl += L"<x 4>";
				strUrl += m_vFavoriteNodeAtTreeNode[iIndex]->szUrl;

				return _wcsdup(strUrl.c_str());
			}
		}
	}

	return _T("");
}

bool CMainFrameWnd::GetWebSiteFavIcon(wstring strUrl, int nRow)
{
	if( strUrl == L"")
		return false;

	// 向数据库查找Icon数据
	datacenter::DataCenter_GetFavoriteIconService getFavoriteIconService;
	STRNCPY(getFavoriteIconService.szDomain, strUrl.c_str());
	g_MainFrameModule->GetModuleManager()->CallService(getFavoriteIconService.serviceId, (param)&getFavoriteIconService);

	wstring wstrIconName =  MiscHelper::GetDomainFromUrl(strUrl.c_str(),FALSE) + wstring(L".ico");
	bool bOk = false;
	bOk = m_pm.AddIcon16(wstrIconName.c_str(), getFavoriteIconService.hIcon) != NULL;
	m_pTipWnd->AddIcon16(wstrIconName.c_str(), getFavoriteIconService.hIcon);

	return bOk;
}

void CMainFrameWnd::UpdateFavoriteIcon( wchar_t* pszUrl, HICON hIcon )
{
	if( pszUrl == NULL)
		return;

	wstring wstrIconName = L"";
	if( hIcon != NULL)
	{
		wstrIconName = MiscHelper::GetDomainFromUrl(pszUrl, FALSE) + wstring(L".ico");
	}
	// 该icon数据库中已经存在
	
	bool bOk = false;
	if( hIcon != NULL)
	{
		m_pm.RemoveImage(wstrIconName.c_str());
		m_pTipWnd->RemoveImage(wstrIconName.c_str());
		bOk = m_pm.AddIcon16(wstrIconName.c_str(), hIcon) != NULL;
		m_pTipWnd->AddIcon16(wstrIconName.c_str(), hIcon);
	}

	CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
	if( pUserList == NULL)
		return;

	pUserList->Invalidate();
}

void	CMainFrameWnd::DeleteFavorite(int nDeleteNodeId)
{
	for( int i=0; i<m_vFavoriteNodeAtTreeNode.size(); i++)
	{
		FAVORITELINEDATA* pData = m_vFavoriteNodeAtTreeNode[i];
		if( pData->nId == nDeleteNodeId)
		{
			m_vFavoriteNodeAtTreeNode.erase(m_vFavoriteNodeAtTreeNode.begin() + i);
			//目前只支持单删除
			break;
		}
	}

	CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
	if( pUserList == NULL)
		return;

	pUserList->Invalidate();
}

void	CMainFrameWnd::DeleteFavoriteFold(int nIndex)
{
	TreeListUI* pFavoriteTree = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritetreelist")));
	if( pFavoriteTree == NULL)
	{
		ASSERT(0);
		return;
	}

	CListLabelElementUI* pElement = (CListLabelElementUI*)pFavoriteTree->GetSubItem(nIndex);
	if( pElement == NULL)
		return;

	TreeListUI::Node* pNode  = (TreeListUI::Node*)pElement->GetTag();
	std::map<TreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(pNode);
	if( itr == m_mapNodeId.end())
		return;

	int nId = itr->second;

	// 检查当前收藏夹下是否存在子文件夹，如果存在则提醒。
	// 向数据中心请求检查是否存在子文件夹
	DataCenter_CheckExistSubFoldService checkService;
	checkService.nFoldId = nId;
	g_MainFrameModule->GetModuleManager()->CallService(checkService.serviceId,(param)&checkService);

	// 存在子文件夹，则提醒是否删除
	if( checkService.bExistSubFolder == TRUE)
	{
		int nRet = ::MessageBox(NULL, L"该收藏夹下存在收藏数据，是否确定要一起删除", L"删除提示", MB_OKCANCEL);
		// 确定删除
		if( nRet == IDOK)
		{
			DataCenter_GetSubFolderIdService getSubFolderService;
			getSubFolderService.nFoldId = nId;
			g_MainFrameModule->GetModuleManager()->CallService(getSubFolderService.serviceId, (param)&getSubFolderService);

			for( int i=0; i<getSubFolderService.nIdNum; i++)
			{
				int nSubFolderId = getSubFolderService.pIdNum[i];

				std::map<int, TreeListUI::Node*>::iterator itr  = m_mapIdNode.find(nSubFolderId);
				if( itr != m_mapIdNode.end())
				{
					TreeListUI::Node* pNode = itr->second;
					if( pNode)
						m_mapNodeId.erase(pNode);
				}
				m_mapIdNode.erase(nSubFolderId);
			}

			pFavoriteTree->RemoveNode(pNode);
			m_mapNodeId.erase(pNode);
			m_mapIdNode.erase(nId);

			g_MainFrameModule->GetModuleManager()->PushEvent(
				MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_DATACENTER_DELETE_FAVORITE_FOLD,
				MODULE_ID_DATACENTER,
				nId));	
		}
	}
	else
	{
		DataCenter_GetSubFolderIdService getSubFolderService;
		getSubFolderService.nFoldId = nId;
		g_MainFrameModule->GetModuleManager()->CallService(getSubFolderService.serviceId, (param)&getSubFolderService);

		for( int i=0; i<getSubFolderService.nIdNum; i++)
		{
			int nSubFolderId = getSubFolderService.pIdNum[i];

			std::map<int, TreeListUI::Node*>::iterator itr  = m_mapIdNode.find(nSubFolderId);
			if( itr != m_mapIdNode.end())
			{
				TreeListUI::Node* pNode = itr->second;
				if( pNode)
					m_mapNodeId.erase(pNode);
			}

			m_mapIdNode.erase(nSubFolderId);
		}

		pFavoriteTree->RemoveNode(pNode);
		m_mapNodeId.erase(pNode);
		m_mapIdNode.erase(nId);

		g_MainFrameModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_DATACENTER_DELETE_FAVORITE_FOLD,
			MODULE_ID_DATACENTER,
			nId));	
	}
}

void CMainFrameWnd::AddUrl()
{
	CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
	if( pUserList == NULL)
		return;

	FAVORITELINEDATA* pData = new FAVORITELINEDATA();
	pData->nPid = m_nTreeNodeId;
	m_vFavoriteNodeAtTreeNode.insert(m_vFavoriteNodeAtTreeNode.begin(), pData);

	pUserList->Invalidate();
	pUserList->SelectItem(0);
}

void	CMainFrameWnd::OpenUrl(int nIndex)
{
	if( nIndex > m_vFavoriteNodeAtTreeNode.size() - 1 || nIndex <0)
		return;

	FAVORITELINEDATA* pData = m_vFavoriteNodeAtTreeNode[nIndex];
	if( wcscmp(pData->szUrl, L"") == 0)
		return;

	ShellExecute(NULL, L"open",pData->szUrl, NULL,NULL,SW_SHOWMAXIMIZED);
}

void	CMainFrameWnd::CopyUrl(int nIndex)
{
	if( nIndex > m_vFavoriteNodeAtTreeNode.size() - 1 || nIndex <0)
		return;

	FAVORITELINEDATA* pData = m_vFavoriteNodeAtTreeNode[nIndex];
	if( wcscmp(pData->szUrl, L"") == 0)
		return;

	BOOL bRet = MiscHelper::SaveTextToClipboard(StringHelper::UnicodeToANSI(pData->szUrl).c_str());
	if( bRet == TRUE)
	{
		MessageBox(L"URL地址已经成功复制到剪贴板", L"复制成功", MB_OK);
	}
}

void	CMainFrameWnd::SelectTreeList(int nId)
{
	TreeListUI* pFavoriteTree = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritetreelist")));
	if( pFavoriteTree == NULL)
	{
		ASSERT(0);
		return;
	}

	if( nId == 0)
	{
		pFavoriteTree->SelectItem(0);
		ShowFavoriteTreeList(0);
		return;
	}

	std::map<int, TreeListUI::Node*>::iterator itr = m_mapIdNode.find(nId);
	if( itr == m_mapIdNode.end())
		return;

	if( itr->second == NULL)
		return;

	int nCount = pFavoriteTree->GetCount();
	for( int i =0; i<nCount;i++)
	{
		CListLabelElementUI* pElement = (CListLabelElementUI*)pFavoriteTree->GetSubItem(i);
		if( pElement == NULL)
			continue;

		TreeListUI::Node* pNode  = (TreeListUI::Node*)pElement->GetTag();
		if( pNode == itr->second)
		{
			pFavoriteTree->SelectItem(i);
			ShowFavoriteTreeList(nId);
			break;
		}
	}
}

void CMainFrameWnd::GetAvailableBrowser()
{
	plugin::PlugIn_GetAvailablePlugInsService	getAvailablePlugInsService;
	g_MainFrameModule->GetModuleManager()->CallService(getAvailablePlugInsService.serviceId, (param)&getAvailablePlugInsService);

	std::vector<IPlugIn*>* pvPlugIn = getAvailablePlugInsService.pvPlugIns;

	CTextUI* pFavoriteNumber = static_cast<CTextUI*>(m_pm.FindControl(_T("BrowserNum")));
	pFavoriteNumber->SetShowHtml();
	if (pFavoriteNumber)
	{
		TCHAR szFavoriteNum[MAX_PATH] = {0};
		_stprintf_s(szFavoriteNum, MAX_PATH - 1, _T("{f 3}{b}您安装了{f 4}{c #F27D30}%d{/c}{/f}种浏览器：{/b}{/f}"), pvPlugIn->size());
		pFavoriteNumber->SetText(szFavoriteNum);
	}

	for( int i=0; i<pvPlugIn->size(); i++)
	{
		IPlugIn* pPlugIn = (*pvPlugIn)[i];
		if( pPlugIn == NULL)
			continue;

		HICON hIcon = pPlugIn->GetBrowserIcon();
		const wchar_t* pPlugInName = pPlugIn->GetBrowserName();
		CIconBoxUI *pIconBox = (CIconBoxUI *)m_pm.FindControl(L"BrowserBox");
		if (pIconBox)
		{
			pIconBox->AddIcon(hIcon, pPlugInName, FALSE);
		}
	}
}
