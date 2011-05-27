#pragma once

#include "PlugIn.h"
#include "QQPlugIn.h"

extern "C" 
{
	DLLEXPORT IPlugInFactory*	GetPlugInFactory();
	DLLEXPORT void	ReleasePlugInFactory( );
}

class QQPlugInFactory : public PlugInFactoryImpl<QQPlugIn>
{
public:
	QQPlugInFactory();
	virtual ~QQPlugInFactory();

	BOOL QueryPlugInCounter(uint32 & counter);
	BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn);
	void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn);

private:
	QQPlugIn	m_QQPlugIn;
};

namespace qqplugin
{
	extern QQPlugInFactory*	g_QQPlugInFactory;
}
using namespace qqplugin;