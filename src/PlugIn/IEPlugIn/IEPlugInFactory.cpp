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

BOOL IEPlugInFactory::QueryPlugInCounter(uint32 & counter)
{
	counter = 1;
	return TRUE;
}

BOOL IEPlugInFactory::QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
{
	PAssert_ReturnWithValue(1 == counter, false);

	IPlugIn** ptrPlugIn = &pPlugIn;

	ptrPlugIn[0] = (IPlugIn *)&m_IE6PlugIn;

	return  true;
}

void IEPlugInFactory::ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
{
}