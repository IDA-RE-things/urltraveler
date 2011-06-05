#include "stdafx.h"
#include "MainFrameWnd.h"
#include "MainFrameDefine.h"
#include "CurlHttp.h"
#include "MainFrameModule.h"
#include "TipWnd.h"
#include "ImageHelper.h"
#include "DatabaseDefine.h"
#include "MiscHelper.h"
#include "StringHelper.h"

using namespace database;
using namespace mainframe;

#define WM_TIPCLOSE    (WM_USER + 1)

CMainFrameWnd::CMainFrameWnd()
{
	m_pLoginFrame = NULL;

	m_nFavoriteNum = 0;
	m_pFavoriteData	= NULL;
	m_pTipWnd = NULL;
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

	TreeListUI* pFavoriteTree = static_cast<TreeListUI*>(m_pm.FindControl(_T("favoritelist")));

	wstring wstrText = L"{x 4}{x 4}";
	wstrText += L"收藏夹";

	TreeListUI::Node* pNode = pFavoriteTree->AddNode(wstrText.c_str());
	m_mapIdNode[0] = pNode;
	m_mapNodeId[pNode] = 0;

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
				pt.y -= msg.pSender->GetY();
			    SIZE sz = pFavoriteTree->GetExpanderSizeX(node);
			    if( pt.x >= sz.cx && pt.y < sz.cy )                     
					pFavoriteTree->SetChildVisible(node, !node->data()._child_visible);

				// 得到该结点对应的nId
				std::map<TreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(node);
				if( itr != m_mapNodeId.end())
				{
					int nId = itr->second;

					m_vFavoriteNode.clear();

					CListUI* pUserList = static_cast<CListUI*>(m_pm.FindControl(_T("favoritefilelist")));
					pUserList->RemoveAllItems();
					int j = 0;
					
					for( int i=0; i<m_nFavoriteNum; i++)
					{
						FAVORITELINEDATA* pData = &m_pFavoriteData[i];

						// 叶子结点
						if( pData->nPid == nId && pData->bFolder == false)
						{
							// 测试代码，此处从服务器拉取http://www.baidu.com/favicon.ico
							GetWebSiteFavIcon(pData->szUrl, j);

							CListTextElementUI* pListElement = new CListTextElementUI;
							pListElement->SetTag((UINT_PTR)pData);
							pUserList->Add(pListElement);
							m_vFavoriteNode.push_back(pData);
							j++;
						}
					}

					if( pUserList)
					{						
						pUserList->SetItemTextStyle(pUserList->GetItemTextStyle() & ~ DT_CENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE);
						pUserList->Invalidate();
					}
				}
			}
		}

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

				strTips += pData->szTitle;
				strTips += L"\n";
				strTips += L"<y 20><a>";
				strTips += pData->szUrl;
				strTips += L"</a>";
				m_pTipWnd->ShowTips(1000, strTips);
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
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
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
		if( m_vFavoriteNode.size() == 0)
			return L"";

		if( iIndex <= m_vFavoriteNode.size() -1)
		{
			if( iSubItem == 0 )
			{
				wstring strIconName;
				wchar_t szRow[5] = {0};
				_ltow(iIndex, szRow, 10);
				strIconName = L"<x 4><i favicon";
				strIconName += szRow;
				strIconName += L".ico><x 4>";

				strIconName += m_vFavoriteNode[iIndex]->szTitle;
				return _wcsdup(strIconName.c_str());
			}
			if( iSubItem == 1 )
			{
				wstring strUrl;
				strUrl += L"<x 4>";
				strUrl += m_vFavoriteNode[iIndex]->szUrl;

				return _wcsdup(strUrl.c_str());
			}
		}

    }

    return _T("");
}

