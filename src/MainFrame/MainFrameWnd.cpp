#include "stdafx.h"
#include "MainFrameWnd.h"
#include "MainFrameDefine.h"
#include "MainFrameModule.h"
#include "PlugInDefine.h"
#include "DataCenterDefine.h"
#include "WebDefine.h"
#include "SettingDefine.h"
#include "LoginDefine.h"

#include "ImageHelper.h"
#include "MiscHelper.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "XString.h"

#include "CurlHttp.h"
#include "TipWnd.h"

#include "FavoriteListMenu.h"
#include "TreeListMenu.h"
#include "AddFavoriteWnd.h"
#include "FavoriteListMenu.h"
#include "AddFavoriteFoldNameWnd.h"

#include <algorithm>

#include "TestTreeWnd.h"

using namespace datacenter;
using namespace mainframe;
using namespace web;
using namespace setting;


CMainFrameWnd::CMainFrameWnd()
{
	m_pDragList = 0;
	m_pLoadingList = 0;
	m_nCurrentFavoriteFoldId = 0;
	m_vFavoriteNodeAtTreeNode.clear();

	m_pTipWnd = new CTipWnd();
	m_pFavTreeList = 0;
}

CMainFrameWnd::~CMainFrameWnd()
{
	if (m_pTipWnd)
	{
		m_pTipWnd->SendMessage(WM_CLOSE, 0, 0);
	}
}

void	CMainFrameWnd::ShowProcessLayout(BOOL bShow)
{
	if( bShow == TRUE)
	{
		CHorizontalLayoutUI*  pSearchLayout  = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"BrowserLayout"));
		if( pSearchLayout)
			pSearchLayout->SetVisible(false);

		CHorizontalLayoutUI*  pProcessLayout  = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"ProcessLayout"));
		if( pProcessLayout)
			pProcessLayout->SetVisible(true);

		CHorizontalLayoutUI*  pFavoriteLoadingLayout  = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"FavoriteLoadingLayout"));
		if( pFavoriteLoadingLayout)
		{
			pFavoriteLoadingLayout->SetVisible(true);
		}

		CHorizontalLayoutUI*  pFavoriteDataLayout  = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"FavoriteDataLayout"));
		if( pFavoriteDataLayout)
		{
			pFavoriteDataLayout->SetVisible(false);
		}

	}
	else
	{
		CHorizontalLayoutUI*  pSearchLayout  = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"BrowserLayout"));
		if( pSearchLayout)
			pSearchLayout->SetVisible(true);

		CHorizontalLayoutUI*  pProcessLayout  = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"ProcessLayout"));
		if( pProcessLayout)
			pProcessLayout->SetVisible(false);

		CHorizontalLayoutUI*  pFavoriteDataLayout  = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"FavoriteDataLayout"));
		if( pFavoriteDataLayout)
		{
			pFavoriteDataLayout->SetVisible(true);
		}

		CHorizontalLayoutUI*  pFavoriteLoadingLayout  = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"FavoriteLoadingLayout"));
		if( pFavoriteLoadingLayout)
			pFavoriteLoadingLayout->SetVisible(false);
	}
}

void	CMainFrameWnd::NotifyExportBegin(IPlugIn* pPlugIn)
{
	CListTextElementUI* pListElement = new CListTextElementUI;
	pListElement->SetFixedHeight(40);
	if( m_pLoadingList)
		m_pLoadingList->Add(pListElement);

	String	strIconName = String(pPlugIn->GetBrowserName()) + String(L".ico");
	m_pm.AddIcon32(strIconName, pPlugIn->GetBrowserIcon());
	strIconName = String(L"<x 4><i ") + strIconName;
	strIconName += String(L">");

	String	strBrowserName = L"<x 4><x 4>";
	strBrowserName +=	 pPlugIn->GetBrowserName();
	strBrowserName +=	 L"<x 4>正在导入收藏夹数据...";

	pListElement->SetText(0,strIconName.GetData());
	pListElement->SetText(1,strBrowserName.GetData());
}

