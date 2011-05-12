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

	//	在当前结点下增加一个子结点
	void	AddChild( FavoriateNode*	pChildNode);
	
	//	从当前结点下删除子结点
	void	RemoveChild(FavoriateNode*	pChildNode);

	//	删除所有的子结点
	void	RemoveAllChild();


private:

	// 保存合并后的收藏夹，指向根结点
	FavoriateNode*	m_pNextSiblingNode;			//	与当前结点同级的下一个结点
	FavoriateNode*	m_pPrevSiblingNode;			//	当前结点的前一个结点
	FavoriateNode*	m_pParentNode;				//	指向父结点

	FavoriateNode*	m_pFirstChildNode;			//	指向当前结点的第一个子结点
	FavoriateNode*	m_pFirstUrlNode;			//	指向子结点中第一个非目录结点。
												//	在所有俄子结点中，目录结点总是排在前面，然后才是子结点

	FAVORITELINEDATA*	m_pFavoriateData;		//	收藏夹的结点数据	

};