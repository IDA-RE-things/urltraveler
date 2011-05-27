#pragma once

#include "PlugIn.h"

#include "360SE3PlugIn.h"
#include "360SE2PlugIn.h"
#include "360ChromePlugIn.h"


extern "C" 
{
	DLLEXPORT IPlugInFactory*	GetPlugInFactory();
	DLLEXPORT void	ReleasePlugInFactory( );
}

class C360SEPlugInFactory : public PlugInFactoryImpl<C360SE3PlugIn>
{
public:
	C360SEPlugInFactory();
	virtual ~C360SEPlugInFactory();

	BOOL QueryPlugInCounter(uint32 & counter);
	BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn);
	void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn);

private:
	C360SE2PlugIn	m_360SE2PlugIn;
	C360SE3PlugIn	m_360SE3PlugIn;
	C360ChromePlugIn	m_360ChromePlugIn;
};

namespace n360seplugin
{
	extern C360SEPlugInFactory*	g_360PlugInFactory;
}
using namespace n360seplugin;