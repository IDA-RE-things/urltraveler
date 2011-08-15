#include "StdAfx.h"
#include "UIMenu.h"


/////////////////////// CMenuUI ///////////////////////
using namespace DuiLib;

CMenuUI::CMenuUI(HWND hWnd,CPaintManagerUI* pManager)
{
	_hWnd=hWnd;
	_pManager=pManager;
	_hMenu =::CreatePopupMenu();

	Init();
}
void CMenuUI::AppendMenu(UINT uFlags,UINT uIDNewItem,LPCTSTR lpNewItem)
{
	::AppendMenu(_hMenu, uFlags, uIDNewItem, lpNewItem);
}

CMenuUI::~CMenuUI()
{
	for ( int i = 0; i < _pmi.GetSize(); i++)
	{
		delete (LPMENUITEM)_pmi[i];
	}

}

void CMenuUI::Init() 
{
	SetSubModify(_hMenu);
	DefaultMenuStyle();
}


void CMenuUI::Show(POINT pt,UINT uFlags)
{
	if (_hMenu==NULL)
		::MessageBox(_hWnd,TEXT("菜单句柄不能为空"),TEXT("CMenuUI"),0);

	::TrackPopupMenuEx(_hMenu, uFlags , pt.x, pt.y, _hWnd,NULL);
}


void CMenuUI::DefaultMenuStyle()
{
	_MenuStyle.ItemHeight = 23;
	_MenuStyle.SeparatorHeight = 3;
	_MenuStyle.ColorText =RGB(0,0,0);// GetSysColor(COLOR_MENUTEXT);
	_MenuStyle.ColorHighLight =RGB(233,245,255);// GetSysColor(COLOR_HIGHLIGHT);
	_MenuStyle.ColorBackground =RGB(252,252,249); //RGB(250,240,230);// GetSysColor(COLOR_MENU);
	_MenuStyle.ColorTextSelected =RGB(0,0,0);// GetSysColor(COLOR_HIGHLIGHTTEXT);
}

void CMenuUI::SetMenuStyle(MENUSTYLE ms)
{
	if ((ms.fMask & MSTYLE_HEIGHT) == MSTYLE_HEIGHT)
		_MenuStyle.ItemHeight = ms.ColorHighLight;
	if ((ms.fMask & MSTYLE_SEPARATOR) == MSTYLE_SEPARATOR)
		_MenuStyle.SeparatorHeight = ms.SeparatorHeight;
	if ((ms.fMask & MSTYLE_TEXT) == MSTYLE_TEXT)
		_MenuStyle.ColorText = ms.ColorText;
	if ((ms.fMask & MSTYLE_HIGHTLIGHT) == MSTYLE_HIGHTLIGHT)
		_MenuStyle.ColorHighLight = ms.ColorHighLight;
	if ((ms.fMask & MSTYLE_BACKGROUND) == MSTYLE_BACKGROUND)
		_MenuStyle.ColorBackground = ms.ColorBackground;
	if ((ms.fMask & MSTYLE_SELECTED) == MSTYLE_SELECTED)
		_MenuStyle.ColorTextSelected = ms.ColorTextSelected;
}

void CMenuUI::SetSubModify(HMENU hMenu)
{
	int mic = GetMenuItemCount(hMenu);

	for (int i = 0; i < mic; i++)
	{
		LPMENUITEM pmi = new MENUITEM;
		//_pmi.push_back(pmi);
		_pmi.Add(pmi);

		pmi->hMenu = hMenu;
		pmi->uItem = i;
		pmi->hID   = GetMenuItemID(hMenu, i);
		HMENU hSubMenu = GetSubMenu(hMenu, i);
		SetSubModify(hSubMenu);

		UINT state = GetMenuState(hMenu, i, MF_BYPOSITION);
		if ((state & MF_POPUP) == MF_POPUP)
		{
			state &= ~MF_POPUP;
			state &= ~MF_SEPARATOR;
		}

		ModifyMenu (hMenu, i,
			state | MF_BYPOSITION | MF_OWNERDRAW,
			pmi->hID, (LPCTSTR) pmi);


	}
}

void CMenuUI::GetMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MEASUREITEM:
		OnMeasureItem(wParam, lParam);
		break;

	case WM_DRAWITEM:
		OnDrawItem(wParam, lParam);
		break;
	}
}

void CMenuUI::OnMeasureItem(WPARAM wParam, LPARAM lParam)
{
	LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT) lParam;
	if (pmis->CtlType != ODT_MENU)
	{
		return;
	}

	LPMENUITEM pmi = (LPMENUITEM) pmis->itemData;
	TCHAR Buffer[MENU_TEXT_LENGTH];
	GetMenuString(pmi->hMenu, pmi->uItem, Buffer,  MENU_TEXT_LENGTH, MF_BYPOSITION);

	TCHAR * p = NULL;
	if (NULL != (p = lstrchr(Buffer, '&')))
	{
		* p = 0;
		wsprintf(Buffer, TEXT("%s%s"), Buffer, p + 1);
	}

	p = NULL;
	if (NULL != (p = lstrchr(Buffer, '\t')))
	{
		* p = 0;
		TCHAR tmp[20];
		lstrcpy(tmp, p + 1);
		wsprintf(Buffer, TEXT("%s  %s"), Buffer, tmp);
	}

	HDC hdc = GetDC(_hWnd);
	SIZE strSize;
	GetTextExtentPoint (hdc, Buffer, lstrlen (Buffer), &strSize);
	ReleaseDC (_hWnd, hdc);

	pmis->itemWidth  = _MenuStyle.ItemHeight + strSize.cx;
	if (pmis->itemWidth<120)
		pmis->itemWidth   =120;
	pmis->itemHeight = _MenuStyle.ItemHeight;

	UINT state = GetMenuState(pmi->hMenu, pmi->uItem, MF_BYPOSITION);
	if ((state & MF_SEPARATOR) == MF_SEPARATOR)
	{
		if ((state & MF_POPUP) != MF_POPUP)
		{
			pmis->itemHeight = _MenuStyle.SeparatorHeight;
		}
	}
}

