#ifndef __TREELISTUI_H__
#define __TREELISTUI_H__

#include <vector>
#include <math.h>
#include "UIList.h"
#include <map>
#include "CallbackEx.h"
#include "DragList.h"

extern HMODULE	g_hModule;

using namespace DuiLib;
using namespace std;

inline double CalculateDelay(double state) {
	return pow(state, 2);
}

class TreeListUI : public CListUI
{
public:
	enum { SCROLL_TIMERID = 10 };

	struct NodeData
	{
		int _level;
		bool _child_visible;
		CStdString _text;
		CListLabelElementUI* _pListElement;
	};

	class Node
	{
		typedef std::vector <Node*>	Children;
		Children	_children;
		Node*		_parent;
		NodeData    _data;

	private:
		void set_parent(Node* parent) { _parent = parent; }

	public:
		Node() : _parent (NULL) {}
		explicit Node(NodeData t) : _data (t), _parent (NULL) {}
		Node(NodeData t, Node* parent)	: _data (t), _parent (parent) {}
		~Node() 
		{
			for (int i = 0; i < num_children(); i++)
				delete _children[i]; 
		}
		NodeData& data() { return _data; }	
		int num_children() const { return _children.size(); }
		Node* child(int i)	{ return _children[i]; }
		Node* parent() { return ( _parent);	}
		bool has_children() const {	return num_children() > 0; }
		void add_child(Node* child) 
		{
			child->set_parent(this); 
			_children.push_back(child); 
		}
		void remove_child(Node* child)
		{
			Children::iterator iter = _children.begin();
			for( ; iter < _children.end(); ++iter )
			{
				if( *iter == child ) 
				{
					_children.erase(iter);
					return;
				}
			}
		}
		Node* get_last_child()
		{
			if( has_children() )
			{
				return child(num_children() - 1)->get_last_child();
			}
			else return this;
		}
	};	

	TreeListUI() : _root(NULL), m_dwDelayDeltaY(0), m_dwDelayNum(0), m_dwDelayLeft(0)
	{
		SetItemShowHtml(true);

		_root = new Node;
		_root->data()._level = -1;
		_root->data()._child_visible = true;
		_root->data()._pListElement = NULL;

		m_bShowEdit	=	false;
		m_ListInfo.bMultiSelect = false;
		m_bAddNotifyer = false;
		m_bIsDragging = false;
	}

	~TreeListUI() { if(_root) delete _root; }

	LPCTSTR GetClass() const	 ;

	void Notify(TNotifyUI& msg);
	virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);

	// 删除指定索引的Item
	bool RemoveAt(int nIndex);
	void RemoveAll()  ;


	void DoEvent(TEventUI& event) ;

	Node* GetRoot() { return _root; }
	Node* AddNode(LPCTSTR text, Node* parent = NULL);
	void RemoveNode(Node* node);

	void SetChildVisible(Node* node, bool visible);
	SIZE GetExpanderSizeX(Node* node) const  ;

	void	OnEventItemClick(TEventUI& event);
	void	OnEventDragOver(TEventUI& event);

	void	OnTreeListItemDragEnd();
	void	OnListItemDragEnd(DragListUI* pDragList);

public:
	// 当前选中的TreeNode结点
	int m_nTreeNodeId;
	TreeListUI::Node*	m_pCurrentTreeNode;

	std::map<int, TreeListUI::Node*>	m_mapIdNode;	//	id到node的关联关系
	std::map<TreeListUI::Node*, int>	m_mapNodeId;	//	node到map的映射

protected:
	Node* _root;

	LONG m_dwDelayDeltaY;
	DWORD m_dwDelayNum;
	DWORD m_dwDelayLeft;
	bool	m_bAddNotifyer;

	bool	m_bIsDragging;
};

#endif __TREELISTUI_H__
