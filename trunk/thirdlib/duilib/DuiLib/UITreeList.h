#pragma once

#include <vector>
#include <math.h>
#include "UIlib.h"

inline double CalculateDelay(double state) {
	return pow(state, 2);
}

namespace DuiLib
{
	class UILIB_API CTreeListUI : public CListUI
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
			Node() : _parent (NULL) 
			{
				_data._child_visible = false;
				_data._level = -1;
				_data._text = L"";
				_data._pListElement = NULL;
			}

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

		CTreeListUI();
		~CTreeListUI() { if(_root) delete _root; }

		virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex) ;

		bool Remove(CControlUI* pControl);
		bool RemoveAt(int iIndex);
		void RemoveAllItems();

		Node* GetRoot() { return _root; }
		Node* AddNode(LPCTSTR text, Node* parent = NULL);
		bool RemoveNode(Node* node);

		void SetChildVisible(Node* node, bool visible)  ;
		SIZE GetExpanderSizeX(Node* node) const;

		void OnEventItemClick(TEventUI& event);
		void DoEvent(TEventUI& event);

	protected:

		Node* _root;

		LONG m_dwDelayDeltaY;
		DWORD m_dwDelayNum;
		DWORD m_dwDelayLeft;
	};

}