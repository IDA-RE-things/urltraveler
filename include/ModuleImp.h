#pragma once

#include "SndaBase.h"
#include "Module.h"

class ModuleImpl : public IModule
{
public:
	ModuleImpl():m_pModuleManager(NULL){};
	virtual ~ModuleImpl();

	virtual BOOL Load(IModuleManager* pModuleManager);
	virtual BOOL Unload();

	virtual  const wchar_t *  GetModuleName();
	virtual void ProcessEvent(const Event& evt);
	virtual void ProcessMessage(const Message& msg);
	virtual int32 CallDirect(const param lparam, param wparam);
	virtual void PaybackExtraInfo(uint32 valueId ,void * pExtraInfo );

private:
	IModuleManager*	m_pModuleManager;
};