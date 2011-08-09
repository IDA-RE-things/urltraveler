// F:	1 ���ӽӿڵ���ʧ��ʱ��GetLastError()���ش�����Ļ���
//		2 ���ӵ���ShutDown()��m_bThreadExit��λ
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
		BOOL          bConnected;//�Ƿ����ӳɹ�
		BOOL          bBeginRecv;//�Ƿ�ʼ����
		BOOL          bClose;    //�Ƿ�ر�
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
		
		// Ϊ�˼�ͬ�����ƣ�dwOptionҪ��Connect()ǰ���ã�֮������ı�
		// SetOptionֻ����Connect()ǰ���ã�Connect֮���������
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

		// ���� m_bConnected ��������������̺߳������̹߳���
		// �������߳��Ѿ��ɹ�����m_bConnectedΪTRUE����֮ΪFALSE��
		// Ŀ���ǣ�
		//		�������߳��ڳ�ʱ��ʱ���ڣ���û�����ӳɹ���
		//	�ϲ��߼���ɾ��������������µĶ���ȥ���ӣ�����ɾ����ʱ��connect��
		//   û�з��أ������߳������������߳�WaitForSingleObjectҲ���������̵߳��˳���
		//   ���ʱ�����߳�Ҳ�����ˡ�Ϊ�˱�������������������������������ʽ�ر������߳�
		BOOL   m_bConnecting; 

		HANDLE m_hThread;
		DWORD	m_dwThreadID;
		
		CTCPClient*                     m_pTCPClient;
		HWND                            m_hTCPWindow;

		//DWORD			 m_dwMode;   // ģʽ.0 : ���ӳٶ���SOCKETMODE_DELAYRECEIVE : �ӳٶ�
		SOCKETOPTION	 m_stSocketOption; // Socket ģʽ
		BOOL			 m_bSleep;   // ���̴߳���sleep״̬

		//DWORD			 m_dwBandWidth;		// ���ش���,�������0������,��λ: KB/S
		CMutex     m_mutex;			// �������̺߳����̻߳������m_dwBandWidth���ٽ���
	};
	
};
#endif // !defined(AFX_EVENTSOCKETEX_H__C8AAFDE1_A6FF_488C_BDD1_4847488341DD__INCLUDED_)
