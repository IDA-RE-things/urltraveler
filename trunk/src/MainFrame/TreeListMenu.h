#pragma once

#include "Menu.h"
#include "TreeList.h"
#include "UIMenu.h"

// Ê÷¿Ø¼þ²Ëµ¥Ïî
#define	IDM_TREELIST_ADD	20000
#define	IDM_TREELIST_DELETE	20001
#define	IDM_TREELIST_RENAME	20002

class CTreeListMenu : public CMenuUI
{
public:
	void	Init(bool bSelectItem, int nSelIndex);

	void OnNew();
	void OnDelete();
};