#pragma once

#include "UIlib.h"

namespace DuiLib
{

	class UILIB_API CMenuUI
	{
	public:

		// Constructors
		CMenuUI();

		BOOL CreateMenu();
		BOOL CreatePopupMenu();
		BOOL LoadMenuIndirect(const void* lpMenuTemplate);
		BOOL DestroyMenu();

		// Attributes
		HMENU m_hMenu;          // must be first data member
		HMENU GetSafeHmenu() const;
		operator HMENU() const;

		BOOL Attach(HMENU hMenu);
		HMENU Detach();

		// CMenuUI Operations
		BOOL DeleteMenu(UINT nPosition, UINT nFlags);
		BOOL TrackPopupMenu(UINT nFlags, int x, int y,
			HWND hWnd, LPCRECT lpRect = 0);
		BOOL TrackPopupMenuEx(UINT fuFlags, int x, int y, HWND hWnd, LPTPMPARAMS lptpm);


		BOOL operator==(const CMenuUI& menu) const;
		BOOL operator!=(const CMenuUI& menu) const;

		// CMenuItem Operations
		BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem = 0,
			LPCTSTR lpszNewItem = NULL);
		BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp);

		UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck);
		UINT EnableMenuItem(UINT nIDEnableItem, UINT nEnable);
		UINT GetMenuItemCount() const;
		UINT GetMenuItemID(int nPos) const;
		UINT GetMenuState(UINT nID, UINT nFlags) const;
		int GetMenuString(UINT nIDItem, LPTSTR lpString, int nMaxCount,
			UINT nFlags) const;
		BOOL GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo,
			BOOL fByPos = FALSE);
		BOOL SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo,
			BOOL fByPos = FALSE);
		HMENU GetSubMenu(int nPos) const;
		BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0,
			LPCTSTR lpszNewItem = NULL);
		BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const HBITMAP hBmp);

		BOOL InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo,
			BOOL fByPos = FALSE);
		BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0,
			LPCTSTR lpszNewItem = NULL);
		BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,
			const HBITMAP hBmp);
		BOOL RemoveMenu(UINT nPosition, UINT nFlags);
		BOOL SetMenuItemBitmaps(UINT nPosition, UINT nFlags,
			const HBITMAP hBmpUnchecked, const HBITMAP hBmpChecked);
		BOOL SetDefaultItem(UINT uItem, BOOL fByPos = FALSE);
		UINT GetDefaultItem(UINT gmdiFlags, BOOL fByPos = FALSE);

		// Overridables (must override draw and measure for owner-draw menu items)
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

		// Implementation
	public:
		virtual ~CMenuUI();
	};

}