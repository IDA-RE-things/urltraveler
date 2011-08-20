#include <stdafx.h>
#include "FavTreeList.h"
#include "resource.h"
#include "DataCenterDefine.h"
#include "MainFrameModule.h"
#include "time.h"
#include "MainFrameDefine.h"
#include "DragList.h"

using namespace datacenter;
using namespace mainframe;

CFavoriteTreeListUI::CFavoriteTreeListUI() 
{
	m_nTreeNodeId = -1;

	m_bIsDragging = false;
	m_mapIdNode.clear();
	m_mapNodeId.clear();

	m_pCurrentTreeNode	=	NULL;
}

LPCTSTR CFavoriteTreeListUI::GetClass() const
{
	return _T("FavTreeListUI");
}

void CFavoriteTreeListUI::Notify(TNotifyUI& msg)
{
}

int CFavoriteTreeListUI::GetIndexFromId(int nId)
{
	// 根据nId找到对应的Node
	std::map<int, CTreeListUI::Node*>::iterator itr  = m_mapIdNode.find(nId);
	if( itr != m_mapIdNode.end())
	{
		CTreeListUI::Node* pNode = itr->second;
		if( pNode)
		{
			CListLabelElementUI* pElement = pNode->data()._pListElement;
			if( pElement == NULL)
				return -1;

			return pElement->GetIndex();
		}

		return -1;
	}

	return -1;
}

int CFavoriteTreeListUI::GetIdFromIndex(int nIndex)
{
	CListLabelElementUI* pElement = (CListLabelElementUI*)GetSubItem(nIndex);
	if( pElement == NULL)
		return -1;

	CTreeListUI::Node* pNode  = (CTreeListUI::Node*)pElement->GetTag();
	if( pNode == NULL)
		return -1;

	std::map<CTreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(pNode);
	if( itr == m_mapNodeId.end())
		return -1;

	// 得到了该结点的ID
	int nParentId = itr->second;
	return nParentId;
}

int CFavoriteTreeListUI::GetIdFromNode(CTreeListUI::Node* pNode)
{
	if( pNode == NULL)
		return -1;

	std::map<CTreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(pNode);
	if( itr == m_mapNodeId.end())
		return -1;

	return itr->second;
}

CTreeListUI::Node* CFavoriteTreeListUI::GetNodeFromId(int nId)
{
	std::map<int, CTreeListUI::Node*>::iterator itr  = m_mapIdNode.find(nId);
	if( itr != m_mapIdNode.end())
	{
		CTreeListUI::Node* pNode = itr->second;
		return pNode;
	}

	return NULL;
}

bool	CFavoriteTreeListUI::Add(int nIndex, int nId,LPCTSTR pszTitle)
{
	if( pszTitle == NULL)
		return false;

	CListLabelElementUI* pElement = (CListLabelElementUI*)GetSubItem(nIndex);
	if( pElement == NULL)
		return false;

	CTreeListUI::Node* pNode  = (CTreeListUI::Node*)pElement->GetTag();
	std::map<CTreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(pNode);
	if( itr == m_mapNodeId.end())
		return false;

	// 得到了该结点的ID
	int nParentId = itr->second;
	CTreeListUI::Node* pChildNode  = AddNode(pszTitle, pNode);
	m_mapIdNode[nId] = pChildNode;
	m_mapNodeId[pChildNode] = nId;
}

bool CFavoriteTreeListUI::RemoveAt(int nIndex)
{
	CControlUI* pControl = m_pList->GetItemAt(nIndex);
	if( !pControl ) 
		return false;

	CListLabelElementUI* pElement = (CListLabelElementUI*)GetSubItem(nIndex);
	if( pElement == NULL)
		return false;

	CTreeListUI::Node* pNode  = (CTreeListUI::Node*)pElement->GetTag();
	std::map<CTreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(pNode);
	if( itr == m_mapNodeId.end())
		return false;

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

				std::map<int, CTreeListUI::Node*>::iterator itr  = m_mapIdNode.find(nSubFolderId);
				if( itr != m_mapIdNode.end())
				{
					CTreeListUI::Node* pNode = itr->second;
					if( pNode)
						m_mapNodeId.erase(pNode);
				}
				m_mapIdNode.erase(nSubFolderId);
			}

			RemoveNode(pNode);
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

			std::map<int, CTreeListUI::Node*>::iterator itr  = m_mapIdNode.find(nSubFolderId);
			if( itr != m_mapIdNode.end())
			{
				CTreeListUI::Node* pNode = itr->second;
				if( pNode)
					m_mapNodeId.erase(pNode);
			}

			m_mapIdNode.erase(nSubFolderId);
		}

		RemoveNode(pNode);
		m_mapNodeId.erase(pNode);
		m_mapIdNode.erase(nId);

		g_MainFrameModule->GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_DATACENTER_DELETE_FAVORITE_FOLD,
			MODULE_ID_DATACENTER,
			nId));	
	}

	for(size_t i =0; i< m_vCurSel.size(); i++)
	{
		if( m_vCurSel[i] == nIndex)
		{
			m_vCurSel.erase(m_vCurSel.begin() + i);
			break;
		}
	}

	ClearSelectedItem();
	return true;
}

