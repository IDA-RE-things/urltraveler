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

	//! 取当前线程ID
	virtual UINT GetThreadID();
	//! 恢复线程.
	virtual BOOL ResumeThread();
	//! 挂起线程.
	virtual BOOL SuspendThread();
	//! 休眠线程.
	virtual BOOL Sleep(int nMilliSecond);
	//! 关闭线程.
	virtual BOOL ShutdownThread(DWORD dwMilliseconds = INFINITE);
	//! 创建线程.
	virtual BOOL CreateThread(IThreadEvent * pThreadEvent, BOOL bSuspend = FALSE);
	//! 得到线程优先级.
	virtual int GetThreadPriority();
	//! 设置线程优先级.
	virtual BOOL SetThreadPriority(int nPriority);

	virtual void Release();
	
protected:
	//! 进入线程虚函数
	virtual void OnThreadEntry();
	//! RUN
	virtual int Run();
	//! 线程退出虚函数
	virtual void OnThreadExit();
	
	//! 线程指针
	CThreadProcess* m_pThreadProcess;
	IThreadEvent*	m_pThreadEvent; 
};
#endif // _THREADIMPL_H_GONGWEI_
