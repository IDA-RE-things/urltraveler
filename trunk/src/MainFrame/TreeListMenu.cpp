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
			AppendMenu(MF_BYCOMMAND, IDM_TREELIST_ADD, L"新建收藏夹");
		}
		else
		{
			AppendMenu(MF_BYCOMMAND, IDM_TREELIST_ADD, L"新建收藏夹");
			AppendMenu(MF_BYCOMMAND, IDM_TREELIST_DELETE, L"删除收藏夹");
			AppendMenu(MF_BYCOMMAND, IDM_TREELIST_RENAME, L"重命名");
		}
	}
	else
	{
		//AppendMenu(MF_BYCOMMAND, IDM_TREELIST_ADD, L"新建收藏夹");
	}
}