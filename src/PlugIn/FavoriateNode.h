#pragma  once

#include "stdafx.h"
#include "PlugIn.h"


#define NODE_IS_DIRECTORY(node)	(node->bFolder== true)
#define NODE_IS_URL(node)	(node->bFolder == false)

class FavoriateNode
{
public:
	FavoriateNode();
	virtual ~FavoriateNode();


	//	�ڵ�ǰ���������һ���ӽ��
	void AddChild( FavoriateNode*	pChildNode);

	//	�ڵ�ǰ���������һ���ӽ��
	void AddChild( FAVORITELINEDATA*	pChildData);
	
	//	�ӵ�ǰ�����ɾ���ӽ��
	void RemoveChild(FavoriateNode*	pChildNode);

	//	ɾ�����е��ӽ��
	void RemoveAllChild();


public:

	// ����ϲ�����ղؼУ�ָ������
	FavoriateNode*	m_pNextSiblingNode;			//	�뵱ǰ���ͬ������һ�����
	FavoriateNode*	m_pPrevSiblingNode;			//	��ǰ����ǰһ�����
	FavoriateNode*	m_pParentNode;				//	ָ�򸸽��

	FavoriateNode*	m_pFirstChildNode;			//	ָ��ǰ���ĵ�һ���ӽ��
	FavoriateNode*	m_pFirstUrlNode;			//	ָ���ӽ���е�һ����Ŀ¼��㡣
												//	�����е��ӽ���У�Ŀ¼�����������ǰ�棬Ȼ������ӽ��

	int	m_nNodeId;								//	��ǰ����ID

	FAVORITELINEDATA*	m_pFavoriateData;		//	�ղؼеĽ������	
};