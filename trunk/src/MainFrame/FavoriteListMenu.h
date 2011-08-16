#pragma once

#include "Menu.h"
#include "MainFrameDefine.h"
#include "UIMenu.h"

using namespace mainframe;
using namespace DuiLib;

#define	IDM_ADD			10000
#define	IDM_DELETE		10001
#define	IDM_OPEN		10002
#define	IDM_COPYURL		10003
#define	IDM_EDIT			10004
#define	IDM_RENAME		10005
#define	IDM_SHARE		10006
#define	IDM_SELECTALL		10007

class CFavoriteListMenu : public CMenuUI
{
public:

	void	Init(bool bSelectItem);

	void	OnAdd();
	void	OnDelete();
	void	OnOpen();
	void	OnCopyUrl();
	void	OnEdit();

	void	Notify(TNotifyUI& msg);
};