void	CMainFrameWnd::NotifyExportEnd(IPlugIn* pPlugIn, int nFavoriteNum,  BOOL bSuccess)
{
	if( m_pLoadingList == NULL)
		return;

	CListTextElementUI* pListElement = (CListTextElementUI*)m_pLoadingList->GetItemAt(m_pLoadingList->GetRowCount() - 1);
	if( pListElement)
	{
		if( bSuccess == TRUE)
		{
			String	strBrowserName = L"<x 4><x 4>";
			strBrowserName +=	 pPlugIn->GetBrowserName();
			strBrowserName +=	 L"<x 4>导出成功";

			String	strFavoriteNum = L"<x 4><x 4>总共 ";
			strFavoriteNum += String::ValueOf(nFavoriteNum);
			strFavoriteNum += L" 记录";

			pListElement->SetText(1,strBrowserName.GetData());
			pListElement->SetText(2,strFavoriteNum.GetData());
		}
		else
		{
			String	strBrowserName = L"<x 4><x 4>";
			strBrowserName +=	 pPlugIn->GetBrowserName();
			strBrowserName +=	 L"<x 4>导出失败";

			String	strFavoriteNum = L"<x 4><x 4>总共 0 条记录";
			pListElement->SetText(1,strBrowserName.GetData());
			pListElement->SetText(2,strFavoriteNum.GetData());
		}
	}
}

void	CMainFrameWnd::NotifyInExportProcess(wchar_t* 	szProcess)
{
	CTextUI* pText = static_cast<CTextUI*>(m_pm.FindControl(L"ProcessHintText"));
	if( pText == NULL || szProcess == NULL)
		return;

	pText->SetText(szProcess);
}

void CMainFrameWnd::OnPrepare(TNotifyUI& msg) 
{ 
	m_pTipWnd->Init(msg.pSender); 

	m_pDragList = 0;
	m_pDragList = static_cast<CDragListUI*>(m_pm.FindControl(_T("favoritefilelist")));
	if( m_pDragList == NULL)
	{			
		return;
	}
	m_pDragList->SetTextCallback(this);
	m_pDragList->SetItemTextStyle(m_pDragList->GetItemTextStyle()
		& ~ DT_CENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE);

	m_pFavTreeList = 0;
	m_pFavTreeList = static_cast<CFavoriteTreeListUI*>(m_pm.FindControl(_T("favoritetreelist")));
	if( m_pFavTreeList == NULL)
	{
		ASSERT(0);
		return;
	}

	m_pLoadingList = static_cast<CListUI*>(m_pm.FindControl(_T("FavoriteLoadingList")));
	if( m_pLoadingList == NULL)
	{			
		return;
	}
	m_pLoadingList->SetItemTextStyle(m_pLoadingList->GetItemTextStyle() 
		& ~ DT_CENTER | DT_LEFT | DT_END_ELLIPSIS | DT_SINGLELINE);
	
	// 发送请求至服务器，统计在线人数
	Web_OpenTravelerService openTravelerService;
	openTravelerService.srcMId = MODULE_ID_MAINFRAME;
	g_MainFrameModule->GetModuleManager()->CallService(openTravelerService.serviceId,(param)&openTravelerService);

	ShowProcessLayout(TRUE);
}

