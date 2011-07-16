#ifndef _THREADIMPL_H_GONGWEI_
#define _THREADIMPL_H_GONGWEI_

#include "threadobject_i.h"

class CThreadProcess;

class CThreadImpl : public IThreadObject
{
	friend CThreadProcess;
	
public:
	CThreadImpl();
	virtual ~CThreadImpl();
	
	virtual void ComponentFinalize();

	//! ȡ��ǰ�߳�ID
	virtual UINT GetThreadID();
	//! �ָ��߳�.
	virtual BOOL ResumeThread();
	//! �����߳�.
	virtual BOOL SuspendThread();
	//! �����߳�.
	virtual BOOL Sleep(int nMilliSecond);
	//! �ر��߳�.
	virtual BOOL ShutdownThread(DWORD dwMilliseconds = INFINITE);
	//! �����߳�.
	virtual BOOL CreateThread(IThreadEvent * pThreadEvent, BOOL bSuspend = FALSE);
	//! �õ��߳����ȼ�.
	virtual int GetThreadPriority();
	//! �����߳����ȼ�.
	virtual BOOL SetThreadPriority(int nPriority);

	virtual void Release();
	
protected:
	//! �����߳��麯��
	virtual void OnThreadEntry();
	//! RUN
	virtual int Run();
	//! �߳��˳��麯��
	virtual void OnThreadExit();
	
	//! �߳�ָ��
	CThreadProcess* m_pThreadProcess;
	IThreadEvent*	m_pThreadEvent; 
};
#endif // _THREADIMPL_H_GONGWEI_
