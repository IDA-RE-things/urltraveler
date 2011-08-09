#ifndef __TCPMODULE_H__
#define __TCPMODULE_H__
#include "incCommon.h"
#include "socketobject_i.h"
#include "TCPSocketThread.h"
#include "SocketLibLockControl.h"
#include "Guard.h"


namespace NetModule
{
	using namespace std;
	
	class CTCPClient;
	
	class CTCPExchange
	{
	public:
		CTCPExchange()
		{
		};
		~CTCPExchange()
		{
		};

		void AddTcpClient(CTCPClient* pTcpClient)
		{
			CGuard guard(m_mutex);
			m_listTCPClient.push_back(pTcpClient);
		}

		void RemoveClient(CTCPClient* pTcpClient)
		{
			CGuard guard(m_mutex);
			for( std::list<CTCPClient*>::iterator iter = m_listTCPClient.begin(); 
			iter != m_listTCPClient.end(); 
			++iter )
			{
				if( pTcpClient == *iter)
				{
					m_listTCPClient.erase(iter);
					break;
				}
			}
		};
		//void OnInterval(DWORD dwParam);
		void ProcessPackage(TCPPACKAGEINFO* pSocketPackInfo);

	private:
		std::list<CTCPClient*>       m_listTCPClient;
		CMutex     m_mutex;		// 临界区
	};
	
	//////////////////////////////////////////////////////////////////////////
	
    class CTCPClient : public ITCPSocketObject
		, public CSocketLibLockControl
	{
	public:
		CTCPClient();
		~CTCPClient();
	public:
		// ISocketObject
		virtual BOOL Create(LPVOID lpParam, ISocketEvent* pEventHandler);
		virtual BOOL Connect(LPTSTR lpszHost, USHORT ushPort);
		virtual BOOL Connect(ULONG ulHostIP, USHORT ushPort);
		virtual BOOL Disconnect();
		virtual BOOL Send(LPBYTE lpBytes, int nLen);
        virtual void Release();
		//void SetMode(DWORD dwMode);
		
		virtual void SetOption(PSOCKETOPTION pSocketOption);
		virtual void SetBandWidth(DWORD dwBandWidth);
		virtual void SetMaxPacketSize(BYTE byPacketSize);
				
	public:
		void OnRead(LPBYTE pBytes, int nLen);
		void OnClose();
		void OnConnect(int nErrorCode);
		
	public:
		BOOL			m_bSocketInited;
		ISocketEvent*	m_pEventHandler;
		
		UINT	m_unReceiveBufferLen;
		UINT	m_unSendBufferLen;
		LPBYTE	m_pReceiveBuffer;
	
		ENUMSOCKETSTATUS m_nSocketStatus;
		
		// Log functions
		void ShowLastErrorInfo();
		void ShowLastErrorInfo(int nErrorCode);
		
		// Network functions
		BOOL  CreateSocket();
		void  CleanUp();
		int   CloseSocket();
		ULONG SelectOneRandomIP(LPHOSTENT pHostent); // 如果需要连接的域名有多个IP, 随机选择一个
		
		// ISocketEventHandler
		void FireOnConnectFail(int nErrorCode);
		void FireOnDisconnect(int nErrorCode);
		
		void OnHostLookupSuccess(ULONG* pulIPs, int nIPNum);
		void OnHostLookupFail(int nError);
		
	//	BOOL RegisterWindowClass();
		
		// Member Function
		BOOL CreateTCPWindow();
		void SetLastErrorCode(DWORD dwErrCode);
		
	private:
		ULONG            m_ulHostIP;
		USHORT           m_usPort;
		CTCPSocketThread m_SocketThread;
		HWND             m_hTCPWindow;
		DWORD			 m_dwErrorCode;
		//DWORD			 m_dwMode;   // 模式.0 : 非延迟读，SOCKETMODE_DELAYRECEIVE : 延迟读
	public:
		static CTCPExchange ms_TCPExchange;
	};
	
};


#endif

