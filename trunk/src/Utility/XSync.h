#pragma once

#include "stdafx.h"
#include "windows.h"
#include <string>

using namespace std;

class CXSyncObject;
	class CXMutex;
	class CXEvent;
	class CXCriticalSection;

//==============================================================
//			CXSyncObject
//==============================================================

class UTILITY_API CXSyncObject
{

public:
	CXSyncObject( LPCTSTR pstrName = NULL);
	virtual ~CXSyncObject();

	// Attributes
public:
	operator HANDLE() const;
	HANDLE  m_hObject;
	string	m_strName;

	// Operations
	virtual BOOL Lock(DWORD dwTimeout = INFINITE);
	virtual BOOL Unlock() = 0;
	virtual BOOL Unlock(LONG /* lCount */, LPLONG /* lpPrevCount=NULL */)
	{ return TRUE; }

};


//==============================================================
//			CXMutex
//==============================================================
class UTILITY_API CXMutex : public CXSyncObject
{
public:
	CXMutex( BOOL bInitiallyOwn = FALSE, LPCTSTR lpszName = NULL,
		LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
	virtual ~CXMutex();

	// Implementation
public:
	BOOL Unlock();
};



//==============================================================
//			CXSemaphore
//==============================================================

class UTILITY_API CXSemaphore : public CXSyncObject
{
public:
	CXSemaphore(LONG lInitialCount = 1, LONG lMaxCount = 1,
		LPCTSTR pstrName = NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL);

	// Implementation
public:
	virtual ~CXSemaphore();
	virtual BOOL Unlock();
	virtual BOOL Unlock(LONG lCount, LPLONG lprevCount = NULL);
};


//==============================================================
//			CXEvent
//==============================================================
class UTILITY_API CXEvent : public CXSyncObject
{
public:
	CXEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE,
		LPCTSTR lpszNAme = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
	virtual ~CXEvent();

	// Operations
public:
	BOOL SetEvent();
	BOOL PulseEvent();
	BOOL ResetEvent();
	BOOL Unlock();
};


//==============================================================
//			CXCriticalSection
//============================================================== 

class UTILITY_API CXCriticalSection : public CXSyncObject
{
public:
	CXCriticalSection();

	// Attributes
public:
	operator CRITICAL_SECTION*();
	CRITICAL_SECTION m_sect;

	// Operations
public:
	BOOL Unlock();
	BOOL Lock();
	BOOL Lock(DWORD dwTimeout);

	// Implementation
public:
	virtual ~CXCriticalSection();
};