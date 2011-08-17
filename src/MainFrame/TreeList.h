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

class CTreeListUI : public CListUI
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
		int get_level() { return _data._level;}
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

	CTreeListUI() : _root(NULL), m_dwDelayDeltaY(0), m_dwDelayNum(0), m_dwDelayLeft(0)
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

	~CTreeListUI() { if(_root) delete _root; }

	LPCTSTR GetClass() const	 ;

	void Notify(TNotifyUI& msg);
	virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	int GetIndexFromId(int nId);
	int GetIdFromIndex(int nIndex);

	bool Add(CControlUI* pControl);
	// 为第nIndex个结点增加一个子结点，内容为pszTitle
	bool	Add(int nIndex, LPCTSTR pszTitle);
	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl);
	bool RemoveAt(int nIndex);
	void RemoveAll()  ;

	Node* GetRoot() { return _root; }
	Node* AddNode(LPCTSTR text, Node* parent = NULL);
	void RemoveNode(Node* node);

	void SetChildVisible(Node* node, bool visible);
	SIZE GetExpanderSizeX(Node* node) const  ;

	void	OnEventItemClick(TEventUI& event);
	void	OnEventDragOver(TEventUI& event);

	void	OnTreeListItemDragEnd();
	void	OnTreeListItemDragOver();
	void	OnListItemDragEnd(CDragListUI* pDragList);

	void DoEvent(TEventUI& event) ;

public:
	// 当前选中的TreeNode结点
	int m_nTreeNodeId;
	CTreeListUI::Node*	m_pCurrentTreeNode;

	std::map<int, CTreeListUI::Node*>	m_mapIdNode;	//	id到node的关联关系
	std::map<CTreeListUI::Node*, int>	m_mapNodeId;	//	node到map的映射

protected:
	Node* _root;

	LONG m_dwDelayDeltaY;
	DWORD m_dwDelayNum;
	DWORD m_dwDelayLeft;
	bool	m_bAddNotifyer;

	bool	m_bIsDragging;
};

#endif __TREELISTUI_H__