void	CFavoriteTreeListUI::OnEventItemClick(TEventUI& event)
{
	if( event.pSender == NULL)
		return;

	CTreeListUI::Node* pNode = (CTreeListUI::Node*)event.pSender->GetTag();
	if( pNode == NULL)
		return;

	POINT pt = { 0 };
	::GetCursorPos(&pt);
	::ScreenToClient(m_pManager->GetPaintWindow(), &pt);
	pt.x -= event.pSender->GetX();
	pt.y -= event.pSender->GetY();

	SIZE sz = GetExpanderSizeX(pNode);
	if( pt.x >= sz.cx && pt.y < sz.cy )                     
		SetChildVisible(pNode, !pNode->data()._child_visible);
}

void	CFavoriteTreeListUI::OnEventDragOver(TEventUI& event)
{
	if( m_bIsDragging == false)
	{
		m_bIsDragging = true;
	}
}

void	CFavoriteTreeListUI::OnEventTreeListItemDragEnd()
{
	// 获取到鼠标所在点的位置
	HCURSOR   hCur   =   ::LoadCursor(NULL,IDC_ARROW); 
	::SetCursor(hCur);

	int nHotIndex = GetHotItem();
	CListLabelElementUI* pDstItem =  (CListLabelElementUI*)GetItemAt(nHotIndex);
	if( pDstItem == NULL)
		return;

	CTreeListUI::Node* pDstNode = (CTreeListUI::Node*)pDstItem->GetTag();
	if( pDstNode == NULL)
	{
		return;
	}

	std::map<CTreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(pDstNode);
	if( itr == m_mapNodeId.end())
	{
		return;
	}

	int nDstId = itr->second;

	// 将当前需要拖放的收藏夹作为目标的子收藏夹
	int srcIndex = m_iLastClickSel;
	int dstIndex = nHotIndex;

	// 被拖动的结点
	CListLabelElementUI* pSrcItem =  (CListLabelElementUI*)GetItemAt(srcIndex);
	if( pSrcItem == NULL)
	{
		return;
	}

	CTreeListUI::Node* pSrcNode = (CTreeListUI::Node*)pSrcItem->GetTag();
	if( pSrcNode == NULL)
	{
		ASSERT(0);
		return;
	}

	itr = m_mapNodeId.find(pSrcNode);
	if( itr == m_mapNodeId.end())
	{
		ASSERT(0);
		return;
	}
	int nSrctId = itr->second;

	if( nDstId == nSrctId)
		return;

	SetChildVisible(pDstNode, true);

	DataCenter_GetFavoriteService favoriteData;
	g_MainFrameModule->GetModuleManager()->CallService(SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA,
		(param)&favoriteData); 

	int nFavoriteNum = favoriteData.nNum;
	PPFAVORITELINEDATA ppFavoriteData = favoriteData.ppFavoriteData;

	PFAVORITELINEDATA pSrcLineData = NULL;
	for( int i=0; i<nFavoriteNum; i++)
	{
		if( ppFavoriteData[i]->nId == nSrctId)
		{
			pSrcLineData = ppFavoriteData[i];
			break;
		}
	}

	if( pSrcLineData == NULL)
		return;

	pSrcLineData->nPid = nDstId;

	// 删除原有结点
	RemoveNode(pSrcNode);

	// 在结点中增加一个新结点
	wstring wstrText = L"{x 4}";
	wstrText += pSrcLineData->szTitle;
	CTreeListUI::Node* pNode  = AddNode(wstrText.c_str(), pDstNode);
	m_mapIdNode[pSrcLineData->nId] = pNode;
	m_mapNodeId[pNode] = pSrcLineData->nId;
	pSrcLineData->nLastModifyTime = time(NULL);

	m_nTreeNodeId = pSrcLineData->nId;
	m_pCurrentTreeNode = pNode;
}

