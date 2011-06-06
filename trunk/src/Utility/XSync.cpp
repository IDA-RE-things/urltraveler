#include "stdafx.h"
#include "XSync.h"
#include "assert.h"


//===================================================================
// CXSyncObject
//===================================================================

CXSyncObject::CXSyncObject( LPCTSTR pstrName)
{
	m_hObject = NULL;
	if( pstrName == NULL )
		m_strName = "";
	else
		m_strName = (char*)pstrName;
}

CXSyncObject::~CXSyncObject()
{
	if (m_hObject != NULL)
	{
		::CloseHandle(m_hObject);
		m_hObject = NULL;
	}
}

BOOL CXSyncObject::Lock(DWORD dwTimeout)
{
	DWORD dwRet = ::WaitForSingleObject(m_hObject, dwTimeout);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
		return TRUE;
	else
		return FALSE;
}

//===================================================================
// CXMutex
//===================================================================
CXMutex::CXMutex(BOOL bInitiallyOwn, LPCTSTR pstrName,
		     LPSECURITY_ATTRIBUTES lpsaAttribute /* = NULL */)
		     : CXSyncObject(pstrName)
{
	m_hObject = ::CreateMutex(lpsaAttribute, bInitiallyOwn, pstrName);
}

CXMutex::~CXMutex()
{
}

BOOL CXMutex::Unlock()
{
	return ::ReleaseMutex(m_hObject);
}


//===================================================================
// CXSemaphore
//===================================================================

CXSemaphore::CXSemaphore(LONG lInitialCount, LONG lMaxCount,
					   LPCTSTR pstrName, LPSECURITY_ATTRIBUTES lpsaAttributes)
					   :  CXSyncObject(pstrName)
{
	assert(lMaxCount > 0);
	assert(lInitialCount <= lMaxCount);

	m_hObject = ::CreateSemaphore(lpsaAttributes, lInitialCount, lMaxCount,
		pstrName);
}

CXSemaphore::~CXSemaphore()
{
}

BOOL CXSemaphore::Unlock()
{
	return Unlock(1, NULL);
}

BOOL CXSemaphore::Unlock(LONG lCount, LPLONG lpPrevCount /* =NULL */)
{
	return ::ReleaseSemaphore(m_hObject, lCount, lpPrevCount);
}


//===================================================================
// CXEvent
//===================================================================
CXEvent::CXEvent(BOOL bInitiallyOwn, BOOL bManualReset,
			LPCTSTR lpszNAme, LPSECURITY_ATTRIBUTES lpsaAttribute)
{
	m_hObject = ::CreateEvent(lpsaAttribute, bManualReset,
		bInitiallyOwn, lpszNAme);
}

CXEvent::~CXEvent()
{
}

BOOL CXEvent::Unlock()
{
	return TRUE;
}

BOOL CXEvent::SetEvent()
{
	assert( m_hObject != NULL );
	return ::SetEvent( m_hObject );
}

BOOL CXEvent::PulseEvent()
{
	assert( m_hObject != NULL );
	return ::PulseEvent( m_hObject );
}

BOOL CXEvent::ResetEvent()
{
	assert( m_hObject != NULL );
	return ::ResetEvent( m_hObject );
}

//===================================================================
// CXCriticalSection
//===================================================================
CXCriticalSection::CXCriticalSection() : CXSyncObject(NULL)
{ ::InitializeCriticalSection(&m_sect); }
CXCriticalSection::operator CRITICAL_SECTION*()
{ return (CRITICAL_SECTION*) &m_sect; }
CXCriticalSection::~CXCriticalSection()
{ ::DeleteCriticalSection(&m_sect); }
BOOL CXCriticalSection::Lock()
{ ::EnterCriticalSection(&m_sect); return TRUE; }
BOOL CXCriticalSection::Lock(DWORD /* dwTimeout */)
{ return Lock(); }
BOOL CXCriticalSection::Unlock()
{ ::LeaveCriticalSection(&m_sect); return TRUE; }