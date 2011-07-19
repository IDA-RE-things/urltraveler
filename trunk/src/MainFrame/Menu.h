#pragma once

#include "stdafx.h"
#include "UIlib.h"
#include <string>
#include "assert.h"

using namespace std;
using namespace DuiLib;

class CMenuWnd : public CWindowWnd, public INotifyUI
{
public:
	CMenuWnd(){}

	CMenuWnd(wchar_t* pSkinXml) : m_pOwner(NULL)
	{ 
		assert( pSkinXml != NULL);
		m_strSkinXml	=	 pSkinXml;
	}

	void Init(CControlUI* pOwner, CRect rc); 

	LPCTSTR GetWindowClassName() const { return _T("MenuWnd"); }
	void Notify(TNotifyUI& msg);

	void OnFinalMessage(HWND /*hWnd*/){	delete this; };
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) ;

	void	Enable(LPCTSTR lMenuItemName, bool bEnabled);

public:
	CPaintManagerUI m_pm;
	CControlUI* m_pOwner;
	std::wstring	m_strSkinXml;	//	Æ¤·ôÎÄ¼þµÄxml
};