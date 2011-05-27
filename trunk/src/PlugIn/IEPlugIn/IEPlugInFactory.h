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
};

namespace ieplugin
{
	extern IEPlugInFactory*	g_IEPlugInFactory;
}
using namespace ieplugin;