void CMainFrameWnd::LoadFavoriteTree(FAVORITELINEDATA** ppFavoriteData, int nNum)
{	 
	wstring wstrText = L"{x 4}";
	wstrText += L"收藏夹";

	if( m_pFavTreeList == NULL)
		m_pFavTreeList = static_cast<CFavoriteTreeListUI*>(m_pm.FindControl(_T("favoritetreelist")));
	if( m_pFavTreeList == NULL)
	{
		ASSERT(0);
		return;
	}

	if( m_pDragList == NULL)
		m_pDragList = static_cast<CDragListUI*>(m_pm.FindControl(_T("favoritefilelist")));
	if( m_pDragList != NULL)
	{
		m_pDragList->SetTextCallback(this);     
	}

/*
	CTreeListUI::Node* pRootNode = m_pFavTreeList->AddNode(wstrText.c_str());
	m_pFavTreeList->m_mapIdNode[0] = pRootNode;
	m_pFavTreeList->m_mapNodeId[pRootNode] = 0;
*/

	for( int i=0; i<nNum; i++)
	{
		PFAVORITELINEDATA pData = ppFavoriteData[i];
		if( pData != NULL && pData->bFolder == true)
		{
			// 找到当前结点的父节点
			int nPid = pData->nPid;
			std::map<int, CFavoriteTreeListUI::Node*>::iterator itr = m_pFavTreeList->m_mapIdNode.find(nPid);
			if( itr != m_pFavTreeList->m_mapIdNode.end())
			{
				CFavoriteTreeListUI::Node* pParentNode = itr->second;
				if( pParentNode != NULL)
				{
					wstring wstrText = L"{x 4}";
					wstrText += pData->szTitle;

					CFavoriteTreeListUI::Node* pNode  = m_pFavTreeList->AddNode(wstrText.c_str(), pParentNode);
					m_pFavTreeList->m_mapIdNode[pData->nId] = pNode;
					m_pFavTreeList->m_mapNodeId[pNode] = pData->nId;
					m_pFavTreeList->Invalidate();
				}
			}
			else
			{
				wstring wstrText = L"{x 4}";
				wstrText += pData->szTitle;

				CFavoriteTreeListUI::Node* pNode  = m_pFavTreeList->AddNode(wstrText.c_str());
				m_pFavTreeList->m_mapIdNode[pData->nId] = pNode;
				m_pFavTreeList->m_mapNodeId[pNode] = pData->nId;
				m_pFavTreeList->Invalidate();
			}
		}
	}

	//m_pFavTreeList->SetChildVisible(pRootNode, true);

	// 显示根结点下的所有的收藏夹记录
	//m_pFavTreeList->m_nTreeNodeId = 0;
	//m_pFavTreeList->m_pCurrentTreeNode	=	pRootNode;
	ShowFavoriteTreeList(0);
}

void	CMainFrameWnd::ShowFavoriteTreeList(int nId)
{
	if( m_pDragList == NULL)
		return;

	m_nCurrentFavoriteFoldId = nId;
	m_vFavoriteNodeAtTreeNode.clear();

	m_pDragList->RemoveAllItems();

	int nFavoriteNum = 0;
	PFAVORITELINEDATA*	ppFavoriteData = GetFavoriteLineData(nFavoriteNum);

	int j = 0;
	for( int i=0; i<nFavoriteNum; i++)
	{
		FAVORITELINEDATA* pData = ppFavoriteData[i];

		// 叶子结点
		if( pData->nPid == nId && pData->bFolder == false && pData->bDelete == false)
		{
			GetWebSiteFavIcon(pData->szUrl, j);

			CListTextEditElementUI* pListElement = new CListTextEditElementUI;
			pListElement->SetTag((UINT_PTR)pData);
			m_pDragList->Add(pListElement);
			pListElement->SetColomnEditable(1, TRUE);
			pListElement->SetColomnEditable(2, TRUE);

			m_vFavoriteNodeAtTreeNode.push_back(pData);
			j++;
		}
	}

	SetFavoriteNumText(j);
}

void	CMainFrameWnd::OnTabChange(TNotifyUI& msg)
{
	CStdString name = msg.pSender->GetName();
	CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
	if(name==_T("FavTravelerTab"))
		pControl->SelectItem(0);
	else if(name==_T("FavSyncTab"))
		pControl->SelectItem(1);
	else if(name==_T("CookieSyncTab"))
		pControl->SelectItem(2);
}

void	CMainFrameWnd::OnSystemMenu(TNotifyUI& msg)
{
	HMENU	hPopMenu;
	hPopMenu = ::LoadMenuW(g_hModule, MAKEINTRESOURCE(IDR_MENU1)); 
	hPopMenu = ::GetSubMenu(hPopMenu, 0);

	POINT pt;
	GetCursorPos(&pt);
	::TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x - 10,pt.y + 17,0, this->m_hWnd, NULL);
}

void	CMainFrameWnd::OnBtnClose(TNotifyUI& msg)
{
	// 检查是否要最小化
	setting::Setting_GetExitWhenCloseWndService service;
	g_MainFrameModule->CallDirect(service.serviceId, (param)&service);
	if( service.bExit == TRUE)
	{
		// 发送请求至服务器，统计在线人数
		CurlHttp* pHttp  = CurlHttp::Instance();
		string strData = pHttp->RequestGet(L"http://1.urltraveler.sinaapp.com/logout.php");

		PostQuitMessage(0);
	}
	else
	{
		::ShowWindow(GetHWND(), SW_HIDE);
	}
}

