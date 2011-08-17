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