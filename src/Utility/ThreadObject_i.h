#ifndef __THREADOBJECT_I_H_GONGWEI__
#define __THREADOBJECT_I_H_GONGWEI__

#include "stdafx.h"

class IThreadEvent;

class IThreadObject
{
public:
	//! 取当前线程ID
	virtual UINT GetThreadID() = 0;
	//! 恢复线程.
	virtual BOOL ResumeThread() = 0;
	//! 挂起线程.
	virtual BOOL SuspendThread() = 0;
	//! 休眠线程.
	virtual BOOL Sleep(int nMilliSecond) = 0;
	//! 关闭线程.
	virtual BOOL ShutdownThread(DWORD dwMilliseconds = INFINITE) = 0;
	//! 创建线程.
	virtual BOOL CreateThread(IThreadEvent * pThreadEvent, BOOL bSuspend = FALSE) = 0;
	//! 得到线程优先级.
	virtual int GetThreadPriority() = 0;
	//! 设置线程优先级.
	virtual BOOL SetThreadPriority(int nPriority) = 0;
	//! 释放对象
	virtual void Release() = 0;
};

class IThreadEvent
{
public:
	//! 进入线程虚函数
	virtual void OnThreadEntry() = 0;
	//! RUN
	virtual int Run() = 0;
	//! 线程退出虚函数
	virtual void OnThreadExit() = 0;
};

UTILITY_API IThreadObject* CreateThreadObject();

#endif	// __THREADOBJECT_I_H_GONGWEI__
