#pragma once

#include "Menu.h"
#include "MainFrameDefine.h"

using namespace mainframe;

class CFavoriteListMenu : public CMenuWnd
{
public:
	CFavoriteListMenu(wchar_t*	 pSkinXml) : CMenuWnd(pSkinXml)
	{
	}

	void	OnAdd();
	void	OnDelete();
	void	OnOpen();
	void	OnCopyUrl();
	void	OnEdit();

	void	Notify(TNotifyUI& msg);

	void	StoreLanuchPos(int nX, int nY);

	//����˵�ʱ���x �� y��������
	int m_nX;
	int m_nY;
};