void	CMainFrameWnd::SetFavoriteNumText(int nNum)
{
	CTextUI* pFavoriteNumber = static_cast<CTextUI*>(m_pm.FindControl(_T("FavoriteNum")));
	pFavoriteNumber->SetShowHtml();
	if (pFavoriteNumber)
	{
		TCHAR szFavoriteNum[MAX_PATH] = {0};
		_stprintf_s(szFavoriteNum, MAX_PATH - 1, _T("该文件夹下共有 {b}{c #FF0000}%d{/c}{/b} 个收藏"), nNum);
		pFavoriteNumber->SetText(szFavoriteNum);
	}
}

void	CMainFrameWnd::OnFavoriteTreeListItemClick(TNotifyUI& msg)
{
	int nIndex = msg.wParam;

	// 点击收藏夹目录的响应代码
	CListLabelElementUI* pTreeListUIElement = (CListLabelElementUI*)m_pFavTreeList->GetItemAt(nIndex);
	if( pTreeListUIElement != NULL )
	{
		CFavoriteTreeListUI::Node* node = (CFavoriteTreeListUI::Node*)pTreeListUIElement->GetTag();

		// 得到该结点对应的nId
		std::map<CFavoriteTreeListUI::Node*, int>::iterator itr = m_pFavTreeList->m_mapNodeId.find(node);
		if( itr != m_pFavTreeList->m_mapNodeId.end())
		{
			int nId = itr->second;
			m_pFavTreeList->m_nTreeNodeId = nId;
			m_pFavTreeList->m_pCurrentTreeNode	=	itr->first;
			ShowFavoriteTreeList(nId);
		}
	}

	if (m_pTipWnd)
	{
		m_pTipWnd->HideTip();
	}
}

void	CMainFrameWnd::OnFavoriteListItemHover(TNotifyUI& msg)
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

void	CMainFrameWnd::OnFavoriteListItemUnHover(TNotifyUI& msg)
{
	if (m_pTipWnd)
	{
		m_pTipWnd->HideTip();
	}
}

void	CMainFrameWnd::OnShowMenu(TNotifyUI& msg)
{
	if( msg.pSender->GetName() == _T("favoritefilelist") ) 
	{
		CFavoriteListMenu* pMenu = new CFavoriteListMenu();
		pMenu->CreatePopupMenu();
		if( pMenu == NULL ) { return; }

		POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
		::ClientToScreen(*this, &pt);

		int nSelNum = 0;
		int* pSelIndex =	  ((CListUI *)msg.pSender)->GetCurSel(nSelNum);
		pMenu->Init(nSelNum);

		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x,
			pt.y,this->GetHWND());
	}
	else 	if( msg.pSender->GetName() == _T("favoritetreelist") ) 
	{
		CTreeListMenu* pMenu = new CTreeListMenu();
		pMenu->CreatePopupMenu();
		if( pMenu == NULL ) { return; }
		POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
		::ClientToScreen(*this, &pt);

		CFavoriteTreeListUI* pTree = (CFavoriteTreeListUI*)msg.pSender;
		int nSelIndex = pTree->GetCurSel();
		if( nSelIndex == -1)
			pMenu->Init(false, nSelIndex);
		else
			pMenu->Init(true, nSelIndex);

		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x,
			pt.y,this->GetHWND());
	}
}

