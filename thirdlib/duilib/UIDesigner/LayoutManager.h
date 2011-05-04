#pragma once
#include "..\DuiLib\UIlib.h"
using DuiLib::CContainerUI;
using DuiLib::CPaintManagerUI;
using DuiLib::CWindowWnd;
using DuiLib::TNotifyUI;

#define EXPORT_UI_SCRIPT

#if defined(EXPORT_UI_SCRIPT)
#include "tinyxml.h"
#endif

//////////////////////////////////////////////////////////////////////////
//CFormUI

class CFormUI : public CContainerUI
{
public:
	CFormUI();
	virtual ~CFormUI();

public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetManager(CPaintManagerUI* pPaintManager);
	CPaintManagerUI* GetManager() const;

	SIZE GetInitSize();
	void SetInitSize(int cx, int cy);
	RECT& GetSizeBox();
	void SetSizeBox(RECT& rcSizeBox);
	RECT& GetCaptionRect();
	void SetCaptionRect(RECT& rcCaption);
	SIZE GetRoundCorner() const;
	void SetRoundCorner(int cx, int cy);
	SIZE GetMinInfo() const;
	SIZE GetMaxInfo() const;
	void SetMinInfo(int cx, int cy);
	void SetMaxInfo(int cx, int cy);
	bool IsShowUpdateRect() const;
	void SetShowUpdateRect(bool show);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetPos(RECT rc);

private:
	CPaintManagerUI* m_pManager;
};

//////////////////////////////////////////////////////////////////////////
//CFormTestWnd

class CFormTestWnd :  public CWindowWnd, public INotifyUI
{
public:
	CFormTestWnd();
	~CFormTestWnd();
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);

public:
	void SetManager(CPaintManagerUI* pPaintManager);
	CPaintManagerUI* GetManager() const;
	void SetRoot(CControlUI* pControl);

	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CPaintManagerUI* m_pManager;
	CControlUI* m_pRoot;
};

//////////////////////////////////////////////////////////////////////////
//CDelayResize

class CDelayRepos
{
public:
	CDelayRepos();
	~CDelayRepos();

public:
	BOOL AddParent(CControlUI* pControl);
	void Repos();

private:
	CArray<CControlUI*,CControlUI*> m_arrDelay;
};

//////////////////////////////////////////////////////////////////////////
//CLayoutManager

typedef struct tagDefaultUINameCount{
	int nControlCount;
	int nButtonCount;
	int nEditCount;
	int nLabelCount;
	int nTextCount;
	int nOptionCount;
	int nComboCount;
	int nListCount;
	int nSliderCount;
	int nProgressCount;
	int nActiveXCount;
	int nContainerCount;
	int nVerticalLayoutCount;
	int nHorizontalLayoutCount;
	int nDialogLayoutCount;
	int nTabLayoutCount;
	int nTileLayoutCount;
	int nListHeaderCount;
	int nListHeaderItemCount;
	int nListBodyCount;
	int nListTextElementCount;
	int nListLabelElementCount;
	int nListExpandElementCount;
	int nListContainerElementCount;
}DefaultUINameCount;

class CLayoutManager
{
public:
	CLayoutManager(void);
	virtual ~CLayoutManager(void);

public:
	void Init(HWND hWnd,LPCTSTR pstrLoad);

	void Draw(CDC* pDC);
	void DrawAuxBorder(CDC* pDC,CControlUI* pControl);
	void DrawGrid(CDC* pDC,CRect& rect);

	CControlUI* NewUI(int nClass,CRect& rect,CControlUI* pParent);
	BOOL RemoveUI(CControlUI* pControl);
	CPaintManagerUI* GetManager();
	inline CFormUI* GetForm() const;
	CControlUI* FindControl(CPoint point) const;

	void TestForm();
	BOOL IsEmptyForm() const;
	void AlignLeft(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignRight(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignTop(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignBottom(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignCenterVertically(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignCenterHorizontally(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignHorizontal(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignVertical(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignSameWidth(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignSameHeight(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignSameSize(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void ShowGrid(bool bShow);
	bool IsShowGrid() const;
	void ShowAuxBorder(bool bShow);
	bool IsShowAuxBorder() const;
	void MicoMoveUp(CArray<CControlUI*,CControlUI*>& arrSelected,int nMoved);
	void MicoMoveDown(CArray<CControlUI*,CControlUI*>& arrSelected,int nMoved);
	void MicoMoveLeft(CArray<CControlUI*,CControlUI*>& arrSelected,int nMoved);
	void MicoMoveRight(CArray<CControlUI*,CControlUI*>& arrSelected,int nMoved);

	void SaveSkinFile(LPCTSTR lpszPathName);
	static void SaveProperties(CControlUI* pControl, TiXmlElement* pParentNode
		, BOOL bSaveChildren = TRUE);

	void SetDefaultUIName(CControlUI* pControl, BOOL bForce = FALSE);

protected:
	CControlUI* CopyControls(CControlUI* pControl);
	CControlUI* CopyControl(CControlUI* pControl);

	static void SaveControlProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveLabelProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveButtonProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveOptionProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveProgressProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveSliderProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveEditProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveScrollBarProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveListProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveComboProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveListHeaderItemProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveListElementProperty(CControlUI* pControl, TiXmlElement* pNode);
	static void SaveContainerProperty(CControlUI* pControl, TiXmlElement* pNode);

private:
	CPaintManagerUI m_Manager;

	bool m_bShowGrid;
	bool m_bShowAuxBorder;

	DefaultUINameCount m_UINameCount;
};