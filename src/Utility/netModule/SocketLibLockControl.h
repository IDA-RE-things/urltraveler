#if !defined(AFX_SOCKETLIBLOCKCONTROL_H__77E995CF_FCDF_40E2_8B1F_17F337805441__INCLUDED_)
#define AFX_SOCKETLIBLOCKCONTROL_H__77E995CF_FCDF_40E2_8B1F_17F337805441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include <WinSock.h>

namespace NetModule
{
	class CSocketLibLockControl  
	{
	public:
		CSocketLibLockControl()
		{
			Lock();
			
			DWORD& dwLockCount = GetCount();
			dwLockCount++;
			
			if(dwLockCount == 1)
			{
				WSADATA stWSAdata = {0};
				WORD wVersionRequested = 0;
				wVersionRequested = MAKEWORD(2, 2);
				int nErrorCode = WSAStartup(wVersionRequested, &stWSAdata);
				
				if( nErrorCode != 0 )
				{
					wVersionRequested = MAKEWORD(1, 1);
					nErrorCode = WSAStartup(wVersionRequested, &stWSAdata);
				}	
			}
			
			Unlock();
		}
		
		virtual ~CSocketLibLockControl()
		{
			Lock();
			
			DWORD& dwLockCount = GetCount();
			dwLockCount--;
			
			if(dwLockCount == 0)
			{
				WSACleanup();
			}
			
			Unlock();
		}
		
		static int m_snGuard;
		
		static void Init()
		{			
			if(m_snGuard == 0)
			{
				InitializeCriticalSection(GetCriticalSection());
			}	
			
			m_snGuard++;
		}
		
		static void Clear()
		{
			m_snGuard--;
			
			if(m_snGuard == 0)
			{
				DeleteCriticalSection(GetCriticalSection());
			}
			
		}
		
		static DWORD& GetCount()
		{
			static DWORD s_dwLockCount = 0;
			return s_dwLockCount;
		}
		
		static CRITICAL_SECTION* GetCriticalSection()
		{
			static CRITICAL_SECTION s_cs = {0};
			return &s_cs;
		}  
		
	private:
		void Lock()
		{
			EnterCriticalSection(GetCriticalSection());
		}
		
		void Unlock()
		{
			LeaveCriticalSection(GetCriticalSection());
		}		
	};
	
	__declspec(selectany) int CSocketLibLockControl::m_snGuard = 0;
	
	class CInitClass
	{
	public:
		CInitClass()
		{
			CSocketLibLockControl::Init();
		}
		~CInitClass()
		{
			CSocketLibLockControl::Clear();
		}
	};
	
};
#endif // !defined(AFX_SOCKETLIBLOCKCONTROL_H__77E995CF_FCDF_40E2_8B1F_17F337805441__INCLUDED_)
