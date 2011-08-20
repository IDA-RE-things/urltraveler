#ifndef __DRAGLISTUI_H__
#define __DRAGLISTUI_H__

#include <vector>
#include <math.h>
#include "UIList.h"
#include <map>

extern HMODULE	g_hModule;

using namespace DuiLib;
using namespace std;

class CDragListUI : public CListUI
{
public:
	CDragListUI();
	~CDragListUI() { }

	void Notify(TNotifyUI& msg);

	virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	void	OnEventItemDragOver();
	void	OnEventItemDragEnd();
	void	OnEventListItemDragEnd(CDragListUI* pDragList);
	void DoEvent(TEventUI& event) ;

public:

	bool	m_bIsDragging;
};

#endif __DRAGLISTUI_H__
