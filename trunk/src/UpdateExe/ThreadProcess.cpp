#include "StdAfx.h"
#include "ThreadProcess.h"
#include "ThreadImpl.h"
#include "assert.h"
#include "process.h"
	
CThreadProcess::CThreadProcess(CThreadImpl *pOnwer)
{
	m_pOwner	  = pOnwer;
	m_hThread	  = NULL;
	m_hCloseEvent = NULL;
	m_nThreadId	  = 0;
}

CThreadProcess::~CThreadProcess()
{
	if (m_hThread)
	{
		Shutdown();
	}
	
	if (m_hCloseEvent)
	{
		CloseHandle(m_hCloseEvent);
		m_hCloseEvent = NULL;
	}
}

UINT WINAPI CThreadProcess::ThreadProc(LPVOID pPaRC)
{
	CThreadProcess* pThis = reinterpret_cast<CThreadProcess*>( pPaRC );
	assert( pThis != NULL );
	
	pThis->Run();
	
	return 1L;
}

BOOL CThreadProcess::Create()
{
	if(m_hThread)
	{
		return FALSE;
	}
	
	m_hThread = (HANDLE)_beginthreadex(
		NULL,				// Security attributes
		0,					// stack
		ThreadProc,			// Thread proc
		this,				// Thread paRC
		CREATE_SUSPENDED,	// creation mode
		&m_nThreadId)		// Thread ID
		;
	
	if (m_hThread != NULL)
	{
		::SetThreadPriority(m_hThread,THREAD_PRIORITY_NORMAL);
		
		ResumeThread( m_hThread );
		
		return TRUE;
	}
	
	return FALSE;
}


int CThreadProcess::Run()
{
	int ret = 0;
	
	m_hCloseEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	m_pOwner->OnThreadEntry();
	
	while (WaitForSingleObject(m_hCloseEvent, 0) == WAIT_TIMEOUT)
	{
		ret = m_pOwner->Run();
		if (!ret)
		{
			break;
		}
	}
	
	CloseHandle(m_hCloseEvent);
	
	m_hCloseEvent = NULL;
	
	m_pOwner->OnThreadExit();
	
	return ret;
}

BOOL CThreadProcess::Shutdown(DWORD dwMillionseconds)
{
	if (m_hThread)
	{
		Resume();
		
		SetEvent(m_hCloseEvent);
		
		if (WaitForSingleObject(m_hThread, dwMillionseconds) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hThread, 1L);
		}
		
		CloseHandle(m_hThread);
		
		m_hThread = NULL;
		
		return TRUE;
	}
	
	return FALSE;
}

BOOL CThreadProcess::Sleep(int nMilliSecond)
{
	return (WAIT_TIMEOUT == WaitForSingleObject(m_hCloseEvent, nMilliSecond));
}

BOOL CThreadProcess::Suspend()
{
	return (-1 != SuspendThread(m_hThread));
}

BOOL CThreadProcess::Resume()
{
	return (-1 != ResumeThread(m_hThread));
}

int CThreadProcess::GetThreadPriority()
{
	return ::GetThreadPriority(m_hThread);
}

BOOL CThreadProcess::SetThreadPriority(int nPriority)
{
	return ::SetThreadPriority(m_hThread, nPriority);
}
