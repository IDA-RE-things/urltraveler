#include "stdafx.h"
#include "SndaBase.h"
#include "IEPlugInFactory.h"

namespace ieplugin
{
	IEPlugInFactory*	g_IEPlugInFactory	=	NULL;
}

// 导出借口实现
IPlugInFactory*	GetPlugInFactory()
{
	if( g_IEPlugInFactory == NULL)
	{
		g_IEPlugInFactory = new IEPlugInFactory();
		ASSERT( g_IEPlugInFactory != NULL);
	}

	return g_IEPlugInFactory;
}

void	ReleasePlugInFactory()
{
	if( g_IEPlugInFactory  != NULL)
	{
		delete g_IEPlugInFactory;
		g_IEPlugInFactory = NULL;
	}
}


IEPlugInFactory::IEPlugInFactory()
{

}

IEPlugInFactory::~IEPlugInFactory()
{
}