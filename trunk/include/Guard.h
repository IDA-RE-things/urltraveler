#ifndef _SYNOBJ_H_
#define _SYNOBJ_H_

#define CLASS_UNCOPYABLE(ClassName) \
private: \
	ClassName##(const ClassName##&); \
	ClassName##& operator=(const ClassName##&);

class CMutex 
{
    CLASS_UNCOPYABLE(CMutex)
public:
	CMutex()
	{
		::InitializeCriticalSection(&m_csLock);
	}
	~CMutex()
	{
		::DeleteCriticalSection(&m_csLock);
	}
	void Lock()
    {
	    ::EnterCriticalSection(&m_csLock);
	}
	void UnLock()
	{ 
	    ::LeaveCriticalSection(&m_csLock);
	}
private:
    CRITICAL_SECTION m_csLock;
};

class CGuard 
{
    CLASS_UNCOPYABLE(CGuard)
public:
	explicit CGuard(CMutex& mutex) : m_mutex(mutex)
	{ 
	    m_mutex.Lock();
	}
	
	~CGuard() 
	{
	    m_mutex.UnLock();
	}

private:
	CMutex& m_mutex;
};

#endif/*SYNOBJ_H*/