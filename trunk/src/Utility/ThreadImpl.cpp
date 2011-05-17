#include "StdAfx.h"
#include "threadprocess.h"
#include "ThreadImpl.h"
#include "assert.h"

	
CThreadImpl::CThreadImpl()
{
	m_pThreadProcess = new CThreadProcess(this);
	m_pThreadEvent   = NULL;
}

CThreadImpl::~CThreadImpl()
{
	ComponentFinalize();
}

void CThreadImpl::ComponentFinalize()
{
	if ((GetCurrentThreadId() == m_pThreadProcess->m_nThreadId))
	{
		assert(FALSE);
	}
	if(m_pThreadProcess)
	{
		m_pThreadProcess->Shutdown();
		delete m_pThreadProcess;
		m_pThreadProcess = NULL;
	}		
}

int CThreadImpl::Run()
{
	return m_pThreadEvent->Run();
}

BOOL CThreadImpl::CreateThread(IThreadEvent * pThreadEvent, BOOL bSuspend)
{
	assert(pThreadEvent);
	
	m_pThreadEvent = pThreadEvent;
	
	if(m_pThreadProcess->Create())
	{
		if (bSuspend)
		{
			SuspendThread();
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CThreadImpl::ShutdownThread(DWORD dwMilliseconds)
{
	if (!m_pThreadProcess)
	{
		return FALSE;
	}
	if ((GetCurrentThreadId() == m_pThreadProcess->m_nThreadId))
	{
		return FALSE;
	}
	
	return m_pThreadProcess->Shutdown(dwMilliseconds);
}

BOOL CThreadImpl::Sleep(int nMilliSecond)
{
	return m_pThreadProcess->Sleep(nMilliSecond);
}

BOOL CThreadImpl::SuspendThread()
{
	return m_pThreadProcess->Suspend();
}

BOOL CThreadImpl::ResumeThread()
{
	return m_pThreadProcess->Resume();
}

void CThreadImpl::OnThreadEntry()
{
	m_pThreadEvent->OnThreadEntry();
}

void CThreadImpl::OnThreadExit()
{
	m_pThreadEvent->OnThreadExit();
}

UINT CThreadImpl::GetThreadID()
{
	return m_pThreadProcess->m_nThreadId;
}

int CThreadImpl::GetThreadPriority()
{
	return m_pThreadProcess->GetThreadPriority();
}

BOOL CThreadImpl::SetThreadPriority(int nPriority)
{
	return m_pThreadProcess->SetThreadPriority(nPriority);
}

void CThreadImpl::Release()
{
	delete this;
}

IThreadObject *CreateThreadObject()
{
	return new CThreadImpl;
}