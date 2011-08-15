#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////
//                                                      //
//     CMenuUI 自绘菜单(sdk)                             //
//     该类全部采用sdk写出的，可以直接使用，而不需改变  //
// 以前的程序。                                         //
//                                                      //
//     使用方法：                                       //
//     1、CMenuUI * pmex = new CMenuUI(hwnd);             //
//     2、在WndProc里加入                               //
//        pmex->GetMessage(message, wParam, lParam);    //
//     这样就完成了菜单的自绘，一切的工作都又类自动完成 //
//     当然，对于菜单状态的操作原来怎样，现在还是怎样。 //
//                                                      //
//     呵呵~这是我写的第一个开源代码，有很多不成熟的地  //
// 方请多多指教。我现在只做sdk开发，至于mfc能否使用该类 //
// 就请各位自己试试了哈~                                //
//                                                      //
//                             冯浩 fhao840911@126.com  //
//                                  2007.7.18 成都      //
//                                                      //
//////////////////////////////////////////////////////////
#pragma  once


#include "StdAfx.h"
#include <windows.h>
#include <vector>
#include "UILib.h"

using namespace std;

#if defined(UNICODE)
#define lstrchr wcschr
#else
#define lstrchr strchr
#endif

#define MENU_TEXT_LENGTH    100
#define MENU_TEXT_GRAYED1   0xffffff
#define MENU_TEXT_GRAYED2   0x7f7f7f

#define MSTYLE_HEIGHT       0X0001
#define MSTYLE_SEPARATOR    MSTYLE_HEIGHT << 1
#define MSTYLE_TEXT         MSTYLE_SEPARATOR << 1
#define MSTYLE_HIGHTLIGHT   MSTYLE_TEXT << 1
#define MSTYLE_BACKGROUND   MSTYLE_HIGHTLIGHT << 1
#define MSTYLE_SELECTED     MSTYLE_BACKGROUND << 1

typedef struct tagMenuStyle
{
	UINT     fMask;
	UINT     ItemHeight;
	UINT     SeparatorHeight;
	COLORREF ColorText;
	COLORREF ColorHighLight;
	COLORREF ColorBackground;
	COLORREF ColorTextSelected;

}MENUSTYLE, * LPMENUSTYLE;

typedef struct tagMenuItem
{
	HMENU hMenu;
	UINT  uItem;
	UINT  hID;

}MENUITEM, * LPMENUITEM;

namespace DuiLib {

class UILIB_API CMenuUI
{
public:
	CMenuUI(HWND,CPaintManagerUI* pManager);

	void AppendMenu(UINT uFlags,UINT uIDNewItem,LPCTSTR lpNewItem);
	virtual void Init() ;

	~CMenuUI();
	void GetMessage(UINT, WPARAM, LPARAM);
	void OnMeasureItem(WPARAM, LPARAM);
	void OnDrawItem(WPARAM, LPARAM);

	void SetMenuStyle(MENUSTYLE);
	void Show(POINT pt,UINT uFlags=TPM_LEFTALIGN);

protected:
	void SetSubModify(HMENU);
	void DefaultMenuStyle();

	void DrawChecked(HDC, RECT, LPMENUITEM, BOOL, BOOL);//绘制菜单Checked状态
	void DrawSeparator(HDC, RECT);//绘制菜单Separator状态
	void DrawImage(HDC, RECT, LPMENUITEM);


	HWND               _hWnd;
	HMENU              _hMenu;
	MENUSTYLE          _MenuStyle;

	CStdPtrArray		_pmi;
	CPaintManagerUI* _pManager;
};

}