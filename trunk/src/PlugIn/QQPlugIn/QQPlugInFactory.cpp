#include "stdafx.h"
#include "QQPlugInFactory.h"
#include "SndaBase.h"

namespace qqplugin
{
	QQPlugInFactory*	g_QQPlugInFactory	=	NULL;
}

// 导出借口实现
IPlugInFactory*	GetPlugInFactory()
{
	if( g_QQPlugInFactory == NULL)
	{
		g_QQPlugInFactory = new QQPlugInFactory();
		ASSERT( g_QQPlugInFactory != NULL);
	}

	return g_QQPlugInFactory;
}

void	ReleasePlugInFactory()
{
	if( g_QQPlugInFactory  != NULL)
	{
		delete g_QQPlugInFactory;
		g_QQPlugInFactory = NULL;
	}
}

QQPlugInFactory::QQPlugInFactory()
{

}

QQPlugInFactory::~QQPlugInFactory()
{

}

BOOL QQPlugInFactory::QueryPlugInCounter(uint32 & counter)
{
	counter = 1;
	return TRUE;
}

BOOL QQPlugInFactory::QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
{
	PAssert_ReturnWithValue(1 == counter, false);

	IPlugIn** ptrPlugIn = &pPlugIn;
	ptrPlugIn[0] = (IPlugIn *)&m_QQPlugIn;

	return  TRUE;
}

void QQPlugInFactory::ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
{
}