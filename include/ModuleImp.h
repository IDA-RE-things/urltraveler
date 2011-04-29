#pragma once

#include "SndaBase.h"
#include "Module.h"

class ModuleImpl : public IModule
{
public:
	ModuleImpl();
	virtual ~ModuleImpl();

private:
	IModuleManager*	m_pModuleManager;
};