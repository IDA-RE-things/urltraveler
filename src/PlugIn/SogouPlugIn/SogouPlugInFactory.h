#pragma once

#include "PlugIn.h"
#include "SogouPlugIn.h"

extern "C" 
{
	DLLEXPORT IPlugInFactory*	GetPlugInFactory();
	DLLEXPORT void	ReleasePlugInFactory( );
}


class SogouPlugInFactory : public PlugInFactoryImpl<SogouPlugIn>
{
public:
	SogouPlugInFactory();
	virtual ~SogouPlugInFactory();

	BOOL QueryPlugInCounter(uint32 & counter);
	BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn);
	void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn);

private:
	SogouPlugIn	m_SogouPlugIn;
};

namespace sogouplugin
{
	extern SogouPlugInFactory*	g_SogouPlugInFactory;
}
using namespace sogouplugin;