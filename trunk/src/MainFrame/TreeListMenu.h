#pragma once

#include "Menu.h"
#include "TreeList.h"

class CTreeListMenu : public CMenuWnd
{
public:
	CTreeListMenu(wchar_t*	 pSkinXml) : CMenuWnd(pSkinXml)
	{
	}

	void OnNew();
	void OnDelete();
	void Notify(TNotifyUI& msg);
};