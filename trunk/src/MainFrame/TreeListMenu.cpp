#include "stdafx.h"
#include "TreeListMenu.h"
#include "MainFrameDefine.h"
#include "MainFrameModule.h"

using namespace mainframe;

void	CTreeListMenu::Init(bool bSelectItem, int nSelIndex)
{
	if( bSelectItem == true)
	{
		if( nSelIndex == 0)
		{
			AppendMenu(MF_BYCOMMAND, IDM_TREELIST_ADD, L"�½��ղؼ�");
		}
		else
		{
			AppendMenu(MF_BYCOMMAND, IDM_TREELIST_ADD, L"�½��ղؼ�");
			AppendMenu(MF_BYCOMMAND, IDM_TREELIST_DELETE, L"ɾ���ղؼ�");
			AppendMenu(MF_BYCOMMAND, IDM_TREELIST_RENAME, L"������");
		}
	}
	else
	{
		//AppendMenu(MF_BYCOMMAND, IDM_TREELIST_ADD, L"�½��ղؼ�");
	}
}

void CTreeListMenu::OnNew()
{
/*
	if( m_pOwner ) 
	{
		TreeListUI* pTree = static_cast<TreeListUI*>(m_pOwner);
		if( pTree)
		{

		}
	}	
*/
}

void CTreeListMenu::OnDelete()
{
/*
	if( m_pOwner ) 
	{
		TreeListUI* pTree = static_cast<TreeListUI*>(m_pOwner);
		if( pTree)
		{
			int nIndex = pTree->GetCurSel();
			if( nIndex < 0)
				return;

			// ֪ͨ��������ɾ��
			MainFrame_DeleteFavoriteFoldEvent* pEvent = new MainFrame_DeleteFavoriteFoldEvent();
			pEvent->desMId = MODULE_ID_MAINFRAME;
			pEvent->nDeleteIndex = nIndex;
			g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
		}
	}	
*/
}