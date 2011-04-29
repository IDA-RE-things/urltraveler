
#include "ModuleImp.h"

ModuleImpl::~ModuleImpl()
{
}

BOOL ModuleImpl::Load(IModuleManager* pModuleManager)
{
	m_pModuleManager = pModuleManager;
	return TRUE;
}

BOOL ModuleImpl::Unload()
{
	m_pModuleManager = NULL;
	return TRUE;
}

const wchar_t *  ModuleImpl::GetModuleName()
{
	return L"ModuleImpl_Example";
}

void ModuleImpl::ProcessEvent(const Event& evt)
{
}

void ModuleImpl::ProcessMessage(const Message& msg)
{
}

int32 ModuleImpl::CallDirect(const param lparam, param wparam)
{
	return -1;
}

void ModuleImpl::PaybackExtraInfo(uint32 valueId ,void * pExtraInfo )
{
	if(IsEventValue(valueId))
	{
		Event * pstEvent=static_cast<Event*>(pExtraInfo);
		if(pstEvent->m_pstExtraInfo)
			delete pstEvent->m_pstExtraInfo;
	}
	else if(IsMessageValue(valueId))
	{
		Message * pstMessage=static_cast<Message*>(pExtraInfo);
		if(pstMessage->m_pstExtraInfo)
			delete pstMessage->m_pstExtraInfo;
	}
}