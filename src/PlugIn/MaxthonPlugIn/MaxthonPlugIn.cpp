#include "stdafx.h"
#include "MaxthonPlugIn.h"

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
	int i = 0;

}

CMaxthonPlugIn::~CMaxthonPlugIn()
{

}
