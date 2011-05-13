#pragma once

#include "FavoriateNode.h"
#include <map>

using namespace std;

class FavoriateTree
{
public:
	FavoriateTree();
	virtual ~FavoriateTree();

	//	从收藏夹数据行中构造数据树
	void Add(PFAVORITELINEDATA	pFavoriteLineData, int nNum);

private:

	void SortFavoriateData(PFAVORITELINEDATA pFavoriteLineData, int nNum);

public:

	//	根结点
	FavoriateNode*	m_pRootNode;

protected:
	

	//	目录结点的映射，用来快速的查找到对应的目录
	std::map<int32, FavoriateNode*>	m_mapDirNode;
};