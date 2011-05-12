#pragma once
#include "PlugIn.h"

extern "C" 
{
	DLLEXPORT IPlugIn*	GetPlugIn();
	DLLEXPORT void	ReleasePlugIn( IPlugIn*);
}

class CMaxthonPlugIn : public IPlugInImp
{
public:
	CMaxthonPlugIn();
	~CMaxthonPlugIn();
};


namespace maxthonplugin
{
	extern CMaxthonPlugIn*	g_MaxthonPlugIn;
}
using namespace maxthonplugin;

