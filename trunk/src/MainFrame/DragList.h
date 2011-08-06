#ifndef __DRAGLISTUI_H__
#define __DRAGLISTUI_H__

#include <vector>
#include <math.h>
#include "UIList.h"
#include <map>

extern HMODULE	g_hModule;

using namespace DuiLib;
using namespace std;

class DragListUI : public CListUI
{
public:
	enum { SCROLL_TIMERID = 10 };

	DragListUI() 
	{
		m_ListInfo.bMultiSelect = true;
		m_bAddNotifyer = false;
		m_bIsDragging = false;
	}

	~DragListUI() { }

	virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	void	OnItemDragOver();
	void	OnItemDragEnd();


	void	OnListItemDragEnd(DragListUI* pDragList);


	void DoEvent(TEventUI& event) ;

public:

	bool	m_bIsDragging;
};

#endif __DRAGLISTUI_H__
