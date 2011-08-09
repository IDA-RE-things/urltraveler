// F:	1 增加接口调用失败时、GetLastError()返回错误码的机制
//		2 增加调用ShutDown()后，m_bThreadExit复位
//		[8/7/2008] edwardkou
#if !defined(AFX_EVENTSOCKETEX_H__C8AAFDE1_A6FF_488C_BDD1_4847488341DD__INCLUDED_)
#define AFX_EVENTSOCKETEX_H__C8AAFDE1_A6FF_488C_BDD1_4847488341DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000

#include "SocketLibLockControl.h"
#include "socketobject_i.h"
#include "Guard.h"

namespace NetModule
{	

	//////////////////////////////////////////////////////////////////////////
	
	class CTCPClient;
	
#define TCPM_SOCKET_EVENT	(WM_USER + 467)
	
	typedef struct _tagTcpPackageInfo
	{
		CTCPClient*   pTCPClient;
		BOOL          bConnected;//是否连接成功
		BOOL          bBeginRecv;//是否开始接收
		BOOL          bClose;    //是否关闭
		BYTE*         pszBuffer;
		int           nLen;
	}TCPPACKAGEINFO;
	
	class CTCPSocketThread
	{
	public:
		void SetShareData(CTCPClient* pTCPClient, HWND hTCPWindow);
		BOOL ShutDown();
		CTCPSocketThread();
		virtual ~CTCPSocketThread();
		
		BOOL Connect(ULONG ulIP, USHORT usPort);
		BOOL Send(const void *pBuf, int nLen);	
		void CloseSocket();

// 		void SetMode(DWORD dwMode);
// 
		
		// 为了简化同步控制，dwOption要在Connect()前设置，之后不允许改变
		// SetOption只能在Connect()前调用，Connect之后不允许调用
		void SetOption(PSOCKETOPTION pSocketOption);

		void SetBandWidth(DWORD dwBandWidth);
		void SetMaxPacketSize(BYTE byPacketSize);
		
		static void* Alloc(size_t nBuffLen);
		static void  Free(void* pBuffer);
		static UINT  _stdcall ConnAndRecvThread(LPVOID lParam);
		static void  AddPackageToList(CTCPSocketThread *pTCPSocketThread,
			HWND hTCPWindow,
			TCPPACKAGEINFO* pSocketInfo);

		static BOOL PostPacket(
			CTCPSocketThread *pTCPSocketThread,
			HWND hTCPWindow,
			BYTE* pBuffer, 
			int nLen);
		
	private:
		void WaitThreadExit();

		void  GetBandWidth(DWORD& dwBandWidth);

		static DWORD GetSleepTime(DWORD dwSize, DWORD dwElapsedTime, DWORD dwBandWidth);
		
	private:
		SOCKET m_hSocket;
		BOOL   m_bThreadExit;

		// 增加 m_bConnected 这个参数，给主线程和连接线程共享，
		// 当连接线程已经成功连接m_bConnected为TRUE，反之为FALSE，
		// 目的是：
		//		当连接线程在超时的时间内，还没有连接成功，
		//	上层逻辑会删除这个对象，启用新的对象去连接；但是删除的时候connect还
		//   没有返回，连接线程阻塞；而主线程WaitForSingleObject也依赖连接线程的退出，
		//   这个时候主线程也阻塞了。为了避免这个情况，如果发现这种情况，则显式关闭连接线程
		BOOL   m_bConnecting; 

		HANDLE m_hThread;
		DWORD	m_dwThreadID;
		
		CTCPClient*                     m_pTCPClient;
		HWND                            m_hTCPWindow;

		//DWORD			 m_dwMode;   // 模式.0 : 非延迟读，SOCKETMODE_DELAYRECEIVE : 延迟读
		SOCKETOPTION	 m_stSocketOption; // Socket 模式
		BOOL			 m_bSleep;   // 子线程处于sleep状态

		//DWORD			 m_dwBandWidth;		// 下载带宽,带宽等于0不限速,单位: KB/S
		CMutex     m_mutex;			// 保护主线程和子线程互斥访问m_dwBandWidth的临界区
	};
	
};
#endif // !defined(AFX_EVENTSOCKETEX_H__C8AAFDE1_A6FF_488C_BDD1_4847488341DD__INCLUDED_)
