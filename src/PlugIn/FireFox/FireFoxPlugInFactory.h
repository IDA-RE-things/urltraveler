#pragma once

#include "PlugIn.h"
#include "FireFox3PlugIn.h"

extern "C" 
{
	DLLEXPORT IPlugInFactory*	GetPlugInFactory();
	DLLEXPORT void	ReleasePlugInFactory( );
}

class FireFoxPlugInFactory : public PlugInFactoryImpl<FireFox3PlugIn>
{
public:
	FireFoxPlugInFactory();
	virtual ~FireFoxPlugInFactory();

	BOOL QueryPlugInCounter(uint32 & counter);
	BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn);
	void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn);

private:
	FireFox3PlugIn	m_FireFoxPlugIn;
};

namespace firefox
{
	extern FireFoxPlugInFactory*	g_FireFoxPlugInFactory;
}
using namespace firefox;
