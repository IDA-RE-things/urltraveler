#include "stdafx.h"
#include "MaxthonPlugInFactory.h"
#include "SndaBase.h"

namespace maxthonplugin
{
	MaxthonPlugInFactory*	g_MaxthonPlugInFactory	=	NULL;
}

// 导出借口实现
IPlugInFactory*	GetPlugInFactory()
{
	if( g_MaxthonPlugInFactory == NULL)
	{
		g_MaxthonPlugInFactory = new MaxthonPlugInFactory();
		ASSERT( g_MaxthonPlugInFactory != NULL);
	}

	return g_MaxthonPlugInFactory;
}

void	ReleasePlugInFactory()
{
	if( g_MaxthonPlugInFactory  != NULL)
	{
		delete g_MaxthonPlugInFactory;
		g_MaxthonPlugInFactory = NULL;
	}
}


MaxthonPlugInFactory::MaxthonPlugInFactory()
{

}

MaxthonPlugInFactory::~MaxthonPlugInFactory()
{

}

BOOL MaxthonPlugInFactory::QueryPlugInCounter(uint32 & counter)
{
	counter = 2;
	return TRUE;
}

BOOL MaxthonPlugInFactory::QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
{
	PAssert_ReturnWithValue(2 == counter, false);

	IPlugIn** ptrPlugIn = &pPlugIn;
	ptrPlugIn[0] = (IPlugIn *)&m_Maxthon2PlugIn;
	ptrPlugIn[1] = (IPlugIn *)&m_Maxthon3PlugIn;

	return  true;
}

void MaxthonPlugInFactory::ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
{
}