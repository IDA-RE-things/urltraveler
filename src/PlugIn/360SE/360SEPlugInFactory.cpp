#include "stdafx.h"
#include "360SEPlugInFactory.h"
#include "SndaBase.h"

namespace n360seplugin
{
	C360SEPlugInFactory*	g_SogouPlugInFactory	=	NULL;
}

// 导出借口实现
IPlugInFactory*	GetPlugInFactory()
{
	if( g_SogouPlugInFactory == NULL)
	{
		g_SogouPlugInFactory = new C360SEPlugInFactory();
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

C360SEPlugInFactory::C360SEPlugInFactory()
{

}

C360SEPlugInFactory::~C360SEPlugInFactory()
{

}

BOOL C360SEPlugInFactory::QueryPlugInCounter(uint32 & counter)
{
	counter = 3;
	return TRUE;
}

BOOL C360SEPlugInFactory::QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
{
	PAssert_ReturnWithValue(3 == counter, false);

	IPlugIn** ptrPlugIn = &pPlugIn;
	ptrPlugIn[0] = (IPlugIn *)&m_360SE2PlugIn;
	ptrPlugIn[1] = (IPlugIn *)&m_360SE3PlugIn;
	ptrPlugIn[2] = (IPlugIn *)&m_360ChromePlugIn;

	return  true;
}

void C360SEPlugInFactory::ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
{
}