bool CMainFrameWnd::GetWebSiteFavIcon(wstring strUrl, int nRow)
{
	wchar_t* pszDomainUrl = MiscHelper::GetDomainFromUrl(strUrl.c_str());
	if( pszDomainUrl == NULL)
		return false;

	wstring wstrDomainUrl = pszDomainUrl + wstring(L"/favicon.ico");

	// 向数据库查找Icon数据
	database::Database_GetFavoriteIconService getFavoriteService;
	STRNCPY(getFavoriteService.szFavoriteUrl, pszDomainUrl);
	g_MainFrameModule->GetModuleManager()->CallService(getFavoriteService.serviceId, (param)&getFavoriteService);

	wstring strRow = StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(nRow));
	wstring wstrIconName = wstring(L"favicon") + strRow + L".ico";
	m_pm.RemoveImage(wstrIconName.c_str());

	// 该icon数据库中已经存在
	bool bOk = false;
	if( getFavoriteService.hcon != NULL)
	{
		bOk = m_pm.AddIcon16(wstrIconName.c_str(), getFavoriteService.hcon) != NULL;
		free(pszDomainUrl);
		return true;
	}


	string strIconBuffer = CurlHttp::Instance()->RequestGet(wstrDomainUrl);
	int nSize = strIconBuffer.size();
	if (nSize != 0)
	{
		 HICON hIcon = ImageHelper::CreateIconFromBuffer((LPBYTE)strIconBuffer.c_str(), nSize, 16);
		 bOk = m_pm.AddIcon16(wstrIconName.c_str(), hIcon) != NULL;

		 if (bOk == false)
		 {
			 HICON hIcon16 = ImageHelper::Convert32x32IconTo16x16(
				 ImageHelper::CreateIconFromBuffer((LPBYTE)strIconBuffer.c_str(), nSize, 32));
			 bOk = m_pm.AddIcon16(wstrIconName.c_str(), hIcon16) != NULL;
		 }
	}

	database::Database_FavIconSaveEvent* pSaveIconEvent = new database::Database_FavIconSaveEvent();
	STRNCPY(pSaveIconEvent->szFavoriteUrl, pszDomainUrl);
	pSaveIconEvent->nIconDataLen = strIconBuffer.size();
	pSaveIconEvent->pIconData = new char[pSaveIconEvent->nIconDataLen];
	memcpy((void*)pSaveIconEvent->pIconData,strIconBuffer.c_str(), pSaveIconEvent->nIconDataLen);

	pSaveIconEvent->m_pstExtraInfo = pSaveIconEvent;
	g_MainFrameModule->GetModuleManager()->PushEvent(*pSaveIconEvent);

	free(pszDomainUrl);

	if (bOk == false)
	{
		unsigned char szDefaultIcon[] = {
			0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x10, 0x00, 
			0x01, 0x00, 0x04, 0x00, 0x28, 0x01, 0x00, 0x00, 0x16, 0x00, 
			0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 
			0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 
			0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 
			0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 
			0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 
			0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 
			0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 
			0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 
			0xCC, 0xCC, 0xCC, 0xCC, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
			0xFF, 0xFC, 0xCF, 0xF8, 0xCC, 0xC7, 0x7C, 0xCC, 0x8F, 0xFC, 
			0xCF, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCF, 0xFC, 0xCF, 0xFC, 
			0xCC, 0xCC, 0xCC, 0xCC, 0xCF, 0xFC, 0xCF, 0xF8, 0xCC, 0xCC, 
			0xCC, 0xCC, 0x8F, 0xFC, 0xCF, 0xFF, 0x8C, 0xCC, 0xCC, 0xC8, 
			0xFF, 0xFC, 0xCF, 0x8C, 0x88, 0xCC, 0xCC, 0x88, 0xC8, 0xFC, 
			0xCF, 0xCC, 0xCF, 0x8C, 0xC8, 0xFC, 0xCC, 0xFC, 0xCF, 0xCC, 
			0xCF, 0xFF, 0xFF, 0xF8, 0xC8, 0xFC, 0xCF, 0x8C, 0x88, 0xC8, 
			0xF8, 0xC8, 0xFF, 0xFC, 0xCF, 0xFF, 0xFC, 0xCC, 0xFC, 0xCC, 
			0xFF, 0xFC, 0xCF, 0xFF, 0xFC, 0xCC, 0xFC, 0xCC, 0xFF, 0xFC, 
			0xCF, 0xFF, 0xF8, 0xC8, 0xFF, 0x8F, 0xFF, 0xFC, 0xCF, 0xFF, 
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xCC, 0xCC, 0xCC, 0xCC, 
			0xCC, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};
		HICON hIcon = ImageHelper::CreateIconFromBuffer((LPBYTE)szDefaultIcon, sizeof(szDefaultIcon), 16);
		m_pm.AddIcon16(wstrIconName.c_str(), hIcon) != NULL;
	}

	return false;
}