#ifndef __THREADOBJECT_I_H_GONGWEI__
#define __THREADOBJECT_I_H_GONGWEI__

#include "stdafx.h"

class IThreadEvent;

class IThreadObject
{
public:
	//! ȡ��ǰ�߳�ID
	virtual UINT GetThreadID() = 0;
	//! �ָ��߳�.
	virtual BOOL ResumeThread() = 0;
	//! �����߳�.
	virtual BOOL SuspendThread() = 0;
	//! �����߳�.
	virtual BOOL Sleep(int nMilliSecond) = 0;
	//! �ر��߳�.
	virtual BOOL ShutdownThread(DWORD dwMilliseconds = INFINITE) = 0;
	//! �����߳�.
	virtual BOOL CreateThread(IThreadEvent * pThreadEvent, BOOL bSuspend = FALSE) = 0;
	//! �õ��߳����ȼ�.
	virtual int GetThreadPriority() = 0;
	//! �����߳����ȼ�.
	virtual BOOL SetThreadPriority(int nPriority) = 0;
	//! �ͷŶ���
	virtual void Release() = 0;
};

class IThreadEvent
{
public:
	//! �����߳��麯��
	virtual void OnThreadEntry() = 0;
	//! RUN
	virtual int Run() = 0;
	//! �߳��˳��麯��
	virtual void OnThreadExit() = 0;
};

UTILITY_API IThreadObject* CreateThreadObject();

#endif	// __THREADOBJECT_I_H_GONGWEI__