void	CMainFrameWnd::OnFavoriteListItemEditFinished(TNotifyUI& msg)
{
	CControlUI *pControl = msg.pSender;
	if (_tcscmp(pControl->GetName(), _T("favoritefilelist")) == 0)
	{
		CListUI *pListUI = (CListUI *)pControl;

		SHORT nRow = HIWORD(msg.wParam);
		SHORT nColomn = LOWORD(msg.wParam);

		int nTotalRow = pListUI->GetRowCount();
		if( nRow < 0 || nTotalRow == 0 || nRow >nTotalRow - 1)
			return;

		FAVORITELINEDATA* pData = m_vFavoriteNodeAtTreeNode[nRow];
		if( pData == NULL)
			return;

		if( nColomn == 1)
		{
			LPCTSTR	szTitle = pListUI->GetEditText().GetData();
			String	strTitle = (szTitle == NULL ? L"" : szTitle);

			if( strTitle == L"")
			{
				::MessageBox(NULL, L"网址标题不能为空", L" 警告", MB_OK);
				pListUI->ShowEdit(nRow, 1);
				return;
			}

			STRNCPY(pData->szTitle, pListUI->GetEditText());
			pListUI->Invalidate();

			return;
		}
		else if( nColomn == 2)
		{
			String strUrl = pListUI->GetEditText();
			String strPrefix = strUrl.Left(5);
			if( strPrefix == L"<x 4>")
				strUrl = strUrl.SubStr(5,strUrl.GetLength()-5);
			if( strUrl == L"")
			{
				::MessageBox(NULL, L"网址不能为空", L" 警告", MB_OK);
				pListUI->ShowEdit(nRow, 2);
				return;
			}

			if( strUrl.Left(7) != L"http://" && strUrl.Left(8) != L"https://")
			{
				strUrl = String(L"http://") + strUrl;
			}
			STRNCPY(pData->szUrl, strUrl.GetData());
			GetWebSiteFavIcon(pData->szUrl, nRow);
		}
		pListUI->Invalidate();
	}
}

void	CMainFrameWnd::OnFavoriteListItemDelete(TNotifyUI& msg)
{
	CControlUI *pControl = msg.pSender;
	if (_tcscmp(pControl->GetName(), _T("favoritefilelist")) == 0)
	{
		CListUI *pList = (CListUI *)pControl;

		int nRow = msg.wParam;
		if( nRow < 0 || nRow > pList->GetRowCount() - 1)
			return;

		FAVORITELINEDATA *pSelNode = (FAVORITELINEDATA *)m_vFavoriteNodeAtTreeNode[nRow];
		pSelNode->bDelete = true;

		// 从数据中心中删除该Item
		MainFrame_DeleteFavoriteEvent* pEvent = new MainFrame_DeleteFavoriteEvent();
		pEvent->desMId = MODULE_ID_MAINFRAME;
		pEvent->nDeleteNodeId = pSelNode->nId;
		g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
	}
}

void	CMainFrameWnd::OnFavoriteListItemMoved(TNotifyUI& msg)
{
	if( m_pDragList == NULL)
		return;

	int nId = msg.wParam;

	for( size_t i=0; i<m_vFavoriteNodeAtTreeNode.size(); i++)
	{
		FAVORITELINEDATA *pSelNode = (FAVORITELINEDATA *)m_vFavoriteNodeAtTreeNode[i];
		if( pSelNode->nId == nId)
		{
			std::vector<FAVORITELINEDATA*>::iterator itr = std::find(m_vFavoriteNodeAtTreeNode.begin(),
				m_vFavoriteNodeAtTreeNode.end(), pSelNode);
			if( itr != m_vFavoriteNodeAtTreeNode.end())
				m_vFavoriteNodeAtTreeNode.erase(itr);

			break;
		}
	}

	SetFavoriteNumText(m_vFavoriteNodeAtTreeNode.size());
}

void CMainFrameWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) 
	{
		OnPrepare(msg);
	}
	else if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == L"closebtn" ) 
		{
			OnBtnClose(msg);
			return; 
		}
		else if( msg.pSender->GetName() == L"minbtn" ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
			return; 
		}
		else if( msg.pSender->GetName() == L"maxbtn" ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
			return; 
		}
		else if( msg.pSender->GetName() == L"restorebtn" ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
			return; 
		}
		else if( msg.pSender->GetName() == L"menubtn" ) 
		{ 
			OnSystemMenu(msg);
			return;
		}
		else if( msg.pSender->GetName() == L"SettingTab" ) 
		{
			setting::Setting_OpenEvent* pEvent = new setting::Setting_OpenEvent();
			pEvent->srcMId = MODULE_ID_MAINFRAME;
			g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
		}
		else if( msg.pSender->GetName() == L"BeginToCombineBtn" ) 
		{
			
			CHorizontalLayoutUI* pLoadingLayout = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("FavoriteLoadingLayout")));
			CHorizontalLayoutUI* pDataLayout = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("FavoriteDataLayout")));
			if( pDataLayout->IsVisible())
			{
				pLoadingLayout->SetVisible(true);
				pDataLayout->SetVisible(false);
			}
			else
			{
				pLoadingLayout->SetVisible(false);	
				pDataLayout->SetVisible(true);	
			}
		}

		
	}
	else if(msg.sType==_T("setfocus"))
	{
		OnTabChange(msg);
		return;
	}
	else if( msg.sType == _T("treelistitemclick") ) 
	{
 		if( msg.pSender->GetName() == L"favoritetreelist" ) 
		{ 
			OnFavoriteTreeListItemClick(msg);
			return;
		}

		return;
	}
	else if( msg.sType == _T("itemclick") ) 
	{
 		if( msg.pSender->GetName() == L"favoritetreelist" ) 
		{ 
			OnFavoriteTreeListItemClick(msg);
			return;
		}

		return;
	}
	else if(msg.sType == L"itemhot")
	{
		if (msg.pSender->GetParent()->GetParent()->GetName() == L"favoritefilelist")
		{
			OnFavoriteListItemHover(msg);
			return;
		}
	}
	else if(msg.sType == L"itemunhot")
	{
		if (msg.pSender->GetParent()->GetParent()->GetName() == L"favoritefilelist")
		{
			OnFavoriteListItemUnHover(msg);
			return;
		}
	}
	else if(msg.sType == L"menu") 
	{
		OnShowMenu(msg);
		return;
	}
	else if(msg.sType == L"favlisteditfinish")
	{
		OnFavoriteListItemEditFinished(msg);
		return;
	}
	else if(msg.sType == L"favlistitemdelete")
	{
		OnFavoriteListItemDelete(msg);
		return;
	}
	else if(msg.sType == L"favlistitemmoved")
	{
		OnFavoriteListItemMoved(msg);
		return;
	}
}

LRESULT CMainFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	// 设置任务栏图标
	HICON hSmallIcon = ::LoadIconW(g_hModule,(LPCTSTR)IDI_APPICON16);
	HICON hBigIcon = ::LoadIconW(g_hModule,(LPCTSTR)IDI_APPICON32);

	SendMessage(WM_SETICON,ICON_SMALL,(LPARAM)hSmallIcon);
	SendMessage(WM_SETICON,ICON_BIG,(LPARAM)hBigIcon); 

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

LRESULT CMainFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	if( ::IsZoomed(*this) != bZoomed ) 
	{
		if( !bZoomed ) 
		{
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
			if( m_pDragList)
			{
				CListHeaderUI* pHeader = m_pDragList->GetHeader();
				RECT rc0 = pHeader->GetItemAt(0)->GetPos();
				rc0.right += 200;
				pHeader->GetItemAt(0)->SetPos(rc0);

				RECT rc1 = pHeader->GetItemAt(1)->GetPos();
				rc1.right -= 200;
				pHeader->GetItemAt(1)->SetPos(rc1);
				m_pDragList->Invalidate();
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

void CMainFrameWnd::OnFavListAdd()
{
	g_MainFrameModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_MAINFRAME_ADD_URL, MODULE_ID_MAINFRAME));
}

void	CMainFrameWnd::OnFavListDelete()
{
	int nSel = m_pDragList->GetCurSel();
	if( nSel < 0 ) return;

	CListElementUI* pElement =		 (CListElementUI*)m_pDragList->GetSubItem(nSel);
	if( pElement == NULL)
	return;

	FAVORITELINEDATA *pSelNode = (FAVORITELINEDATA *)(pElement->GetTag());
	pSelNode->bDelete = true;
	m_pDragList->RemoveAt(nSel);

	// 从数据中心中删除该Item
	MainFrame_DeleteFavoriteEvent* pEvent = new MainFrame_DeleteFavoriteEvent();
	pEvent->desMId = MODULE_ID_MAINFRAME;
	pEvent->nDeleteNodeId = pSelNode->nId;
	g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
}

void	CMainFrameWnd::OnFavListOpen()
{
	int nSel = m_pDragList->GetCurSel();
	if( nSel < 0 ) return;

	// 通知打开URL
	g_MainFrameModule->GetModuleManager()->PushEvent(
	MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_MAINFRAME_OPEN_URL, MODULE_ID_MAINFRAME, nSel));
}

