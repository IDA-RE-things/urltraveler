#include "stdafx.h"
#include "TreeListMenu.h"
#include "MainFrameDefine.h"
#include "MainFrameModule.h"

using namespace mainframe;

void CTreeListMenu::OnNew()
{
	if( m_pOwner ) 
	{
		TreeListUI* pTree = static_cast<TreeListUI*>(m_pOwner);
		if( pTree)
		{

		}
	}	
}

void CTreeListMenu::OnDelete()
{
	if( m_pOwner ) 
	{
		TreeListUI* pTree = static_cast<TreeListUI*>(m_pOwner);
		if( pTree)
		{
			int nIndex = pTree->GetCurSel();
			if( nIndex < 0)
				return;

			// 通知数据中心删除
			MainFrame_DeleteFavoriteFoldEvent* pEvent = new MainFrame_DeleteFavoriteFoldEvent();
			pEvent->desMId = MODULE_ID_MAINFRAME;
			pEvent->nDeleteIndex = nIndex;
			g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
		}
	}	
}

void CTreeListMenu::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("itemselect") ) {
		Close();
	}
	else if( msg.sType == _T("itemclick") ) {
		if( msg.pSender->GetName() == _T("menu_New") ) 
		{
			OnNew();
		}
		else if( msg.pSender->GetName() == _T("menu_Delete") ) 
		{
			OnDelete();
		}
	}
}