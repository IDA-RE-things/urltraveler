#include "FavoriteListMenu.h"
#include "MainFrameDefine.h"
#include "MainFrameModule.h"

using namespace mainframe;

void	CFavoriteListMenu::Init(bool bSelectItem)
{
	if( bSelectItem == true)
	{
		InsertMenu(0,MF_BYCOMMAND, IDM_OPEN, L"打开网站");
		InsertMenu(1,MF_SEPARATOR, 0, L"");

		InsertMenu(2,MF_BYCOMMAND, IDM_ADD, L"增加收藏");
		InsertMenu(3,MF_BYCOMMAND, IDM_EDIT, L"编辑收藏");
		InsertMenu(4,MF_BYCOMMAND, IDM_DELETE, L"删除收藏");
		InsertMenu(5,MF_BYCOMMAND, IDM_RENAME, L"重命名");
		InsertMenu(6,MF_BYCOMMAND, IDM_COPYURL, L"复制网址URL");

		InsertMenu(7,MF_SEPARATOR, 0, L"");

		InsertMenu(8,MF_BYCOMMAND, IDM_SELECTALL, L"全选");
		InsertMenu(9,MF_BYCOMMAND, IDM_OPEN, L"打开该网址");
		InsertMenu(10,MF_BYCOMMAND, IDM_SHARE, L"分享该网址");
	}
	else
	{
		InsertMenu(0,MF_BYCOMMAND, IDM_ADD, L"增加收藏");
		InsertMenu(1,MF_BYCOMMAND, IDM_SELECTALL, L"全选");
	}
}

void CFavoriteListMenu::OnAdd()
{
	g_MainFrameModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_MAINFRAME_ADD_URL, MODULE_ID_MAINFRAME));
}

void	CFavoriteListMenu::OnDelete()
{
/*
	if( m_pOwner ) 
	{
		CListUI* pList = static_cast<CListUI*>(m_pOwner);
		int nSel = pList->GetCurSel();
		if( nSel < 0 ) return;

		CListElementUI* pElement =		 (CListElementUI*)pList->GetSubItem(nSel);
		if( pElement == NULL)
			return;

		FAVORITELINEDATA *pSelNode = (FAVORITELINEDATA *)(pElement->GetTag());
		pSelNode->bDelete = true;
		pList->RemoveAt(nSel);

		// 从数据中心中删除该Item
		MainFrame_DeleteFavoriteEvent* pEvent = new MainFrame_DeleteFavoriteEvent();
		pEvent->desMId = MODULE_ID_MAINFRAME;
		pEvent->nDeleteNodeId = pSelNode->nId;
		g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
	}
*/
}

void	CFavoriteListMenu::OnOpen()
{
/*
	CListUI* pList = static_cast<CListUI*>(m_pOwner);
	int nSel = pList->GetCurSel();
	if( nSel < 0 ) return;

	// 通知打开URL
	g_MainFrameModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_MAINFRAME_OPEN_URL, MODULE_ID_MAINFRAME, nSel));
*/
}

void	CFavoriteListMenu::OnCopyUrl()
{
/*
	CListUI* pList = static_cast<CListUI*>(m_pOwner);
	int nSel = pList->GetCurSel();
	if( nSel < 0 ) return;

	// 通知打开URL
	g_MainFrameModule->GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_MAINFRAME_COPY_URL, MODULE_ID_MAINFRAME, nSel));
*/
}

void CFavoriteListMenu::OnEdit()
{
/*
	CListUI* pList = static_cast<CListUI*>(m_pOwner);

	pList->EditItem(m_nX, m_nY);
*/
}

void CFavoriteListMenu::Notify(TNotifyUI& msg)
{
/*
		if( msg.sType == _T("itemselect") ) {
			Close();
		}
		else if( msg.sType == _T("itemclick") ) 
		{
			if( msg.pSender->GetName() == _T("menu_Add") ) 
			{
				OnAdd();
			}
			else if( msg.pSender->GetName() == _T("menu_Delete") ) 
			{
				OnDelete();
			}
			else if( msg.pSender->GetName() == _T("menu_Open") )
			{
				OnOpen();
			}
			else if( msg.pSender->GetName() == _T("menu_CopyUrl") )
			{
				OnCopyUrl();
			}
			else if( msg.pSender->GetName() == _T("menu_Edit") )
			{
				OnEdit();
			}
		}*/
	
}