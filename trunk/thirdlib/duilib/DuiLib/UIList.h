#ifndef __UILIST_H__
#define __UILIST_H__

#pragma once

#include <vector>
using namespace std;

namespace DuiLib 
{
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CListHeaderUI;

#define UILIST_MAX_COLUMNS 32

	typedef struct tagTListInfoUI
	{
		int nColumns;
		RECT rcColumn[UILIST_MAX_COLUMNS];
		int nFont;
		UINT uTextStyle;
		RECT rcTextPadding;
		DWORD dwTextColor;
		DWORD dwBkColor;
		CStdString sBkImage;
		bool bAlternateBk;
		DWORD dwSelectedTextColor;
		DWORD dwSelectedBkColor;
		CStdString sSelectedImage;
		DWORD dwHotTextColor;
		DWORD dwHotBkColor;
		CStdString sHotImage;
		DWORD dwDisabledTextColor;
		DWORD dwDisabledBkColor;
		CStdString sDisabledImage;
		DWORD dwLineColor;
		bool bShowHtml;
		bool bMultiExpandable;
		bool	bMultiSelect;	// 是否支持多选

	} TListInfoUI;


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class IListCallbackUI
	{
	public:
		virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem) = 0;
	};

	class IListOwnerUI
	{
	public:
		virtual TListInfoUI* GetListInfo() = 0;
		virtual int GetCurSel() const = 0;
		virtual int* GetCurSel(int& nSelNum) const = 0;
		virtual bool SelectItem(int iIndex, bool bTakeFocus = false) = 0;
		virtual void DoEvent(TEventUI& event) = 0;
	};

	class IListUI : public IListOwnerUI
	{
	public:
		virtual CListHeaderUI* GetHeader() const = 0;
		virtual CContainerUI* GetList() const = 0;
		virtual IListCallbackUI* GetTextCallback() const = 0;
		virtual void SetTextCallback(IListCallbackUI* pCallback) = 0;
		virtual bool ExpandItem(int iIndex, bool bExpand = true) = 0;
		virtual int GetExpandedItem() const = 0;
		virtual void ShowEditText(LPCTSTR pstrText, CRect rc, int nRow, int nColomn) = 0;
		virtual void HideEditText() = 0;
	};

	class IListItemUI
	{
	public:
		virtual int GetIndex() const = 0;
		virtual void SetIndex(int iIndex) = 0;
		virtual IListOwnerUI* GetOwner() = 0;
		virtual void SetOwner(CControlUI* pOwner) = 0;
		virtual bool IsSelected() const = 0;
		virtual bool Select(bool bSelect = true) = 0;
		virtual bool IsExpanded() const = 0;
		virtual bool Expand(bool bExpand = true) = 0;
		virtual void DrawItemText(HDC hDC, const RECT& rcItem) = 0;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class CListBodyUI;
	class CListHeaderUI;
	class CListHeaderItemUI;
	class CListElementUI;

	class UILIB_API CListUI : public CVerticalLayoutUI, public IListUI, public INotifyUI
	{
	public:
		CListUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool GetScrollSelect();
		void SetScrollSelect(bool bScrollSelect);
		int	GetCurSel() const;
		int* GetCurSel(int& nSelNum) const;
		int	GetHotItem() const;

		bool	IsItemSelected(int iIndex);
		bool SelectItem(int iIndex, bool bTakeFocus = false);
		bool	SelectItem(CListElementUI* pElement);
		bool UnSelectItem(int iIndex);
		bool UnSelectItem(CListElementUI* pElement);
		bool SelectContinualItem(int iIndex);

		// 给定索引的nIndex不取消选择
		void	ClearSelectedItem(int nIndex = -1);
		void	DeleteSelected();

		CListHeaderUI* GetHeader() const;  
		CContainerUI* GetList() const;
		TListInfoUI* GetListInfo();

		CControlUI* GetItemAt(int iIndex) const;
		int GetItemIndex(CControlUI* pControl) const;
		bool SetItemIndex(CControlUI* pControl, int iIndex);
		int GetRowCount() const;
		int GetColumnCount() const;

		//列表头操作
		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);

		bool AddHeader(CListHeaderUI* pHeader);
		bool	AddHeaderItem(CListHeaderItemUI* pHeaderItem);
		bool	AddHeaderItemAt(CListHeaderItemUI* pHeaderItem, int iIndex);
		bool AddItem(CListElementUI* pItem);
		bool AddItemAt(CListElementUI* pItem, int iIndex);

		// 增加和删除列表中的Item项
		bool Remove(CControlUI* pControl);
		bool RemoveHeader();
		bool RemoveItem(CListElementUI* pListItem);
		bool RemoveItemAt(int iIndex);
		void RemoveAllItems();

		void EnsureVisible(int iIndex);
		void Scroll(int dx, int dy);

		int GetChildPadding() const;
		void SetChildPadding(int iPadding);

		void SetItemFont(int index);
		void SetItemTextStyle(UINT uStyle);
		void SetItemTextPadding(RECT rc);
		void SetItemTextColor(DWORD dwTextColor);
		void SetItemBkColor(DWORD dwBkColor);
		void SetItemImage(LPCTSTR pStrImage);
		void SetSelectedItemTextColor(DWORD dwTextColor);
		void SetSelectedItemBkColor(DWORD dwBkColor);
		void SetSelectedItemImage(LPCTSTR pStrImage); 
		void SetHotItemTextColor(DWORD dwTextColor);
		void SetHotItemBkColor(DWORD dwBkColor);
		void SetHotItemImage(LPCTSTR pStrImage);
		void SetDisabledItemTextColor(DWORD dwTextColor);
		void SetDisabledItemBkColor(DWORD dwBkColor);
		void SetDisabledItemImage(LPCTSTR pStrImage);
		void SetItemLineColor(DWORD dwLineColor);
		bool IsItemShowHtml();
		void SetItemShowHtml(bool bShowHtml = true);

		bool	IsItemMultiSelect();
		void SetItemMultiSelect(bool bMultiSelect = true);

		RECT GetItemTextPadding() const;
		DWORD GetItemTextColor() const;
		DWORD GetItemBkColor() const;
		DWORD GetItemTextStyle() const;
		LPCTSTR GetItemImage() const;
		DWORD GetSelectedItemTextColor() const;
		DWORD GetSelectedItemBkColor() const;
		LPCTSTR GetSelectedItemImage() const;
		DWORD GetHotItemTextColor() const;
		DWORD GetHotItemBkColor() const;
		LPCTSTR GetHotItemImage() const;
		DWORD GetDisabledItemTextColor() const;
		DWORD GetDisabledItemBkColor() const;
		LPCTSTR GetDisabledItemImage() const;
		DWORD GetItemLineColor() const;

		void SetMultiExpanding(bool bMultiExpandable); 
		int GetExpandedItem() const;
		bool ExpandItem(int iIndex, bool bExpand = true);

		void SetPos(RECT rc);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		IListCallbackUI* GetTextCallback() const;
		void SetTextCallback(IListCallbackUI* pCallback);

		SIZE GetScrollPos() const;
		SIZE GetScrollRange() const;
		void SetScrollPos(SIZE szPos);
		void LineUp();
		void LineDown();
		void PageUp();
		void PageDown();
		void HomeUp();
		void EndDown();
		void LineLeft();
		void LineRight();
		void PageLeft();
		void PageRight();
		void HomeLeft();
		void EndRight();
		void	KeyUp();
		void	KeyDown();
		void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false);
		virtual CScrollBarUI* GetVerticalScrollBar() const;
		virtual CScrollBarUI* GetHorizontalScrollBar() const;

		// 增加删除所有Item接口 [6/2/2011 linjinming]
		CListElementUI* GetSubItem(int nIndex);

		void	ShowEdit(int nRow, int nColumn);
		void ShowEditText(LPCTSTR pstrText, CRect rc, int nRow, int nColomn);
		void HideEditText();
		bool EditItem(int nX, int nY);
		CStdString GetEditText();
		LPCTSTR	GetItemText(int nRow, int nColumn);

	public:
		void Notify(TNotifyUI& msg);
		virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

		void	OnNotifyReturn(TNotifyUI& msg);
		void	OnNotifyItemSelectAll(TNotifyUI& msg);

		void	OnEventItemClick(TEventUI& event);
		void	OnEventItemClickUp(TEventUI& event);
		void	OnEventItemRightClick(TEventUI& event);
		void	OnEventKeyDown(TEventUI& event);

	public:

		int	m_iHotIndex;					//	处于Hot状态的Item。Hot状态的只有一个，与选中状态完全不同
		int	m_iLastClickSel;				//	最后一次点击的Item
		int	m_iLastSel;					//	最后一次被选中的Item，而不仅仅是被
		std::vector<int>	m_vCurSel;		//	当前所有被选中的列表项

		bool	m_bScrollSelect;
		int	m_iExpandedItem;

		IListCallbackUI* m_pCallback;
		CListBodyUI* m_pList;
		CListHeaderUI* m_pHeader;
		TListInfoUI m_ListInfo;
		CEditUI*	m_pEditUI;			//	编辑框，用来对给定的列编辑

		bool		m_bAddNotifyer;
		bool		m_bShowEdit;
		int		m_nEditRow;
		int		m_nEditColomn;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListBodyUI : public CVerticalLayoutUI
	{
	public:
		CListBodyUI(CListUI* pOwner);
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetScrollPos(SIZE szPos);
		void SetPos(RECT rc);
		void DoEvent(TEventUI& event);

	protected:
		CListUI* m_pOwner;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListHeaderUI : public CHorizontalLayoutUI
	{
	public:
		CListHeaderUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		SIZE EstimateSize(SIZE szAvailable);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListHeaderItemUI : public CControlUI
	{
	public:
		CListHeaderItemUI();
		~CListHeaderItemUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetEnabled(bool bEnable = true);

		bool IsDragable() const;
		void SetDragable(bool bDragable);
		DWORD GetSepWidth() const;
		void SetSepWidth(int iWidth);
		DWORD GetTextStyle() const;
		void SetTextStyle(UINT uStyle);
		DWORD GetTextColor() const;
		void SetTextColor(DWORD dwTextColor);
		void SetTextPadding(RECT rc);
		RECT GetTextPadding() const;
		void SetFont(int index);
		bool IsShowHtml();
		void SetShowHtml(bool bShowHtml = true);
		LPCTSTR GetNormalImage() const;
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage() const;
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetPushedImage() const;
		void SetPushedImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage() const;
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetSepImage() const;
		void SetSepImage(LPCTSTR pStrImage);

		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		RECT GetThumbRect() const;

		void PaintText(HDC hDC);
		void PaintStatusImage(HDC hDC);

	protected:
		POINT ptLastMouse;
		bool m_bDragable;
		UINT m_uButtonState;
		int m_iSepWidth;
		DWORD m_dwTextColor;
		int m_iFont;
		UINT m_uTextStyle;
		bool m_bShowHtml;
		RECT m_rcTextPadding;
		CStdString m_sNormalImage;
		CStdString m_sHotImage;
		CStdString m_sPushedImage;
		CStdString m_sFocusedImage;
		CStdString m_sSepImage;
		CStdString m_sSepImageModify;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//					列表项

	class UILIB_API CListElementUI : public CControlUI, public IListItemUI
	{
	public:
		CListElementUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetEnabled(bool bEnable = true);

		int GetIndex() const;
		void SetIndex(int iIndex);

		IListOwnerUI* GetOwner();
		void SetOwner(CControlUI* pOwner);
		void SetVisible(bool bVisible = true);

		bool IsSelected() const;
		bool Select(bool bSelect = true);
		bool IsExpanded() const;
		bool Expand(bool bExpand = true);

		void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
		bool Activate();

		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void DrawItemBk(HDC hDC, const RECT& rcItem);

	protected:
		int m_iIndex;
		bool m_bSelected;
		UINT m_uButtonState;
		IListOwnerUI* m_pOwner;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListLabelElementUI : public CListElementUI
	{
	public:
		CListLabelElementUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);
		void DoPaint(HDC hDC, const RECT& rcPaint);

		void DrawItemText(HDC hDC, const RECT& rcItem);
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListTextElementUI : public CListLabelElementUI
	{
	public:
		CListTextElementUI();
		~CListTextElementUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		LPCTSTR GetText(int iIndex) const;
		void SetText(int iIndex, LPCTSTR pstrText);

		void SetOwner(CControlUI* pOwner);
		CStdString* GetLinkContent(int iIndex);

		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);

		void DrawItemText(HDC hDC, const RECT& rcItem);

	protected:
		enum { MAX_LINK = 8 };
		int m_nLinks;
		RECT m_rcLinks[MAX_LINK];
		CStdString m_sLinks[MAX_LINK];
		int m_nHoverLink;
		IListUI* m_pOwner;
		CStdPtrArray m_aTexts;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//									允许对List中的Item进行编辑
	//		双击Item，则对该Item进行编辑


	class UILIB_API CListTextEditElementUI : public CListTextElementUI
	{
	public:
		CListTextEditElementUI();
		~CListTextEditElementUI();

		LPCTSTR GetClass() const;
		LPVOID  GetInterface(LPCTSTR pstrName);
		BOOL    SetColomnEditable(int nColomnIndex, bool bEditable);
		BOOL    IsColomnEditable(int nColomn);

		void DoEvent(TEventUI& event);

	protected:
		bool*       m_bColomnEditable;
	};



	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListExpandElementUI : public CListTextElementUI
	{
	public:
		CListExpandElementUI();
		virtual ~CListExpandElementUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		void SetMouseEnabled(bool bEnable = true);

		void SetPos(RECT rc);
		void DoEvent(TEventUI& event);
		SIZE EstimateSize(SIZE szAvailable);
		void DoPaint(HDC hDC, const RECT& rcPaint);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
		CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

		bool IsExpanded() const;
		bool Expand(bool bExpand = true);
		bool GetExpandHideSelf() const;
		void SetExpandHideSelf(bool bHideSelf);
		RECT GetExpanderRect() const;
		void SetExpanderRect(RECT rc);
		void SetExpandItem(CControlUI* pControl);

		void DrawItemText(HDC hDC, const RECT& rcItem);

	protected:
		bool m_bExpanded;
		bool m_bHideSelf;
		RECT m_rcExpander;
		CControlUI* m_pSubControl;
	};


	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CListContainerElementUI : public CContainerUI, public IListItemUI
	{
	public:
		CListContainerElementUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		int GetIndex() const;
		void SetIndex(int iIndex);

		IListOwnerUI* GetOwner();
		void SetOwner(CControlUI* pOwner);
		void SetVisible(bool bVisible = true);
		void SetEnabled(bool bEnable = true);

		bool IsSelected() const;
		bool Select(bool bSelect = true);
		bool IsExpanded() const;
		bool Expand(bool bExpand = true);

		void Invalidate(); // 直接CControl::Invalidate会导致滚动条刷新，重写减少刷新区域
		bool Activate();

		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoPaint(HDC hDC, const RECT& rcPaint);

		void DrawItemText(HDC hDC, const RECT& rcItem);    
		void DrawItemBk(HDC hDC, const RECT& rcItem);

	protected:
		int m_iIndex;
		bool m_bSelected;
		UINT m_uButtonState;
		IListOwnerUI* m_pOwner;
	};

} // namespace DuiLib

#endif // __UILIST_H__
