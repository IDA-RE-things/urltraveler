#pragma once

#include "Menu.h"
#include "TreeList.h"

class CTreeListMenu : public CMenuWnd
{
public:
	CTreeListMenu(wchar_t*	 pSkinXml) : CMenuWnd(pSkinXml)
	{
	}

	void OnNew()
	{
		if( m_pOwner ) 
		{
			TreeListUI* pTree = static_cast<TreeListUI*>(m_pOwner);
			if( pTree)
			{

			}
		}	
	}

	void OnDelete()
	{
		if( m_pOwner ) 
		{
			TreeListUI* pTree = static_cast<TreeListUI*>(m_pOwner);
			if( pTree)
			{
				int nIndex = pTree->GetCurSel();
				CListLabelElementUI* pElement = (CListLabelElementUI*)pTree->GetSubItem(nIndex);
				if( pElement == NULL)
					return;

				TreeListUI::Node* pNode  = (TreeListUI::Node*)pElement->GetTag();
				pTree->RemoveNode(pNode);
			}
		}	
	}

	void Notify(TNotifyUI& msg)
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
};