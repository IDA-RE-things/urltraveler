#include "stdafx.h"
#include "SndaBase.h"
#include "ChromePlugInFactory.h"

namespace chromeplugin
{
	ChromePlugInFactory*	g_ChromePlugInFactory	=	NULL;
}

// 导出借口实现
IPlugInFactory*	GetPlugInFactory()
{
	if( g_ChromePlugInFactory == NULL)
	{
		g_ChromePlugInFactory = new ChromePlugInFactory();
		ASSERT( g_ChromePlugInFactory != NULL);
	}

	return g_ChromePlugInFactory;
}

void	ReleasePlugInFactory()
{
	if( g_ChromePlugInFactory  != NULL)
	{
		delete g_ChromePlugInFactory;
		g_ChromePlugInFactory = NULL;
	}
}


ChromePlugInFactory::ChromePlugInFactory()
{

}

ChromePlugInFactory::~ChromePlugInFactory()
{

}

BOOL ChromePlugInFactory::QueryPlugInCounter(uint32 & counter)
{
	counter = 1;
	return TRUE;
}

BOOL ChromePlugInFactory::QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
{
	PAssert_ReturnWithValue(1 == counter, false);

	IPlugIn** ptrPlugIn = &pPlugIn;

	ptrPlugIn[0] = (IPlugIn *)&m_ChromePlugIn;

	return  true;
}

void ChromePlugInFactory::ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
{
}