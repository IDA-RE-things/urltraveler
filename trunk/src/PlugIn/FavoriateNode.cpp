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


// �ڵ�ǰ���������һ���ӽ��
void FavoriateNode::AddChild( FavoriateNode*	pChildNode)
{
	if(pChildNode == NULL)
	{
		ASSERT(0);
		return;
	}

	pChildNode->m_pParentNode	=	this;

	//	��鵱ǰ�Ƿ��Ѿ������ӽ��
	if( m_pFirstChildNode == NULL)
	{
		m_pFirstChildNode	=	pChildNode;

		// ��������URL���
		if( NODE_IS_URL(pChildNode->m_pFavoriateData))
		{
			m_pFirstUrlNode	=	pChildNode;
		}

		return;
	}

	//	�������ͨ��URL��㣬��ֱ�Ӳ�����Ϊ��һ��URL���
	if( NODE_IS_URL(pChildNode->m_pFavoriateData))
	{
		// m_pFirstUrlNode��m_pFistChildNode��ȣ���ζ�ŵ�һ�����϶���Ҷ�ӽ��
		if( m_pFirstUrlNode	==	 m_pFirstChildNode)
		{
			pChildNode->m_pNextSiblingNode	=	m_pFirstUrlNode;
			m_pFirstUrlNode->m_pPrevSiblingNode	=	pChildNode;

			m_pFirstChildNode	=	pChildNode;
		}
		else
		{	
			// ���m_pFirstUrlNode�Ƿ�Ϊ��
			if( m_pFirstChildNode != NULL)
			{
				// ����Nextָ��
				pChildNode->m_pNextSiblingNode	=	m_pFirstUrlNode;
				m_pFirstUrlNode->m_pPrevSiblingNode->m_pNextSiblingNode	=	pChildNode;

				// ����Prevָ��
				pChildNode->m_pPrevSiblingNode	=	m_pFirstUrlNode->m_pPrevSiblingNode;
				m_pFirstUrlNode->m_pPrevSiblingNode	=	pChildNode;
			}
			else
			{
				// ������m_pFistChildNode��ĩβ
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


	//	���ǵ�һ���ӽ�㣬�����Ƿ���Ŀ¼�������Ŀ¼�������ͷ��
	//	�������ͨ��URL������뵽β��
	if( NODE_IS_DIRECTORY(pChildNode->m_pFavoriateData))
	{
		pChildNode->m_pNextSiblingNode	=	m_pFirstChildNode;
		m_pFirstChildNode->m_pPrevSiblingNode	=	pChildNode;
		m_pFirstChildNode	=	pChildNode;

		return;
	}
}

//	�ڵ�ǰ���������һ���ӽ��
void FavoriateNode::AddChild( FAVORITELINEDATA*	pChildData)
{
	FavoriateNode* pNode = new FavoriateNode();
	pNode->m_pFavoriateData	=	pChildData;
	pNode->m_nNodeId	=	pChildData->nPid;

	AddChild(pNode);
}


//	�ӵ�ǰ�����ɾ���ӽ��
void FavoriateNode::RemoveChild(FavoriateNode*	pChildNode)
{
	if( pChildNode == NULL)
	{
		ASSERT(0);
		return;
	}


}

//	ɾ�����е��ӽ��
void FavoriateNode::RemoveAllChild()
{

}