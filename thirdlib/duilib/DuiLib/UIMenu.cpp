#include <StdAfx.h>
#include "UIMenu.h"

namespace DuiLib
{
	CMenuUI::CMenuUI()
	{ m_hMenu = NULL; }

	CMenuUI::~CMenuUI()
	{ DestroyMenu(); }

	BOOL CMenuUI::CreateMenu()
	{ return Attach(::CreateMenu()); }

	BOOL CMenuUI::CreatePopupMenu()
	{ return Attach(::CreatePopupMenu()); }

	BOOL CMenuUI::Attach(HMENU hMenu)
	{
		ASSERT(m_hMenu == NULL);        // only attach once, detach on destroy
		if (hMenu == NULL)
		{
			return FALSE;
		}

		m_hMenu=hMenu;
		return TRUE;
	}

	HMENU CMenuUI::Detach()
	{
		HMENU hMenu;
		if ((hMenu = m_hMenu) != NULL)
		{
		}
		m_hMenu = NULL;
		return hMenu;
	}

	BOOL CMenuUI::DestroyMenu()
	{
		if (m_hMenu == NULL)
			return FALSE;
		return ::DestroyMenu(Detach());
	}

	CMenuUI::operator HMENU() const
	{ ASSERT(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
	return this == NULL ? NULL : m_hMenu; }

	BOOL CMenuUI::operator==(const CMenuUI& menu) const
	{ return ((HMENU) menu) == m_hMenu; }

	BOOL CMenuUI::operator!=(const CMenuUI& menu) const
	{ return ((HMENU) menu) != m_hMenu; }

	HMENU CMenuUI::GetSafeHmenu() const
	{ ASSERT(this == NULL || m_hMenu == NULL || ::IsMenu(m_hMenu));
	return this == NULL ? NULL : m_hMenu; }

	BOOL CMenuUI::DeleteMenu(UINT nPosition, UINT nFlags)
	{ ASSERT(::IsMenu(m_hMenu)); return ::DeleteMenu(m_hMenu, nPosition, nFlags); }

	BOOL CMenuUI::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem)
	{ ASSERT(::IsMenu(m_hMenu)); return ::AppendMenu(m_hMenu, nFlags, nIDNewItem, lpszNewItem); }

	BOOL CMenuUI::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
	{ ASSERT(::IsMenu(m_hMenu)); return ::AppendMenu(m_hMenu, nFlags | MF_BITMAP, nIDNewItem,
	(LPCTSTR)hBmp); }

	UINT CMenuUI::CheckMenuItem(UINT nIDCheckItem, UINT nCheck)
	{ ASSERT(::IsMenu(m_hMenu)); return (UINT)::CheckMenuItem(m_hMenu, nIDCheckItem, nCheck); }

	UINT CMenuUI::EnableMenuItem(UINT nIDEnableItem, UINT nEnable)
	{ ASSERT(::IsMenu(m_hMenu)); return ::EnableMenuItem(m_hMenu, nIDEnableItem, nEnable); }

	BOOL CMenuUI::SetDefaultItem(UINT uItem, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::SetMenuDefaultItem(m_hMenu, uItem, fByPos); }

	UINT CMenuUI::GetDefaultItem(UINT gmdiFlags, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuDefaultItem(m_hMenu, fByPos, gmdiFlags); }

	UINT CMenuUI::GetMenuItemCount() const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuItemCount(m_hMenu); }

	UINT CMenuUI::GetMenuItemID(int nPos) const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuItemID(m_hMenu, nPos); }

	UINT CMenuUI::GetMenuState(UINT nID, UINT nFlags) const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuState(m_hMenu, nID, nFlags); }

	int CMenuUI::GetMenuString( UINT nIDItem, LPTSTR lpString, int nMaxCount, UINT nFlags) const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuString(m_hMenu, nIDItem, lpString, nMaxCount, nFlags); }

	BOOL CMenuUI::GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo); }

	BOOL CMenuUI::SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::SetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo); }

	HMENU CMenuUI::GetSubMenu(int nPos) const
	{ ASSERT(::IsMenu(m_hMenu)); return ::GetSubMenu(m_hMenu, nPos); }

	BOOL CMenuUI::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,
		LPCTSTR lpszNewItem)
	{ ASSERT(::IsMenu(m_hMenu)); return ::InsertMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem); }

	BOOL CMenuUI::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
	{ ASSERT(::IsMenu(m_hMenu)); return ::InsertMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem,
	(LPCTSTR)hBmp); }

	BOOL CMenuUI::InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
	{ ASSERT(::IsMenu(m_hMenu)); return ::InsertMenuItem(m_hMenu, uItem, fByPos, lpMenuItemInfo); }

	BOOL CMenuUI::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem)
	{ ASSERT(::IsMenu(m_hMenu)); return ::ModifyMenu(m_hMenu, nPosition, nFlags, nIDNewItem, lpszNewItem); }

	BOOL CMenuUI::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp)
	{ ASSERT(::IsMenu(m_hMenu)); return ::ModifyMenu(m_hMenu, nPosition, nFlags | MF_BITMAP, nIDNewItem,
	(LPCTSTR)hBmp); }

	BOOL CMenuUI::RemoveMenu(UINT nPosition, UINT nFlags)
	{ ASSERT(::IsMenu(m_hMenu)); return ::RemoveMenu(m_hMenu, nPosition, nFlags); }

	BOOL CMenuUI::SetMenuItemBitmaps(UINT nPosition, UINT nFlags,
		const HBITMAP hBmpUnchecked, const HBITMAP hBmpChecked)
	{ 
		ASSERT(::IsMenu(m_hMenu)); 
		return ::SetMenuItemBitmaps(m_hMenu, nPosition, nFlags,
			(HBITMAP)hBmpUnchecked,	(HBITMAP)hBmpChecked);
	}

	BOOL CMenuUI::TrackPopupMenu(UINT nFlags, int x, int y,
		HWND hWnd, LPCRECT lpRect)
	{
		ASSERT(m_hMenu != NULL);

		BOOL bOK = ::TrackPopupMenu(m_hMenu, nFlags, x, y, 0,
			hWnd, lpRect);
		return bOK;
	}

	BOOL CMenuUI::TrackPopupMenuEx(UINT fuFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm)
	{
		ASSERT(m_hMenu != NULL);

		BOOL bOK = ::TrackPopupMenuEx(m_hMenu, fuFlags, x, y,
			hWnd, lptpm);
		return bOK;
	}

	void CMenuUI::DrawItem(LPDRAWITEMSTRUCT /*pdis*/)
	{
		// default does nothing
	}

	void CMenuUI::MeasureItem(LPMEASUREITEMSTRUCT /*pmis*/)
	{
		// default does nothing
	}

}