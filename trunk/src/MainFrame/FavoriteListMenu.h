#pragma once

#include "Menu.h"
#include "MainFrameDefine.h"
#include "UIMenu.h"

using namespace mainframe;
using namespace DuiLib;

// 列表菜单项
#define	IDM_FAVLIST_ADD			10000
#define	IDM_FAVLIST_DELETE		10001
#define	IDM_FAVLIST_OPEN		10002
#define	IDM_FAVLIST_COPYURL		10003
#define	IDM_FAVLIST_EDIT			10004
#define	IDM_FAVLIST_RENAME		10005
#define	IDM_FAVLIST_SHARE		10006
#define	IDM_FAVLIST_SELECTALL		10007

class CFavoriteListMenu : public CMenuUI
{
public:

	void	Init(int nSelNum);

	void	OnAdd();
	void	OnDelete();
	void	OnOpen();
	void	OnCopyUrl();
	void	OnEdit();

	void	Notify(TNotifyUI& msg);
};