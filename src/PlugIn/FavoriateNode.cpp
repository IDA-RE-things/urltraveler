#include "stdafx.h"
#include "FavoriateNode.h"
#include "SndaBase.h"

FavoriateNode::FavoriateNode()
{
	m_pParentNode	=	NULL;
	m_pNextSiblingNode	=	NULL;
	m_pPrevSiblingNode	=	NULL;

	m_pFavoriateData	=	NULL;

	m_pFirstChildNode	=	NULL;
	m_pFirstUrlNode		=	NULL;
}

FavoriateNode::~FavoriateNode()
{

}

// �ڵ�ǰ���������һ���ӽ��
void	FavoriateNode::AddChild( FavoriateNode*	pChildNode)
{
	if(pChildNode == NULL)
	{
		ASSERT(0);
		return;
	}

	
	//	��鵱ǰ�Ƿ��Ѿ������ӽ��
	if( m_pFirstChildNode == NULL)
	{
		m_pFirstChildNode	=	pChildNode;
		pChildNode->m_pParentNode	=	this;

		//	�����ǰ�����URL�ӽ��

		return;
	}

	//	���ǵ�һ���ӽ�㣬�����Ƿ���Ŀ¼�������Ŀ¼�������ͷ��
	//	�������ͨ��URL������뵽β��
	if( NODE_IS_DIRECTORY(pChildNode->m_pFavoriateData))
	{
		pChildNode->m_pNextSiblingNode	=	m_pFirstChildNode;
		pChildNode->m_pParentNode	=	this;

		m_pFirstChildNode->m_pPrevSiblingNode	=	pChildNode;
		m_pFirstChildNode	=	pChildNode;

		return;
	}

	//	�������ͨ��URL
}


//	�ӵ�ǰ�����ɾ���ӽ��
void	FavoriateNode::RemoveChild(FavoriateNode*	pChildNode)
{

}

//	ɾ�����е��ӽ��
void	FavoriateNode::RemoveAllChild()
{

}
