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
	// ���ȶ�pFavoriteLineData��������
	FAVORITELINEDATA*	pSortLineData = new FAVORITELINEDATA[nNum];
	memset(pSortLineData, 0x0, sizeof(FAVORITELINEDATA) * nNum);

	FAVORITELINEDATA*	pSortLineDataPos = pSortLineData;

	// ��һ�ҵ����ʵ����ݣ������뵽pSortLineData��ȥ
	int nParentId	=	0;
	for( int i=0; i<nNum; i++)
	{
		// ��δ����������в���ParentΪnParentId������
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

	// ���������ݿ���
	memcpy(pFavoriteLineData, pSortLineData, nNum*sizeof( FAVORITELINEDATA));
	delete[] pSortLineData;
}


//	���ղؼ��������й���������
void FavoriateTree::Add(PFAVORITELINEDATA	pFavoriteLineData, int nNum)
{
	//	��һ���Ѳؼ�����ֱ������Tree����
	if( m_pRootNode == NULL)
	{
		//	���ø����
		m_pRootNode	=	 new FavoriateNode();
		m_pRootNode->m_nNodeId	=	0;
		m_mapDirNode[0]	=	m_pRootNode;	//	�����

		// ���ȶ�pFavoriteLineData��������
		SortFavoriateData(pFavoriteLineData, nNum);

		// pSortLineData�Ѿ��������򣬰��ղ�ν�������
		// ��ǰ���ָ��pSortLineData�еĵ�һ����㣬��Ϊ��������
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