void	CFavoriteTreeListUI::OnEventTreeListItemDragOver()
{
	HCURSOR hCursor   =   LoadCursorW((HINSTANCE)g_hModule,MAKEINTRESOURCE(IDC_DRAGCURSOR));
	::SetCursor(hCursor);

	int nHotIndex = GetHotItem();
	CListLabelElementUI* pDstItem =  (CListLabelElementUI*)GetItemAt(nHotIndex);
	if( pDstItem == NULL)
		return;

	CTreeListUI::Node* pDstNode = (CTreeListUI::Node*)pDstItem->GetTag();
	if( pDstNode == NULL)
	{
		ASSERT(0);
		return;
	}

	SetChildVisible(pDstNode, true);
}

void	CFavoriteTreeListUI::OnEventListItemDragEnd(CDragListUI* pDragList)
{
	int nSelNum = 0;
	int* pSel = pDragList->GetCurSel(nSelNum);
	if( nSelNum != 0 && pSel != NULL)				
	{
		// 得到目标的Item
		// 获取到鼠标所在点的位置
		HCURSOR   hCur   =   ::LoadCursor(NULL,IDC_ARROW); 
		::SetCursor(hCur);

		int nHotIndex = GetHotItem();
		CListLabelElementUI* pDstItem =  (CListLabelElementUI*)GetItemAt(nHotIndex);
		if( pDstItem == NULL)
			return;

		CTreeListUI::Node* pDstNode = (CTreeListUI::Node*)pDstItem->GetTag();
		if( pDstNode == NULL)
		{
			ASSERT(0);
			return;
		}

		std::map<CTreeListUI::Node*, int>::iterator itr = m_mapNodeId.find(pDstNode);
		if( itr == m_mapNodeId.end())
		{
			ASSERT(0);
			return;
		}

		int nDstId = itr->second;

		// 将当前需要拖放的收藏夹作为目标的子收藏夹
		std::vector<CListElementUI*>	vElement;

		int dstIndex = nHotIndex;
		for(int i = 0; i<nSelNum; i++)
		{
			CListElementUI* pElement = (CListElementUI*)pDragList->GetItemAt(pSel[i]);
			if( pElement == NULL)
				continue;

			FAVORITELINEDATA* pData = (FAVORITELINEDATA*)pElement->GetTag();
			if( pData == NULL)
				continue;

			int nId = pData->nId;
			pData->nPid = nDstId;
			pData->nLastModifyTime = time(NULL);

			vElement.push_back(pElement);

			TNotifyUI notify;
			notify.sType = _T("favlistitemmoved");
			notify.pSender = this;
			notify.wParam = nId;
			m_pManager->SendNotify(notify);
		}	

		for(size_t i=0; i<vElement.size(); i++)
		{
			CListElementUI* pElement = vElement[i];
			// 将nId记录从源List中删除
			pDragList->RemoveItem(pElement);
		}

		pDragList->ClearSelectedItem();
		pDragList->Invalidate();
	}
}

