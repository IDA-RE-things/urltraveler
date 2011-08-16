// App.cpp : Defines the entry point for the application.
//
#pragma  once

#include "stdafx.h"
#include <objbase.h>
#include "UIlib.h"
#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "TreeList.h"
#include "resource.h"
#include "PlugIn.h"
#include <map>
#include <vector>
#include "TipWnd.h"

using namespace std;
using namespace DuiLib;

extern HMODULE	g_hModule;

class CMainFrameWnd : public CWindowWnd, public INotifyUI, public IListCallbackUI
{
public:
	CMainFrameWnd();
	~CMainFrameWnd();
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Init(){} ;
	void OnPrepare(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)  ;
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) ;
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 更新指定网站的收藏夹图标
	void	GetAvailableBrowser();
	void	LoadFavoriteTree(FAVORITELINEDATA**	ppFavoriteData, int nNum);
	void	UpdateFavoriteIcon(wchar_t*	pszUrl, HICON hIcon);
	void	DeleteFavorite(int nDeleteNodeId);
	void	DeleteFavoriteFold(int nIndex);
	void	AddUrl();
	void	OpenUrl(int nIndex);
	void	CopyUrl(int nIndex);
	void	SelectTreeList(int nId);
	void	AddUrlSuccess(PFAVORITELINEDATA pData);
	PFAVORITELINEDATA*	GetFavoriteLineData(int& nFavoriteNum);

private:

	//	各种消息响应处理函数
	void	OnTabChange(TNotifyUI& msg);
	void	OnSystemMenu(TNotifyUI& msg);
	void	OnBtnClose(TNotifyUI& msg);
	void	OnFavoriteTreeListItemClick(TNotifyUI& msg);
	void	OnFavoriteListItemHover(TNotifyUI& msg);
	void	OnFavoriteListItemUnHover(TNotifyUI& msg);
	void	OnShowMenu(TNotifyUI& msg);
	void	OnFavoriteListItemEditFinished(TNotifyUI& msg);
	void	OnFavoriteListItemDelete(TNotifyUI& msg);
	void	OnFavoriteListItemMoved(TNotifyUI& msg);
	void	OnItemReturnKeyDown(TNotifyUI& msg);

	//	列表菜单项响应函数
	void	OnAdd();
	void	OnDelete();
	void	OnOpen();
	void	OnCopyUrl();
	void	OnEdit();



protected:
	bool GetWebSiteFavIcon(wstring strUrl, int nRow);
	void	ShowFavoriteTreeList(int nId);
	void	SetFavoriteNumText(int nNum);

public:
	CPaintManagerUI m_pm;

private:

	TreeListUI* m_pFavoriteTree;
	int m_nCurrentFavoriteFoldId;				//	当前显示的收藏夹目录的 ID

	CTipWnd*            m_pTipWnd;

	// 保存某个目录结点下的所有的叶子结点
	std::vector<FAVORITELINEDATA*>		m_vFavoriteNodeAtTreeNode;
};