void	CMainFrameWnd::OnFavListCopyUrl()
{
	int nSel = m_pDragList->GetCurSel();
	if( nSel < 0 ) return;

	// 通知打开URL
	g_MainFrameModule->GetModuleManager()->PushEvent(
	MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_MAINFRAME_COPY_URL, MODULE_ID_MAINFRAME, nSel));
}

void CMainFrameWnd::OnFavListEdit()
{
	int nSelRow = m_pDragList->GetCurSel();
	if( nSelRow != -1)
	{
		m_pDragList->ShowEdit(nSelRow, 1);
	}
}

void	CMainFrameWnd::OnFavListSelectAll()
{
	for( int i=0; i<m_pDragList->GetRowCount(); i++)
		m_pDragList->SelectItem(i);
}

void CMainFrameWnd::OnTreeListNew()
{
	// 找到当前选中的item
	int nCurSel = m_pFavTreeList->GetCurSel();
	if( nCurSel == -1)
		return;

	CListLabelElementUI* pElement =		 (CListLabelElementUI*)m_pFavTreeList->GetItemAt(nCurSel);
	if( pElement == NULL)
		return;

	CFavoriteTreeListUI::Node* pNode  = (CFavoriteTreeListUI::Node*)pElement->GetTag();
	if( pNode == NULL)
		return;

	int nParentId = m_pFavTreeList->GetIdFromNode(pNode);
	if( nParentId == -1)
		return;

	CAddFavoriteFoldNameWnd* pWnd = new CAddFavoriteFoldNameWnd();
	if( pWnd == NULL ) { Close(); return;  }
	pWnd->m_nParentId = nParentId;
	pWnd->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
	pWnd->CenterWindow();
	pWnd->ShowModal();
}

void CMainFrameWnd::OnTreeListDelete()
{
	if( m_pFavTreeList)
	{
		int nIndex = m_pFavTreeList->GetCurSel();
		if( nIndex < 0)
			return;

		// 通知数据中心删除
		MainFrame_DeleteFavoriteFoldEvent* pEvent = new MainFrame_DeleteFavoriteFoldEvent();
		pEvent->desMId = MODULE_ID_MAINFRAME;
		pEvent->nDeleteIndex = nIndex;
		g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
	}
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
			g_MainFrameModule->GetModuleManager()->PushEvent(
				MakeEvent<MODULE_ID_MAINFRAME>()(login::EVENT_VALUE_LOGIN_OPEN, MODULE_ID_LOGIN));
		}

		break;

	case IDM_ABOUT:
		{
			CTestTreeWnd* pTestWnd = new CTestTreeWnd();
			pTestWnd->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
			pTestWnd->CenterWindow();
			pTestWnd->ShowModal();
		}
		break;

	case IDM_SETTING:
		{
			setting::Setting_OpenEvent* pEvent = new setting::Setting_OpenEvent();
			pEvent->srcMId = MODULE_ID_MAINFRAME;
			g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
		}
		break;

	// 列表菜单项
	case IDM_FAVLIST_ADD:
		OnFavListAdd();
		break;

	case IDM_FAVLIST_DELETE:
		OnFavListDelete();
		break;

	case IDM_FAVLIST_COPYURL:
		OnFavListCopyUrl();
		break;

	case IDM_FAVLIST_EDIT:
		OnFavListEdit();
		break;

	case IDM_FAVLIST_SHARE:
		break;

	case IDM_FAVLIST_SELECTALL:
		OnFavListSelectAll();
		break;

	case IDM_FAVLIST_OPEN:
		OnFavListOpen();
		break;

	case IDM_TREELIST_ADD:
		OnTreeListNew();
		break;

	case IDM_TREELIST_DELETE:
		OnTreeListDelete();
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
	switch( uMsg ) 
	{
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

	if( m_pDragList == NULL)
		return;

	m_pDragList->Invalidate();
}

void	CMainFrameWnd::DeleteFavorite(int nDeleteNodeId)
{
	size_t i = 0;
	for( i=0; i<m_vFavoriteNodeAtTreeNode.size(); i++)
	{
		FAVORITELINEDATA* pData = m_vFavoriteNodeAtTreeNode[i];
		if( pData->nId == nDeleteNodeId)
		{
			m_vFavoriteNodeAtTreeNode.erase(m_vFavoriteNodeAtTreeNode.begin() + i);
			//目前只支持单删除
			break;
		}
	}

	if( m_pDragList == NULL)
		return;
	m_pDragList->Invalidate();

	SetFavoriteNumText(m_vFavoriteNodeAtTreeNode.size());
}

