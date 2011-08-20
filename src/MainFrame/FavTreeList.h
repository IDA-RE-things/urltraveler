#ifndef __TREELISTUI_H__
#define __TREELISTUI_H__

#include <vector>
#include <math.h>
#include "UIList.h"
#include <map>
#include "CallbackEx.h"
#include "DragList.h"
#include "UITreeList.h"

extern HMODULE	g_hModule;

using namespace DuiLib;
using namespace std;

class CFavoriteTreeListUI : public CTreeListUI
{
public:

	CFavoriteTreeListUI();
	~CFavoriteTreeListUI(){}

	LPCTSTR GetClass() const	 ;

	void Notify(TNotifyUI& msg);

	int GetIndexFromId(int nId);
	int GetIdFromIndex(int nIndex);
	int GetIdFromNode(CTreeListUI::Node* pNode);
	CTreeListUI::Node* GetNodeFromId(int nId);

	// 为第nIndex个结点增加一个子结点，内容为pszTitle
	bool	Add(int nIndex, int nId,LPCTSTR pszTitle);
	bool RemoveAt(int nIndex);

	void	OnEventItemClick(TEventUI& event);
	void	OnEventDragOver(TEventUI& event);
	void	OnEventTreeListItemDragEnd();
	void	OnEventTreeListItemDragOver();
	void	OnEventListItemDragEnd(CDragListUI* pDragList);
	void DoEvent(TEventUI& event) ;

public:

	// 当前选中的TreeNode结点
	int m_nTreeNodeId;
	CTreeListUI::Node*	m_pCurrentTreeNode;

	std::map<int, CTreeListUI::Node*>	m_mapIdNode;	//	id到node的关联关系
	std::map<CTreeListUI::Node*, int>	m_mapNodeId;	//	node到map的映射

protected:

	bool	m_bIsDragging;
};

#endif __TREELISTUI_H__
