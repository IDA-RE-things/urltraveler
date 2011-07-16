#if !defined(AFX_THREADPROCESS_H__9779D4C6_EF41_4024_B324_1879AA84F046__INCLUDED_)
#define AFX_THREADPROCESS_H__9779D4C6_EF41_4024_B324_1879AA84F046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CThreadImpl;
class CThreadProcess
{
public:
	CThreadProcess(CThreadImpl *pOwner);
	virtual ~CThreadProcess();
	
public:
	BOOL Resume();
	BOOL Suspend();
	BOOL Sleep(int nMilliSecond);
	BOOL Shutdown(DWORD dwMilliseconds = INFINITE);
	BOOL Create();
	int GetThreadPriority();
	BOOL SetThreadPriority(int nPriority);
	
public:
	UINT m_nThreadId;
	static UINT WINAPI ThreadProc(LPVOID pParam);
	virtual int Run();
	
protected:
	BOOL	m_bAutoDelete;
	HANDLE	m_hCloseEvent;
	
private:
	HANDLE			m_hThread;
	CThreadImpl *	m_pOwner;
};
	
#endif // !defined(AFX_THREADPROCESS_H__9779D4C6_EF41_4024_B324_1879AA84F046__INCLUDED_)
