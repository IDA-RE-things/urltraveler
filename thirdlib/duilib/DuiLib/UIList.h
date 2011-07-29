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
		CStdString sImage;
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
		bool	bMultiSelect;	// �Ƿ�֧�ֶ�ѡ

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
		virtual bool SelectItem(int iIndex) = 0;
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
		int GetCurSel() const;

		bool	IsItemSelected(int iIndex);
		bool SelectItem(int iIndex);
		bool UnSelectItem(int iIndex);
		bool SelectContinualItem(int iIndex);

		// ����������nIndex��ȡ��ѡ��
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

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl);
		void RemoveAll();

		//�б�ͷ����
		bool AddHeader(CListHeaderUI* pHeader);
		bool RemoveHeader();
		bool	AddHeaderItem(CListHeaderItemUI* pHeaderItem);
		bool	AddHeaderItemAt(CListHeaderItemUI* pHeaderItem, int iIndex);

		// ���Ӻ�ɾ���б��е�Item��
		bool AddItem(CListElementUI* pItem);
		bool AddItemAt(CListElementUI* pItem, int iIndex);
		bool RemoveItem(CListElementUI* pListItem);
		bool RemoveItemAt(int iIndex);

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

		// ����ɾ������Item�ӿ� [6/2/2011 linjinming]
		void RemoveAllItems();
		CListElementUI* GetSubItem(int nIndex);

		void ShowEditText(LPCTSTR pstrText, CRect rc, int nRow, int nColomn);
		void HideEditText();
		bool EditItem(int nX, int nY);
		CStdString GetEditText();

	public:
		void Notify(TNotifyUI& msg);
		virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	protected:
		void	OnNotifyReturn(TNotifyUI& msg);
		void	OnNotifyItemDelete(TNotifyUI& msg);
		void	OnNotifyItemSelectAll(TNotifyUI& msg);

		void	OnEventItemClick(TEventUI& event);
		void	OnEventItemClickUp(TEventUI& event);
		void	OnEventItemRightClick(TEventUI& event);
		void	OnEventKeyDown(TEventUI& event);

	protected:
		int	m_iCurSel;
		int	m_iLastClickSel;				//	���һ�ε����Item
		int	m_iLastSel;						//	���һ�α�ѡ�е�Item�����������Ǳ�
		std::vector<int>	m_vCurSel;	//	��ǰ���б�ѡ�е��б���

		bool	m_bScrollSelect;
		int	m_iExpandedItem;

		IListCallbackUI* m_pCallback;
		CListBodyUI* m_pList;
		CListHeaderUI* m_pHeader;
		TListInfoUI m_ListInfo;
		CEditUI*	m_pEditUI;			//	�༭�������Ը������б༭

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
	//					�б���

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

		void Invalidate(); // ֱ��CControl::Invalidate�ᵼ�¹�����ˢ�£���д����ˢ������
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
	//									�����List�е�Item���б༭
	//		˫��Item����Ը�Item���б༭


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

		void Invalidate(); // ֱ��CControl::Invalidate�ᵼ�¹�����ˢ�£���д����ˢ������
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
