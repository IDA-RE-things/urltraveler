#include "stdafx.h"
#include "FavoriateTree.h"

FavoriateTree::FavoriateTree()
{
	m_pRootNode	=	NULL;
	m_mapDirNode.clear();
}

FavoriateTree::~FavoriateTree()
{
}

void FavoriateTree::SortFavoriateData(PFAVORITELINEDATA pFavoriteLineData, int nNum)
{
	// 首先对pFavoriteLineData进行排序
	FAVORITELINEDATA*	pSortLineData = new FAVORITELINEDATA[nNum];
	memset(pSortLineData, 0x0, sizeof(FAVORITELINEDATA) * nNum);

	FAVORITELINEDATA*	pSortLineDataPos = pSortLineData;

	// 逐一找到合适的数据，并插入到pSortLineData中去
	int nParentId	=	0;
	for( int i=0; i<nNum; i++)
	{
		// 在未排序的数据中查找Parent为nParentId的数据
		for(int j=0; j<nNum; j++)
		{
			if( pFavoriteLineData[j].nPid	==	nParentId)
			{
				memcpy(pSortLineDataPos, &pFavoriteLineData[j], sizeof(FAVORITELINEDATA));
				pSortLineDataPos++;
			}
		}

		nParentId	=	pSortLineData[i].nId;
	}

	// 排序后的数据拷贝
	memcpy(pFavoriteLineData, pSortLineData, nNum*sizeof( FAVORITELINEDATA));
	delete[] pSortLineData;
}


//	从收藏夹数据行中构造数据树
void FavoriateTree::Add(PFAVORITELINEDATA	pFavoriteLineData, int nNum)
{
	//	第一个搜藏夹树，直接生成Tree即可
	if( m_pRootNode == NULL)
	{
		//	设置根结点
		m_pRootNode	=	 new FavoriateNode();
		m_pRootNode->m_nNodeId	=	0;
		m_mapDirNode[0]	=	m_pRootNode;	//	根结点

		// 首先对pFavoriteLineData进行排序
		SortFavoriateData(pFavoriteLineData, nNum);

		// pSortLineData已经进过排序，按照层次进行排序
		// 当前结点指向pSortLineData中的第一个结点，作为根结点存在
		for( int i=0; i<nNum; i++)
		{
			FAVORITELINEDATA* pData = &pFavoriteLineData[i];

			int nParentId = pData[i].nPid;

			std::map<int, FavoriateNode*>::iterator itr = m_mapDirNode.find(nParentId);
			if( itr != m_mapDirNode.end())
			{
				FavoriateNode* pParentNode = itr->second;
				pParentNode->AddChild(pData);
			}
		}

		return;
	}



	
}