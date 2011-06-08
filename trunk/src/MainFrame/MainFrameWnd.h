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
#include "LoginFrameWnd.h"
#include "PlugIn.h"
#include <map>
#include <vector>
#include "TipWnd.h"
#include "Menu.h"

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
	void LoadFavoriteTree(FAVORITELINEDATA*	pFavoriteData, int nNum);
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
	void	UpdateFavoriteIcon(wchar_t*	pszUrl, HICON hIcon);

protected:
	bool GetWebSiteFavIcon(wstring strUrl, int nRow);

public:
	CPaintManagerUI m_pm;

private:
	CLoginFrameWnd* m_pLoginFrame;
	std::map<int, TreeListUI::Node*>	m_mapIdNode;	//	id到node的关联关系
	std::map<TreeListUI::Node*, int>	m_mapNodeId;	//	node到map的映射

	int m_nFavoriteNum;
	FAVORITELINEDATA*	m_pFavoriteData;
	CTipWnd*            m_pTipWnd;

	// 保存某个目录结点下的所有的叶子结点
	std::vector<FAVORITELINEDATA*>		m_vFavoriteNode;
};
