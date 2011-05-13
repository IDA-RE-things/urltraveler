#pragma once

#include "FavoriateNode.h"
#include <map>

using namespace std;

class FavoriateTree
{
public:
	FavoriateTree();
	virtual ~FavoriateTree();

	//	���ղؼ��������й���������
	void Add(PFAVORITELINEDATA	pFavoriteLineData, int nNum);

private:

	void SortFavoriateData(PFAVORITELINEDATA pFavoriteLineData, int nNum);

public:

	//	�����
	FavoriateNode*	m_pRootNode;

protected:
	

	//	Ŀ¼����ӳ�䣬�������ٵĲ��ҵ���Ӧ��Ŀ¼
	std::map<int32, FavoriateNode*>	m_mapDirNode;
};