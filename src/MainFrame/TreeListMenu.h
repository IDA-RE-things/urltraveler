#pragma once

#include "Menu.h"
#include "FavTreeList.h"
#include "UIMenu.h"

// ���ؼ��˵���
#define	IDM_TREELIST_ADD	20000
#define	IDM_TREELIST_DELETE	20001
#define	IDM_TREELIST_RENAME	20002

class CTreeListMenu : public CMenuUI
{
public:
	void	Init(bool bSelectItem, int nSelIndex);
};