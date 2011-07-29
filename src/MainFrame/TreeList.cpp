#include <stdafx.h>
#include "TreeList.h"

bool TreeListUI::Add(CControlUI* pControl)
{
	if( !pControl ) return false;
	if( _tcscmp(pControl->GetClass(), _T("ListLabelElementUI")) == 0 ) return false;
	return CListUI::Add(pControl);
}

bool TreeListUI::AddAt(CControlUI* pControl, int iIndex)
{
	if( !pControl ) return false;
	if( _tcscmp(pControl->GetClass(), _T("ListLabelElementUI")) == 0 ) return false;
	return CListUI::AddAt(pControl, iIndex);
}

bool TreeListUI::Remove(CControlUI* pControl)
{
	if( !pControl ) return false;
	if( _tcscmp(pControl->GetClass(), _T("ListLabelElementUI")) == 0 ) return false;
	return CListUI::Remove(pControl);
}

bool TreeListUI::RemoveAt(int iIndex)
{
	CControlUI* pControl = m_pList->GetItemAt(iIndex);
	if( !pControl ) 
		return false;

	if( _tcscmp(pControl->GetClass(), _T("ListLabelElementUI")) == 0 ) 
		return false;

	return CListUI::RemoveAt(iIndex);
}

void TreeListUI::RemoveAll()
{
	CListUI::RemoveAll();
	delete _root;
	_root = new TreeListUI::Node;
	_root->data()._level = -1;
	_root->data()._child_visible = true;
	_root->data()._pListElement = NULL;
}

void TreeListUI::DoEvent(TEventUI& event) 
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CVerticalLayoutUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_TIMER && event.wParam == SCROLL_TIMERID )
	{
		if( m_dwDelayLeft > 0 ) {
			--m_dwDelayLeft;
			SIZE sz = GetScrollPos();
			LONG lDeltaY =  (LONG)(CalculateDelay((double)m_dwDelayLeft / m_dwDelayNum) * m_dwDelayDeltaY);
			if( (lDeltaY > 0 && sz.cy != 0)  || (lDeltaY < 0 && sz.cy != GetScrollRange().cy ) ) {
				sz.cy -= lDeltaY;
				SetScrollPos(sz);
				return;
			}
		}
		m_dwDelayDeltaY = 0;
		m_dwDelayNum = 0;
		m_dwDelayLeft = 0;
		m_pManager->KillTimer(this, SCROLL_TIMERID);
		return;
	}
	if( event.Type == UIEVENT_SCROLLWHEEL )
	{
		LONG lDeltaY = 0;
		if( m_dwDelayNum > 0 ) lDeltaY =  (LONG)(CalculateDelay((double)m_dwDelayLeft / m_dwDelayNum) * m_dwDelayDeltaY);
		switch( LOWORD(event.wParam) ) 
		{
			case SB_LINEUP:
				if( m_dwDelayDeltaY >= 0 ) m_dwDelayDeltaY = lDeltaY + 8;
				else m_dwDelayDeltaY = lDeltaY + 12;
				break;
			case SB_LINEDOWN:
				if( m_dwDelayDeltaY <= 0 ) m_dwDelayDeltaY = lDeltaY - 8;
				else m_dwDelayDeltaY = lDeltaY - 12;
				break;
		}
		if( m_dwDelayDeltaY > 100 ) m_dwDelayDeltaY = 100;
		else if( m_dwDelayDeltaY < -100 ) m_dwDelayDeltaY = -100;
		m_dwDelayNum = (DWORD)sqrt((double)abs(m_dwDelayDeltaY)) * 5;
		m_dwDelayLeft = m_dwDelayNum;
		m_pManager->SetTimer(this, SCROLL_TIMERID, 50U);
		return;
	}

	CListUI::DoEvent(event);
}

