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

	// ����ָ����վ���ղؼ�ͼ��
	void	GetAvailableBrowser();
	void	LoadFavoriteTree(FAVORITELINEDATA*	pFavoriteData, int nNum);
	void	UpdateFavoriteIcon(wchar_t*	pszUrl, HICON hIcon);
	void	DeleteFavorite(int nDeleteNodeId);
	void	DeleteFavoriteFold(int nIndex);
	void	AddUrl();
	void	OpenUrl(int nIndex);
	void	CopyUrl(int nIndex);
	void	SelectTreeList(int nId);

protected:
	bool GetWebSiteFavIcon(wstring strUrl, int nRow);
	void	ShowFavoriteTreeList(int nId);

public:
	CPaintManagerUI m_pm;

private:
	CLoginFrameWnd* m_pLoginFrame;
	std::map<int, TreeListUI::Node*>	m_mapIdNode;	//	id��node�Ĺ�����ϵ
	std::map<TreeListUI::Node*, int>	m_mapNodeId;	//	node��map��ӳ��

	// ��ǰѡ�е�TreeNode���
	int m_nTreeNodeId;
	TreeListUI::Node*	m_pCurrentTreeNode;

	int m_nFavoriteNum;
	FAVORITELINEDATA*	m_pFavoriteData;

	CTipWnd*            m_pTipWnd;

	// ����ĳ��Ŀ¼����µ����е�Ҷ�ӽ��
	std::vector<FAVORITELINEDATA*>		m_vFavoriteNodeAtTreeNode;
};
