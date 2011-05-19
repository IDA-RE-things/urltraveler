#pragma once

#include "PlugIn.h"
#include "Maxthon2PlugIn.h"
#include "Maxthon3PlugIn.h"

extern "C" 
{
	DLLEXPORT IPlugInFactory*	GetPlugInFactory();
	DLLEXPORT void	ReleasePlugInFactory();
}


class MaxthonPlugInFactory : public PlugInFactoryImpl<Maxthon2PlugIn>
{
public:
	MaxthonPlugInFactory();
	virtual ~MaxthonPlugInFactory();

	BOOL QueryPlugInCounter(uint32 & counter);
	BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn);
	void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn);

private:
	Maxthon2PlugIn	m_Maxthon2PlugIn;
	Maxthon3PlugIn	m_Maxthon3PlugIn;
};

namespace maxthonplugin
{
	extern MaxthonPlugInFactory*	g_MaxthonPlugInFactory;
}
using namespace maxthonplugin;