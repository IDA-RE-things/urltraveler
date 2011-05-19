#include "stdafx.h"
#include "FireFoxPlugInFactory.h"
#include "SndaBase.h"

namespace firefox
{
	FireFoxPlugInFactory*	g_FireFoxPlugInFactory	=	NULL;
}

// 导出借口实现
IPlugInFactory*	GetPlugInFactory()
{
	if( g_FireFoxPlugInFactory == NULL)
	{
		g_FireFoxPlugInFactory = new FireFoxPlugInFactory();
		ASSERT( g_FireFoxPlugInFactory != NULL);
	}

	return g_FireFoxPlugInFactory;
}

void	ReleasePlugInFactory( )
{
	if( g_FireFoxPlugInFactory  != NULL)
	{
		delete g_FireFoxPlugInFactory;
		g_FireFoxPlugInFactory = NULL;
	}
}


FireFoxPlugInFactory::FireFoxPlugInFactory()
{

}

FireFoxPlugInFactory::~FireFoxPlugInFactory()
{

}

BOOL FireFoxPlugInFactory::QueryPlugInCounter(uint32 & counter)
{
	counter = 1;
	return TRUE;
}

BOOL FireFoxPlugInFactory::QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
{
	PAssert_ReturnWithValue(1 == counter, false);

	IPlugIn** ptrPlugIn = &pPlugIn;
	ptrPlugIn[0] = (IPlugIn *)&m_FireFoxPlugIn;

	return  true;
}

void FireFoxPlugInFactory::ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
{
}