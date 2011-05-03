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

	virtual IModuleManager*	GetModuleManager() { return m_pModuleManager;}

protected:
	IModuleManager*	m_pModuleManager;
};

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


// 
template <typename T>
class ModuleFactoryImpl :public  IModuleFactory
{
public:
	ModuleFactoryImpl():m_pstIModule(NULL){}
	~ModuleFactoryImpl()
	{
		if(m_pstIModule!=NULL)
			assert(0);
	}

	BOOL QueryModuleCounter(uint32 & counter)
	{
		counter=1;
		return TRUE;
	}

	BOOL QueryModulePoint(uint32 counter,IModule * & pIModule)
	{
		if(counter == 1 )
		{
			if(m_pstIModule)
			{
				pIModule=m_pstIModule;
				return TRUE;
			}

			m_pstIModule=new T;
			pIModule =m_pstIModule;
			return TRUE;
		}

		return FALSE;
	}

	void 	ReleaseModulePoint(uint32 counter,IModule * pIModule)
	{
		if(counter==1 && pIModule!=NULL)
		{
			if(m_pstIModule==pIModule)
			{
				delete m_pstIModule;
				m_pstIModule=NULL;
				return;
			}

			ASSERT(0);
			return;
		}

		ASSERT(0);
	}

private:
	T * m_pstIModule;
};