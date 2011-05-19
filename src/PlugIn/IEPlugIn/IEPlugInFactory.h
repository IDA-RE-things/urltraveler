#pragma once

#include "PlugIn.h"
#include "IePlugIn.h"

extern "C" 
{
	DLLEXPORT IPlugInFactory*	GetPlugInFactory();
	DLLEXPORT void	ReleasePlugInFactory();
}

class IEPlugInFactory : public PlugInFactoryImpl<IEPlugIn>
{
public:
	IEPlugInFactory();
	virtual ~IEPlugInFactory();

	BOOL QueryPlugInCounter(uint32 & counter);
	BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn);
	void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn);

private:
	IEPlugIn	m_IE6PlugIn;
};

namespace ieplugin
{
	extern IEPlugInFactory*	g_IEPlugInFactory;
}
using namespace ieplugin;