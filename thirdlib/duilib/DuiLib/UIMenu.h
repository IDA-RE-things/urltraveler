#pragma once

/////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////
//                                                      //
//     CMenuUI �Ի�˵�(sdk)                             //
//     ����ȫ������sdkд���ģ�����ֱ��ʹ�ã�������ı�  //
// ��ǰ�ĳ���                                         //
//                                                      //
//     ʹ�÷�����                                       //
//     1��CMenuUI * pmex = new CMenuUI(hwnd);             //
//     2����WndProc�����                               //
//        pmex->GetMessage(message, wParam, lParam);    //
//     ����������˲˵����Ի棬һ�еĹ����������Զ���� //
//     ��Ȼ�����ڲ˵�״̬�Ĳ���ԭ�����������ڻ��������� //
//                                                      //
//     �Ǻ�~������д�ĵ�һ����Դ���룬�кܶ಻����ĵ�  //
// ������ָ�̡�������ֻ��sdk����������mfc�ܷ�ʹ�ø��� //
// �����λ�Լ������˹�~                                //
//                                                      //
//                             ��� fhao840911@126.com  //
//                                  2007.7.18 �ɶ�      //
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

	void DrawChecked(HDC, RECT, LPMENUITEM, BOOL, BOOL);//���Ʋ˵�Checked״̬
	void DrawSeparator(HDC, RECT);//���Ʋ˵�Separator״̬
	void DrawImage(HDC, RECT, LPMENUITEM);


	HWND               _hWnd;
	HMENU              _hMenu;
	MENUSTYLE          _MenuStyle;

	CStdPtrArray		_pmi;
	CPaintManagerUI* _pManager;
};

}