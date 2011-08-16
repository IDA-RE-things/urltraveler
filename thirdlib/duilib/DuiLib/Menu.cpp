#include <StdAfx.h>
#include "Menu.h"

MenuUI::MenuUI()
	{ m_hMenu = NULL; }

 MenuUI::~MenuUI()
	{ DestroyMenu(); }

 BOOL MenuUI::CreateMenu()
	{ return Attach(::CreateMenu()); }

 BOOL MenuUI::CreatePopupMenu()
	{ return Attach(::CreatePopupMenu()); }

 BOOL MenuUI::Attach(HMENU hMenu)
{
	ASSERT(m_hMenu == NULL);        // only attach once, detach on destroy
	if (hMenu == NULL)
	{
		return FALSE;
	}
    
	m_hMenu=hMenu;
}

 HMENU MenuUI::Detach()
{
	HMENU hMenu;
	if ((hMenu = m_hMenu) != NULL)
	{
	}
	m_hMenu = NULL;
	return hMenu;
}

 BOOL MenuUI::DestroyMenu()
{
	if (m_hMenu == NULL)
		return FALSE;
	return ::DestroyMenu(Detach());
}

 MenuUI::operator HMENU() const
	{ ASSERT(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
		return this == NULL ? NULL : m_hMenu; }

 BOOL MenuUI::operator==(const MenuUI& menu) const
	{ return ((HMENU) menu) == m_hMenu; }

 BOOL MenuUI::operator!=(const MenuUI& menu) const
	{ return ((HMENU) menu) != m_hMenu; }

 HMENU MenuUI::GetSafeHmenu() const
	{ ASSERT(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
		return this == NULL ? NULL : m_hMenu; }

 BOOL MenuUI::DeleteMenu(UINT nPosition, UINT nFlags)
	{ ASSERT(::IsMenu(m_hMenu)); return ::DeleteMenu(m_hMenu, nPosition, nFlags); }

 BOOL MenuUI::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem)
	{ ASSERT(::IsMenu(m_hMenu)); return ::AppendMenu(m_hMenu, nFlags, nIDNewItem, lpszNewItem); }

 BOOL MenuUI::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
	{ ASSERT(::IsMenu(m_hMenu)); return ::AppendMenu(m_hMenu, nFlags | MF_BITMAP, nIDNewItem,
		(LPCTSTR)hBmp); }

 UINT MenuUI::CheckMenuItem(UINT nIDCheckItem, UINT nCheck)
	{ ASSERT(::IsMenu(m_hMenu)); return (UINT)::CheckMenuItem(m_hMenu, nIDCheckItem, nCheck); }

 UINT MenuUI::EnableMenuItem(UINT nIDEnableItem, UINT nEnable)
	{ ASSERT(::IsMenu(m_hMenu)); return ::EnableMenuItem(m_hMenu, nIDEnableItem, nEnable); }

 BOOL MenuUI::SetDefaultItem(UINT uItem, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::SetMenuDefaultItem(m_hMenu, uItem, fByPos); }

 UINT MenuUI::GetDefaultItem(UINT gmdiFlags, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuDefaultItem(m_hMenu, fByPos, gmdiFlags); }

 UINT MenuUI::GetMenuItemCount() const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuItemCount(m_hMenu); }

 UINT MenuUI::GetMenuItemID(int nPos) const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuItemID(m_hMenu, nPos); }

 UINT MenuUI::GetMenuState(UINT nID, UINT nFlags) const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuState(m_hMenu, nID, nFlags); }

 int MenuUI::GetMenuString( UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags) const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuString(m_hMenu, nIDItem, lpString, nMaxCount, nFlags); }

 BOOL MenuUI::GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo); }

 BOOL MenuUI::SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::SetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo); }

 HMENU MenuUI::GetSubMenu(int nPos) const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetSubMenu(m_hMenu, nPos); }

 BOOL MenuUI::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,
		LPCTSTR lpszNewItem)
	{ ASSERT(::IsMenu(m_hMenu)); return ::InsertMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem); }

 BOOL MenuUI::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
	{ ASSERT(::IsMenu(m_hMenu)); return ::InsertMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem,
		(LPCTSTR)hBmp); }

 BOOL MenuUI::InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::InsertMenuItem(m_hMenu, uItem, fByPos, lpMenuItemInfo); }

 BOOL MenuUI::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem)
	{ ASSERT(::IsMenu(m_hMenu)); return ::ModifyMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem); }

 BOOL MenuUI::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
	{ ASSERT(::IsMenu(m_hMenu)); return ::ModifyMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem,
		(LPCTSTR)hBmp); }

 BOOL MenuUI::RemoveMenu(UINT nPosition, UINT nFlags)
	{ ASSERT(::IsMenu(m_hMenu)); return ::RemoveMenu(m_hMenu, nPosition, nFlags); }

 BOOL MenuUI::SetMenuItemBitmaps(UINT nPosition, UINT nFlags,
		const HBITMAP hBmpUnchecked, const HBITMAP hBmpChecked)
{ 
	ASSERT(::IsMenu(m_hMenu)); 
	return ::SetMenuItemBitmaps(m_hMenu, nPosition, nFlags,
		(HBITMAP)hBmpUnchecked,	(HBITMAP)hBmpChecked);
 }

 void MenuUI::DrawItem(LPDRAWITEMSTRUCT /*pdis*/)
{
	// default does nothing
}

void MenuUI::MeasureItem(LPMEASUREITEMSTRUCT /*pmis*/)
{
	// default does nothing
}