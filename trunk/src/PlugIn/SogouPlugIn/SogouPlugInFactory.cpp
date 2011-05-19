#include "stdafx.h"
#include "SogouPlugInFactory.h"
#include "SndaBase.h"

namespace sogouplugin
{
	SogouPlugInFactory*	g_SogouPlugInFactory	=	NULL;
}

// 导出借口实现
IPlugInFactory*	GetPlugInFactory()
{
	if( g_SogouPlugInFactory == NULL)
	{
		g_SogouPlugInFactory = new SogouPlugInFactory();
		ASSERT( g_SogouPlugInFactory != NULL);
	}

	return g_SogouPlugInFactory;
}

void	ReleasePlugInFactory()
{
	if( g_SogouPlugInFactory  != NULL)
	{
		delete g_SogouPlugInFactory;
		g_SogouPlugInFactory = NULL;
	}
}



SogouPlugInFactory::SogouPlugInFactory()
{

}

SogouPlugInFactory::~SogouPlugInFactory()
{

}

BOOL SogouPlugInFactory::QueryPlugInCounter(uint32 & counter)
{
	counter = 1;
	return TRUE;
}

BOOL SogouPlugInFactory::QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
{
	PAssert_ReturnWithValue(1 == counter, false);

	IPlugIn** ptrPlugIn = &pPlugIn;
	ptrPlugIn[0] = (IPlugIn *)&m_SogouPlugIn;

	return  true;
}

void SogouPlugInFactory::ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
{
}