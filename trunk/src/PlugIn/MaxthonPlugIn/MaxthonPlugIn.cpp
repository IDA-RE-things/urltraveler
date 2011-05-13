#include "stdafx.h"
#include "MaxthonPlugIn.h"
#include <string>
#include "Decoder.h"

namespace maxthonplugin
{
	CMaxthonPlugIn*	g_MaxthonPlugIn	=	NULL;
}

// 导出借口实现
IPlugIn*	GetPlugIn()
{
	if( g_MaxthonPlugIn == NULL)
	{
		g_MaxthonPlugIn = new CMaxthonPlugIn();
		ASSERT( g_MaxthonPlugIn != NULL);
	}

	return g_MaxthonPlugIn;
}

void	ReleasePlugIn( IPlugIn* p)
{
	ASSERT( g_MaxthonPlugIn == p);
	if( g_MaxthonPlugIn  != NULL)
	{
		delete g_MaxthonPlugIn;
		g_MaxthonPlugIn = NULL;
	}
}



CMaxthonPlugIn::CMaxthonPlugIn()
{
	std::string strDecodeContent;
	decode("C:\\Users\\linjinming.SNDA\\AppData\\Roaming\\Maxthon3\\Users\\guest\\Favorite\\Favorite.dat", strDecodeContent);
	int i = 0;

}

CMaxthonPlugIn::~CMaxthonPlugIn()
{

}