void CFavoriteTreeListUI::DoEvent(TEventUI& event) 
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CVerticalLayoutUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_TIMER && event.wParam == SCROLL_TIMERID )
	{
		if( m_dwDelayLeft > 0 ) 
		{
			--m_dwDelayLeft;
			SIZE sz = GetScrollPos();
			LONG lDeltaY =  (LONG)(CalculateDelay((double)m_dwDelayLeft / m_dwDelayNum) * m_dwDelayDeltaY);
			if( (lDeltaY > 0 && sz.cy != 0)  || (lDeltaY < 0 && sz.cy != GetScrollRange().cy ) ) 
			{
				sz.cy -= lDeltaY;
				SetScrollPos(sz);
				return;
			}
		}
		m_dwDelayDeltaY = 0;
		m_dwDelayNum = 0;
		m_dwDelayLeft = 0;
		m_pManager->KillTimer(this, SCROLL_TIMERID);
		return;
	}

	if( event.Type == UIEVENT_SCROLLWHEEL )
	{
		LONG lDeltaY = 0;
		if( m_dwDelayNum > 0 ) lDeltaY =  (LONG)(CalculateDelay((double)m_dwDelayLeft / m_dwDelayNum) * m_dwDelayDeltaY);
		switch( LOWORD(event.wParam) ) 
		{
		case SB_LINEUP:
			if( m_dwDelayDeltaY >= 0 ) m_dwDelayDeltaY = lDeltaY + 8;
			else m_dwDelayDeltaY = lDeltaY + 12;
			break;
		case SB_LINEDOWN:
			if( m_dwDelayDeltaY <= 0 ) m_dwDelayDeltaY = lDeltaY - 8;
			else m_dwDelayDeltaY = lDeltaY - 12;
			break;
		}

		if( m_dwDelayDeltaY > 100 ) m_dwDelayDeltaY = 100;
		else if( m_dwDelayDeltaY < -100 ) m_dwDelayDeltaY = -100;
		m_dwDelayNum = (DWORD)sqrt((double)abs(m_dwDelayDeltaY)) * 5;
		m_dwDelayLeft = m_dwDelayNum;
		m_pManager->SetTimer(this, SCROLL_TIMERID, 50U);

		return;
	}

	if( event.Type == UIEVENT_BUTTONDOWN )
	{
		CListLabelElementUI* pLabelElement = (CListLabelElementUI*)event.pSender;
		if( pLabelElement != NULL)
		{
			int nSelectedIndex = pLabelElement->GetIndex();
			if(nSelectedIndex == 0)
			{
				CListUI::DoEvent(event);

				TNotifyUI notify;
				notify.sType = _T("treelistitemclick");
				notify.pSender = this;
				notify.wParam = event.wParam;
				m_pManager->SendNotify(notify);

				return;
			}
		}

		CTreeListUI::DoEvent(event);
		m_pManager->SetEventSrcControl(this);
		m_bIsDragging = true;

		TNotifyUI notify;
		notify.sType = _T("treelistitemclick");
		notify.pSender = this;
		notify.wParam = event.wParam;
		m_pManager->SendNotify(notify);

		return;
	}

	if( event.Type == UIEVENT_BUTTONUP )
	{
		CControlUI* pSrcCtrl = m_pManager->GetEventSrcControl();
		if( pSrcCtrl == NULL)
			return;

		//如果是TreeList
		if( _tcscmp(pSrcCtrl->GetClass(), _T("FavTreeListUI")) == 0 ) 
		{
			if( m_bIsDragging == true )
			{
				OnEventTreeListItemDragEnd();
				m_bIsDragging = false;
			}
		}
		// 是从列表中拖放过来的
		else if( _tcscmp(pSrcCtrl->GetClass(), _T("ListUI")) == 0 ) 
		{
			CDragListUI* pDragList = (CDragListUI*)pSrcCtrl;
			OnEventListItemDragEnd(pDragList);
		}

		m_pManager->SetEventSrcControl(NULL);
		return;
	}

	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( GetAsyncKeyState(VK_LBUTTON) )
		{
			OnEventTreeListItemDragOver();
		}
		return;
	}

	if( event.Type == UIEVENT_KEYDOWN )
	{
		switch(event.chKey)
		{
		case VK_RETURN:
			{
				int nCurSel = GetCurSel();
				CListLabelElementUI* pItem =  (CListLabelElementUI*)GetItemAt(nCurSel);
				if( pItem == NULL)
					return;

				CTreeListUI::Node* node = (CTreeListUI::Node*)pItem->GetTag();
				SetChildVisible(node, !node->data()._child_visible);

				return;
			}

			break;

		case VK_DELETE:
			{
				int nCurSel = GetCurSel();
				if( nCurSel < 0)
					return;

				// 通知数据中心删除
				MainFrame_DeleteFavoriteFoldEvent* pEvent = new MainFrame_DeleteFavoriteFoldEvent();
				pEvent->desMId = MODULE_ID_MAINFRAME;
				pEvent->nDeleteIndex = nCurSel;
				g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);

				return;
			}
			break;

		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_HOME:
		case VK_END:
			{
				TNotifyUI notify;
				notify.sType = _T("treelistkeydown");
				notify.pSender = this;
				notify.wParam = GetCurSel();
				m_pManager->SendNotify(notify);
			}
			break;
		}

	}


	CTreeListUI::DoEvent(event);
}