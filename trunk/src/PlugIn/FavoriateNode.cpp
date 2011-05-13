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

	m_nNodeId	=	0;
}

FavoriateNode::~FavoriateNode()
{

}


// 在当前结点下增加一个子结点
void FavoriateNode::AddChild( FavoriateNode*	pChildNode)
{
	if(pChildNode == NULL)
	{
		ASSERT(0);
		return;
	}

	pChildNode->m_pParentNode	=	this;

	//	检查当前是否已经存在子结点
	if( m_pFirstChildNode == NULL)
	{
		m_pFirstChildNode	=	pChildNode;

		// 如果结点是URL结点
		if( NODE_IS_URL(pChildNode->m_pFavoriateData))
		{
			m_pFirstUrlNode	=	pChildNode;
		}

		return;
	}

	//	如果是普通的URL结点，则直接插入作为第一个URL结点
	if( NODE_IS_URL(pChildNode->m_pFavoriateData))
	{
		// m_pFirstUrlNode与m_pFistChildNode相等，意味着第一个结点肯定是叶子结点
		if( m_pFirstUrlNode	==	 m_pFirstChildNode)
		{
			pChildNode->m_pNextSiblingNode	=	m_pFirstUrlNode;
			m_pFirstUrlNode->m_pPrevSiblingNode	=	pChildNode;

			m_pFirstChildNode	=	pChildNode;
		}
		else
		{	
			// 检查m_pFirstUrlNode是否为空
			if( m_pFirstChildNode != NULL)
			{
				// 设置Next指针
				pChildNode->m_pNextSiblingNode	=	m_pFirstUrlNode;
				m_pFirstUrlNode->m_pPrevSiblingNode->m_pNextSiblingNode	=	pChildNode;

				// 设置Prev指针
				pChildNode->m_pPrevSiblingNode	=	m_pFirstUrlNode->m_pPrevSiblingNode;
				m_pFirstUrlNode->m_pPrevSiblingNode	=	pChildNode;
			}
			else
			{
				// 遍历到m_pFistChildNode的末尾
				FavoriateNode*	pLastDirectoryNode = m_pFirstChildNode;
				while( pLastDirectoryNode->m_pNextSiblingNode != NULL)
					pLastDirectoryNode = pLastDirectoryNode->m_pNextSiblingNode;

				pLastDirectoryNode->m_pNextSiblingNode	=	pChildNode;
				pChildNode->m_pPrevSiblingNode	=	pLastDirectoryNode;
			}

		}

		m_pFirstUrlNode		=	pChildNode;

		return;
	}


	//	不是第一个子结点，则检查是否是目录，如果是目录，则插入头部
	//	如果是普通的URL，则插入到尾部
	if( NODE_IS_DIRECTORY(pChildNode->m_pFavoriateData))
	{
		pChildNode->m_pNextSiblingNode	=	m_pFirstChildNode;
		m_pFirstChildNode->m_pPrevSiblingNode	=	pChildNode;
		m_pFirstChildNode	=	pChildNode;

		return;
	}
}

//	在当前结点下增加一个子结点
void FavoriateNode::AddChild( FAVORITELINEDATA*	pChildData)
{
	FavoriateNode* pNode = new FavoriateNode();
	pNode->m_pFavoriateData	=	pChildData;
	pNode->m_nNodeId	=	pChildData->nPid;

	AddChild(pNode);
}


//	从当前结点下删除子结点
void FavoriateNode::RemoveChild(FavoriateNode*	pChildNode)
{
	if( pChildNode == NULL)
	{
		ASSERT(0);
		return;
	}


}

//	删除所有的子结点
void FavoriateNode::RemoveAllChild()
{

}