TreeListUI::Node* TreeListUI::AddNode(LPCTSTR text, TreeListUI::Node* parent)
{
	if( !parent ) parent = _root;

	CListLabelElementUI* pListElement = new CListLabelElementUI;
	TreeListUI::Node* node = new TreeListUI::Node;
	node->data()._level = parent->data()._level + 1;
	if( node->data()._level == 0 ) node->data()._child_visible = true;
	else node->data()._child_visible = false;
	node->data()._text = text;
	node->data()._pListElement = pListElement;

	if( !parent->data()._child_visible )
	{
		pListElement->SetVisible(false);
	}
	if( parent != _root && !parent->data()._pListElement->IsVisible() )
	{
		pListElement->SetVisible(false);
	}

	CStdString html_text;
	html_text += _T("<x 6>");
	for( int i = 0; i < node->data()._level; ++i )
	{
		html_text += _T("<x 24>");
	}
	if( node->data()._level < 3 ) {
		if( node->data()._child_visible ) html_text += _T("<a><i tree_expand.png 2 1></a>");
		else html_text += _T("<a><i tree_expand.png 2 0></a>");
	}
	html_text += node->data()._text;
	pListElement->SetText(html_text);
	//if( node->data()._level == 0 ) pListElement->SetFixedHeight(28);
	//else pListElement->SetFixedHeight(24);
	pListElement->SetTag((UINT_PTR)node);
	if( node->data()._level == 0 ) {
		pListElement->SetBkImage(_T("file='tree_top.png' corner='2,1,2,1' fade='100'"));
	}

	int index = 0;
	if( parent->has_children() )
	{
		TreeListUI::Node* prev = parent->get_last_child();
		index = prev->data()._pListElement->GetIndex() + 1;
	}
	else 
	{
		if( parent == _root ) index = 0;
		else index = parent->data()._pListElement->GetIndex() + 1;
	}
	if( !CListUI::AddAt(pListElement, index) ) {
		delete pListElement;
		delete node;
		node = NULL;
	}

	parent->add_child(node);
	return node;
}

void TreeListUI::RemoveNode(TreeListUI::Node* node)
{
	if( !node || node == _root ) return;

	//需要倒扫删除，不然会崩溃  [6/11/2011 linjinming]
	int nChildNum = node->num_children();
	for( int i = nChildNum - 1; i >= 0; --i )
	{
		TreeListUI::Node* child = node->child(i);
		RemoveNode(child);
	}

	CListUI::Remove(node->data()._pListElement);
	node->parent()->remove_child(node);
	delete node;
}

void TreeListUI::SetChildVisible(TreeListUI::Node* node, bool visible)
{
	if( !node || node == _root ) return;

	if( node->data()._child_visible == visible ) return;
	node->data()._child_visible = visible;

	CStdString html_text;
	html_text += _T("<x 6>");
	for( int i = 0; i < node->data()._level; ++i )
	{
		html_text += _T("<x 24>");
	}
	if( node->data()._level < 3 ) {
		if( node->data()._child_visible ) html_text += _T("<a><i tree_expand.png 2 1></a>");
		else html_text += _T("<a><i tree_expand.png 2 0></a>");
	}
	html_text += node->data()._text;
	node->data()._pListElement->SetText(html_text);

	if( !node->data()._pListElement->IsVisible() ) return;
	if( !node->has_children() ) return;

	TreeListUI::Node* begin = node->child(0);
	TreeListUI::Node* end = node->get_last_child();
	for( int i = begin->data()._pListElement->GetIndex(); i <= end->data()._pListElement->GetIndex(); ++i )
	{
		CControlUI* control = m_pList->GetItemAt(i);
		if( _tcscmp(control->GetClass(), _T("ListLabelElementUI")) == 0 )
		{
			if( !visible ) 
			{
				control->SetVisible(false);
			}
			else
			{
				TreeListUI::Node* local_parent = ((TreeListUI::Node*)control->GetTag())->parent();
				if( local_parent->data()._child_visible && local_parent->data()._pListElement->IsVisible() )
				{
					control->SetVisible(true);
				}
			}
		}
	}
}

SIZE TreeListUI::GetExpanderSizeX(TreeListUI::Node* node) const
{
	if( !node || node == _root ) return CSize();
	if( node->data()._level >= 3 ) return CSize();

	SIZE szExpander = {0};
	szExpander.cx = 6 + 24 * node->data()._level - 4/*适当放大一点*/;
	szExpander.cy = szExpander.cx + 16 + 8/*适当放大一点*/;
	return szExpander;
}