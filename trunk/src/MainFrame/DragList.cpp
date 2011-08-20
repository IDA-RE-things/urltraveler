#include <stdafx.h>
#include "DragList.h"
#include "resource.h"
#include "DataCenterDefine.h"
#include "MainFrameModule.h"
#include "time.h"
#include "MainFrameDefine.h"

using namespace datacenter;
using namespace mainframe;

void CDragListUI::SetManager( CPaintManagerUI* pManager, CControlUI* pParent, bool bInit /*= true*/ )
{
	CListUI::SetManager(pManager, pParent, bInit);

	if( m_bAddNotifyer == false)
	{
		m_pManager->AddNotifier(this);
		m_bAddNotifyer = true;
	}
}

void	CDragListUI::OnEventListItemDragEnd(CDragListUI* pDragList)
{
	// 获取到鼠标所在点的位置
	HCURSOR   hCursor   =   ::LoadCursor(NULL,IDC_ARROW); 
	::SetCursor(hCursor);

	int nHotIndex = GetHotItem();
	CListElementUI* pDstItem =  (CListElementUI*)GetItemAt(nHotIndex);
	if( pDstItem == NULL)
		return;

	// 找到源item
	int nSelNum = 0;
	int* pSel = GetCurSel(nSelNum);
	if( nSelNum == 0 || pSel == NULL)
		return;

	std::vector<CListElementUI*> vElements;
	for(int i=0; i<nSelNum; i++)
	{
		CListElementUI* pElement = (CListElementUI*)GetItemAt(pSel[i]);
		vElements.push_back(pElement);
	}

	for( size_t i=0; i<vElements.size(); i++)
	{
		this->AddAt(vElements[i], nHotIndex);
	}
}

void	CDragListUI::OnItemDragEnd()
{
/*
	CControlUI* pSrcCtrl = m_pManager->GetEventSrcControl();
	if( pSrcCtrl == NULL)
		return;

	// 是从列表中拖放过来的
	if( _tcscmp(pSrcCtrl->GetClass(), _T("ListUI")) == 0 ) 
	{
		CDragListUI* pDragList = (CDragListUI*)pSrcCtrl;
		OnEventListItemDragEnd(pDragList);
	}
*/
}

void	CDragListUI::OnItemDragOver()
{
	HCURSOR hCursor   =   LoadCursorW((HINSTANCE)g_hModule,MAKEINTRESOURCE(IDC_DRAGCURSOR));
	::SetCursor(hCursor);

	int nHotIndex = GetHotItem();
	CListElementUI* pDstItem =  (CListElementUI*)GetItemAt(nHotIndex);
	if( pDstItem == NULL)
		return;
}

void CDragListUI::DoEvent(TEventUI& event) 
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CListUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_BUTTONDOWN )
	{
		CListUI::DoEvent(event);
		m_pManager->SetEventSrcControl(this);

		m_bIsDragging = true;
		return;
	}
	
	if( event.Type == UIEVENT_BUTTONUP )
	{
		CListUI::DoEvent(event);

		if( m_bIsDragging == true )
		{
			OnItemDragEnd();
			m_bIsDragging = false;
		}
		m_pManager->SetEventSrcControl(NULL);
		return;
	}

	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( GetAsyncKeyState(VK_LBUTTON) )
		{
			OnItemDragOver();
		}
		return;
	}

	CListUI::DoEvent(event);
}