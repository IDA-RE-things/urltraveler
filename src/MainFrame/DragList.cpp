#include <stdafx.h>
#include "DragList.h"
#include "resource.h"
#include "DataCenterDefine.h"
#include "MainFrameModule.h"
#include "time.h"
#include "MainFrameDefine.h"

using namespace datacenter;
using namespace mainframe;

void DragListUI::Notify(TNotifyUI& msg)
{
}

void DragListUI::SetManager( CPaintManagerUI* pManager, CControlUI* pParent, bool bInit /*= true*/ )
{
	CListUI::SetManager(pManager, pParent, bInit);

	if( m_bAddNotifyer == false)
	{
		m_pManager->AddNotifier(this);
		m_bAddNotifyer = true;
	}
}

void	DragListUI::OnItemDragEnd()
{
	// 获取到鼠标所在点的位置
	HCURSOR   hCursor   =   ::LoadCursor(NULL,IDC_ARROW); 
	::SetCursor(hCursor);

	int nHotIndex = GetHotItem();
	CListElementUI* pDstItem =  (CListElementUI*)GetItemAt(nHotIndex);
	if( pDstItem == NULL)
		return;

	// 找到源item

}

void	DragListUI::OnItemDragOver()
{
	HCURSOR hCursor   =   LoadCursorW((HINSTANCE)g_hModule,MAKEINTRESOURCE(IDC_DRAGCURSOR));
	::SetCursor(hCursor);

	int nHotIndex = GetHotItem();
	CListElementUI* pDstItem =  (CListElementUI*)GetItemAt(nHotIndex);
	if( pDstItem == NULL)
		return;
}

void DragListUI::DoEvent(TEventUI& event) 
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

/*
		if( m_bIsDragging == true )
		{
			OnItemDragEnd();
			m_bIsDragging = false;
		}
*/
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