void CMenuUI::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT pdis   = (LPDRAWITEMSTRUCT) lParam;
	if (pdis->CtlType != ODT_MENU)
	{
		return;
	}

	LPMENUITEM pmi = (LPMENUITEM) pdis->itemData;
	HDC        hdc = pdis->hDC;

	SetBkMode(hdc, TRANSPARENT);

	UINT state = GetMenuState(pmi->hMenu, pmi->uItem, MF_BYPOSITION);
	if ((state & MF_POPUP) == MF_POPUP)
	{
		state &= ~MF_SEPARATOR;
	}


	HBRUSH hBrush = CreateSolidBrush(_MenuStyle.ColorBackground);
	FillRect(hdc, &pdis->rcItem, hBrush);
	DeleteObject(hBrush);

	BOOL Seleted = FALSE;

	RECT ImageBgRect = pdis->rcItem;	
	ImageBgRect.right=ImageBgRect.left+_MenuStyle.ItemHeight ;
	CRenderEngine::DrawGradient(hdc,ImageBgRect,RGB(255,255,255),RGB(233,231,215),false,4);

	//鼠标滑过
	if ((pdis->itemState & ODS_SELECTED) == ODS_SELECTED)
	{
		Seleted = TRUE;	
		if ((state & MF_DISABLED) != MF_DISABLED)
		{
			CRenderEngine::DrawGradient(hdc,pdis->rcItem,RGB(233,245,255),RGB(255,233,245),true,16);
		}

		CRenderEngine::DrawRect(hdc,pdis->rcItem,1,RGB(203,231,215));
	}

	RECT txtRect = pdis->rcItem;	
	txtRect.left=txtRect.left+_MenuStyle.ItemHeight+9 ;

	TCHAR Buffer[MENU_TEXT_LENGTH];
	GetMenuString(pmi->hMenu, pmi->uItem, Buffer,  MENU_TEXT_LENGTH, MF_BYPOSITION);

	if ((state & MF_CHECKED) == MF_CHECKED)
	{
		BOOL Grayed = FALSE;
		if ((state & MF_GRAYED) == MF_GRAYED)
		{
			Grayed = TRUE;
		}
		DrawChecked(hdc, pdis->rcItem, pmi, Seleted, Grayed);
		state &= ~MF_CHECKED;
	}

	if ((pmi->hID != -1) && ((state & MF_SEPARATOR) == MF_SEPARATOR))
	{
		DrawSeparator(hdc, pdis->rcItem);
	}	
	else if ((state & MF_DISABLED) == MF_DISABLED)
	{	
		CRenderEngine::DrawText(hdc,_pManager,txtRect,(LPCTSTR)Buffer, RGB(200,197,200),-1, DT_SINGLELINE|DT_VCENTER);
	}
	else if ((state & MF_GRAYED) == MF_GRAYED)
	{
	}
	else
	{
		CRenderEngine::DrawText(hdc,_pManager,txtRect,(LPCTSTR)Buffer, RGB(0,0,0), -1, DT_SINGLELINE|DT_VCENTER);
	}
}


void CMenuUI::DrawSeparator(HDC hdc, RECT rect)
{
	RECT a=rect;
	a.right=rect.left+_MenuStyle.ItemHeight;

	RECT line={rect.left+_MenuStyle.ItemHeight+6,(rect.bottom +rect.top) / 2,rect.right,(rect.bottom+rect.top) / 2};
	CRenderEngine::DrawLine(hdc,line,1,RGB(200,197,200));
}


void CMenuUI::DrawImage(HDC hdc, RECT rect, LPMENUITEM pmi)
{
	RECT ImageRect={rect.left+3,rect.top+((rect.bottom-rect.top)-16)/2,rect.left+3+16,rect.top+((rect.bottom-rect.top)-16)/2+16};

	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask  = MIIM_CHECKMARKS;
	GetMenuItemInfo(pmi->hMenu, pmi->uItem, TRUE, &mii);

	if (mii.hbmpUnchecked == NULL)
	{
		//	return;
	}

	HDC hmemDC = NULL;
	hmemDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hmemDC, mii.hbmpUnchecked);
	BitBlt(hdc, ImageRect.left, ImageRect.top,
		ImageRect.right - ImageRect.left,
		ImageRect.bottom - ImageRect.top, hmemDC, 0, 0, SRCCOPY);
	DeleteDC(hmemDC);
}

void CMenuUI::DrawChecked(HDC hdc, RECT rect, LPMENUITEM pmi,
			  BOOL Seleted, BOOL Grayed)
{
	int x,y;
	x=y=4;
	rect.right=rect.left+_MenuStyle.ItemHeight;
	::InflateRect(&rect, -(rect.right-rect.left-x)/2,- (rect.bottom-rect.top-y)/2);

	return;
}
