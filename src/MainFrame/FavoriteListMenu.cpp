#include "FavoriteListMenu.h"
#include "MainFrameDefine.h"
#include "MainFrameModule.h"

using namespace mainframe;

void	CFavoriteListMenu::Init(int nSelNum)
{
	if( nSelNum == 1)
	{
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_OPEN, L"����վ");
		AppendMenu(MF_SEPARATOR, 0, L"");

		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_ADD, L"�����ղ�");
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_EDIT, L"�༭�ղ�");
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_DELETE, L"ɾ���ղ�");
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_COPYURL, L"������ַURL");

		AppendMenu(MF_SEPARATOR, 0, L"");

		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_SELECTALL, L"ȫѡ");
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_SHARE, L"�������ַ");
	}
	else if( nSelNum > 1)
	{
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_OPEN, L"��������ַ");
		AppendMenu(MF_SEPARATOR, 0, L"");

		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_ADD, L"�����ղ�");
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_DELETE, L"ɾ���ղ�");

		AppendMenu(MF_SEPARATOR, 0, L"");

		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_SELECTALL, L"ȫѡ");
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_SHARE, L"����ѡ����ַ");
	}
	else if( nSelNum == 0)
	{
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_ADD, L"�����ղ�");
		AppendMenu(MF_BYCOMMAND, IDM_FAVLIST_SELECTALL, L"ȫѡ");
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

		// ������������ɾ����Item
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

	// ֪ͨ��URL
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

	// ֪ͨ��URL
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