void	CMainFrameWnd::DeleteFavoriteFold(int nIndex)
{
	if( nIndex == 0)
		return;

	if( m_pFavTreeList == NULL)
	{
		ASSERT(0);
		return;
	}

	m_pFavTreeList->RemoveAt(nIndex);
}

void CMainFrameWnd::AddUrl()
{
	CAddFavoriteWnd* pAddFavoriteWnd = new CAddFavoriteWnd();
	if( pAddFavoriteWnd == NULL ) { Close(); return;  }
	pAddFavoriteWnd->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
	pAddFavoriteWnd->CenterWindow();
	pAddFavoriteWnd->m_nFavoriteId = m_nCurrentFavoriteFoldId;
	pAddFavoriteWnd->ShowModal();
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
		MessageBox(NULL,L"URL地址已经成功复制到剪贴板", L"复制成功", MB_OK);
	}
}

void	CMainFrameWnd::SelectTreeList(int nId)
{
	if( m_pFavTreeList == NULL)
	{
		ASSERT(0);
		return;
	}

	if( nId == 0)
	{
		m_pFavTreeList->SelectItem(0);
		ShowFavoriteTreeList(0);
		return;
	}

	std::map<int, CFavoriteTreeListUI::Node*>::iterator itr = m_pFavTreeList->m_mapIdNode.find(nId);
	if( itr == m_pFavTreeList->m_mapIdNode.end())
		return;

	if( itr->second == NULL)
		return;

	int nCount = m_pFavTreeList->GetRowCount();
	for( int i =0; i<nCount;i++)
	{
		CListLabelElementUI* pElement = (CListLabelElementUI*)m_pFavTreeList->GetSubItem(i);
		if( pElement == NULL)
			continue;

		CFavoriteTreeListUI::Node* pNode  = (CFavoriteTreeListUI::Node*)pElement->GetTag();
		if( pNode == itr->second)
		{
			m_pFavTreeList->ClearSelectedItem();
			m_pFavTreeList->SelectItem(i);
			ShowFavoriteTreeList(nId);
			break;
		}
	}
}

void	CMainFrameWnd::AddUrlSuccess(PFAVORITELINEDATA pData)
{
	if(m_pDragList)
	{
		CListTextEditElementUI* pListElement = new CListTextEditElementUI;
		pListElement->SetTag((UINT_PTR)pData);
		pListElement->SetColomnEditable(1, TRUE);
		pListElement->SetColomnEditable(2, TRUE);
		m_pDragList->Add(pListElement);

		m_vFavoriteNodeAtTreeNode.insert(m_vFavoriteNodeAtTreeNode.begin(),1,pData);

		GetWebSiteFavIcon(pData->szUrl, 0);

		m_pDragList->Invalidate();
	}

	int nFavoriteNum = m_vFavoriteNodeAtTreeNode.size();
	SetFavoriteNumText(nFavoriteNum);
}

void	CMainFrameWnd::AddFavoriteFoldSuccess(int nParentId, PFAVORITELINEDATA pData)
{
	if( m_pFavTreeList)
	{
		String strFavFoldName =  L"<x 4><x 4>";
		strFavFoldName += pData->szTitle;

		int nIndex =  m_pFavTreeList->GetIndexFromId(nParentId);
		if( nIndex != -1)
			m_pFavTreeList->Add(nIndex, pData->nId,strFavFoldName.GetData());
	}
}

PFAVORITELINEDATA*	CMainFrameWnd::GetFavoriteLineData(int& nFavoriteNum)
{
	DataCenter_GetFavoriteService favoriteData;
	g_MainFrameModule->GetModuleManager()->CallService(SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA,
		(param)&favoriteData); 

	nFavoriteNum = favoriteData.nNum;
	return favoriteData.ppFavoriteData;
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
		_stprintf_s(szFavoriteNum, MAX_PATH - 1, _T("{f 微软雅黑 15}{b}您安装了 {c #F27D30}%d{/c} 种浏览器：{/b}{/f}"), pvPlugIn->size());
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
