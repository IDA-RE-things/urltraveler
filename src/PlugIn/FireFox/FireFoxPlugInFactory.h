#pragma once

#include "PlugIn.h"
#include "FireFoxPlugIn.h"

extern "C" 
{
	DLLEXPORT IPlugInFactory*	GetPlugInFactory();
	DLLEXPORT void	ReleasePlugInFactory( );
}

class FireFoxPlugInFactory : public PlugInFactoryImpl<FireFoxPlugIn>
{
public:
	FireFoxPlugInFactory();
	virtual ~FireFoxPlugInFactory();

	BOOL QueryPlugInCounter(uint32 & counter);
	BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn);
	void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn);

private:
	FireFoxPlugIn	m_FireFoxPlugIn;
};

namespace firefox
{
	extern FireFoxPlugInFactory*	g_FireFoxPlugInFactory;
}
using namespace firefox;
