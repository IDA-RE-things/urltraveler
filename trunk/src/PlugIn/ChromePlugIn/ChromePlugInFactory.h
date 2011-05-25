#pragma once

#include "PlugIn.h"
#include "ChromePlugIn.h"

extern "C" 
{
	DLLEXPORT IPlugInFactory*	GetPlugInFactory();
	DLLEXPORT void	ReleasePlugInFactory();
}

class ChromePlugInFactory : public PlugInFactoryImpl<CChromePlugIn>
{
public:
	ChromePlugInFactory();
	virtual ~ChromePlugInFactory();

	BOOL QueryPlugInCounter(uint32 & counter);
	BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn);
	void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn);

private:
	CChromePlugIn	m_ChromePlugIn;
};

namespace chromeplugin
{
	extern ChromePlugInFactory*	g_chromePlugInFactory;
}
using namespace chromeplugin;