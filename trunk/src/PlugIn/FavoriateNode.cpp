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

// 在当前结点下增加一个子结点
void	FavoriateNode::AddChild( FavoriateNode*	pChildNode)
{
	if(pChildNode == NULL)
	{
		ASSERT(0);
		return;
	}

	
	//	检查当前是否已经存在子结点
	if( m_pFirstChildNode == NULL)
	{
		m_pFirstChildNode	=	pChildNode;
		pChildNode->m_pParentNode	=	this;

		//	如果当前结点是URL子结点

		return;
	}

	//	不是第一个子结点，则检查是否是目录，如果是目录，则插入头部
	//	如果是普通的URL，则插入到尾部
	if( NODE_IS_DIRECTORY(pChildNode->m_pFavoriateData))
	{
		pChildNode->m_pNextSiblingNode	=	m_pFirstChildNode;
		pChildNode->m_pParentNode	=	this;

		m_pFirstChildNode->m_pPrevSiblingNode	=	pChildNode;
		m_pFirstChildNode	=	pChildNode;

		return;
	}

	//	如果是普通的URL
}


//	从当前结点下删除子结点
void	FavoriateNode::RemoveChild(FavoriateNode*	pChildNode)
{

}

//	删除所有的子结点
void	FavoriateNode::RemoveAllChild()
{

}
