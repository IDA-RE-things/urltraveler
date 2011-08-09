#pragma warning(disable:4786)
#include "StdAfx.h"
#include "TCPClient.h"
#include <time.h>

namespace NetModule
{
	//////////////////////////////////////////////////////////////////////////
	
	CTCPExchange CTCPClient::ms_TCPExchange;
	
	LRESULT CALLBACK 
		TCPWindowWinProc(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
	{
		if(nMessage == TCPM_SOCKET_EVENT)
		{
			CTCPClient::ms_TCPExchange.ProcessPackage((TCPPACKAGEINFO*)wParam);
		}
		
		return DefWindowProc (hWnd, nMessage, wParam, lParam);
	}
	
	void CTCPExchange::ProcessPackage(TCPPACKAGEINFO* pSocketPackInfo)
	{
		CTCPClient* pTCPClient = pSocketPackInfo->pTCPClient;
		
		BOOL bSuccess = FALSE;
		std::list<CTCPClient*>::iterator it;

		{
		CGuard guard(m_mutex);

		for(it = m_listTCPClient.begin(); it != m_listTCPClient.end(); ++it)
		{
			if( pTCPClient == *it )
			{
				bSuccess = TRUE;
				break;
			}
		}
		}
		
		if( bSuccess )
		{
			if( !pSocketPackInfo->bBeginRecv )
			{
				//test
//				OutputDebugString(_T("2 Get conncet message!"));
				pTCPClient->OnConnect( pSocketPackInfo->bConnected ? 0 : 1 );
			}
			else
			{
				if( pSocketPackInfo->bClose )
				{
					pTCPClient->OnClose();
				}
				else
				{					
					pTCPClient->OnRead((LPBYTE)pSocketPackInfo->pszBuffer, pSocketPackInfo->nLen);
				}
			}
		}
		else
		{
			//Log(_T("CTCPClient is already destroy!\n"));
		}
		
		// TODO
		
		if (pSocketPackInfo != NULL) 
		{
			if( pSocketPackInfo->pszBuffer != NULL)
			{
				delete [] (LPBYTE)(pSocketPackInfo->pszBuffer);
			}
			
			delete [] (LPBYTE)pSocketPackInfo;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	//		CTCPClient
	//////////////////////////////////////////////////////////////////////////
	
	CTCPClient::CTCPClient()
	{
	//	ms_TCPExchange.m_listTCPClient.push_back(this);
		ms_TCPExchange.AddTcpClient(this);
		
		m_pEventHandler		 = NULL;
		m_nSocketStatus		 = ESS_NOTPREPARED;
		m_ulHostIP           = 0;
		m_hTCPWindow         = NULL;
		m_dwErrorCode		 = ERROR_SUCCESS;
		//m_dwMode			 = 0;
		CreateTCPWindow();
		
	srand((unsigned)time(0));
	}
	
	CTCPClient::~CTCPClient()
	{
        ms_TCPExchange.RemoveClient(this);
		CleanUp();	
	}

	// Member Function
// 	BOOL CTCPClient::RegisterWindowClass()
// 	{
// 		WNDCLASS wndclass = {0};
// 		
// 		wndclass.style			= 0;
// 		wndclass.lpfnWndProc	= TCPWindowWinProc;
// 		wndclass.cbClsExtra		= 0 ;
// 		wndclass.cbWndExtra		= 0 ;
// 		wndclass.hInstance		= 0 ;
// 		wndclass.hIcon			= 0 ;
// 		wndclass.hCursor		= 0 ;
// 		wndclass.hbrBackground	= 0 ;
// 		wndclass.lpszClassName	= _T("TCPModuleWindow") ;
// 		
// 		return RegisterClass (&wndclass);
// 	}
	
	// Member Function
	BOOL CTCPClient::CreateTCPWindow()
	{
		//UnregisterClass(_T("TCPModuleWindow"), NULL);
		//RegisterWindowClass();
		
		m_hTCPWindow = CreateWindow(
			_T("static"),	// window class name
			_T("TCPModuleWindow"),	// window caption
			NULL,			// window style
			CW_USEDEFAULT,	// initial x position
			CW_USEDEFAULT,	// initial y position
			CW_USEDEFAULT,	// initial x size
			CW_USEDEFAULT,	// initial y size
			NULL,			// parent window handle
			NULL,	        // window menu handle
			NULL,			// program instance handle
			NULL) ; 	    // creation parameters
		
		if(m_hTCPWindow == NULL)
		{
			int nErrorCode = 0;
			nErrorCode = GetLastError();
			return FALSE;
		}
		
		SetLastError(0);
		
		SetWindowLong(m_hTCPWindow, GWL_WNDPROC, (LONG)TCPWindowWinProc);
		LONG lWinData = (LONG)this;
		
		if(SetWindowLong(m_hTCPWindow, GWL_USERDATA, lWinData) == 0)
		{
			DWORD dwErrorCode = GetLastError();
			
			if(dwErrorCode != 0)
			{
				int nErrorCode = 0;
				nErrorCode = GetLastError();
				//OutputErrorInfo(TEXT("setwindowlong fail!\n"));
				return FALSE;
			}
		}
		
		return TRUE;
	}
	
	// Interface
	BOOL CTCPClient::Create(LPVOID /*lpParam*/, ISocketEvent* pEventHandler)
	{
		//CALLSTACKGUARD;
		
		// 1 TRACE
		//OutputTraceInfo(TEXT("create is invoked\n"));
		
		BOOL bSuccess = FALSE;
		
		// 2 Status Check
		if(m_nSocketStatus == ESS_NOTPREPARED)
		{
			m_pEventHandler = pEventHandler;
			
			// 如果为NULL,只提醒
			if(pEventHandler == NULL)
			{
				//OutputErrorInfo(TEXT("create with null eventhandler\n"));
			}
			if( CreateSocket())
			{
				// 3 Status Set
				m_nSocketStatus = ESS_NOTCONNECTED;
				bSuccess = TRUE;
			}
		}
		else
		{
			//SetLastError(ERROR_CREATESOCKET_TWICE);
			//OutputErrorInfo(TEXT("please don't create twice!\n"));
		}
		
		return bSuccess;
	}
	
	// 外露接口 - 需判断状态 m_nSocketStatus
	BOOL CTCPClient::Connect(LPTSTR lpszHost, USHORT ushPort)
	{
		//CALLSTACKGUARD;
		
		USES_CONVERSION;
		
		
	BOOL bSuccess = lpszHost != NULL;

	m_dwErrorCode = ERROR_SUCCESS;
	
	if(bSuccess && m_nSocketStatus == ESS_NOTCONNECTED)
		{
			char szAsciiHost[MAX_PATH] = {0};
			//strcpy_s(szAsciiHost, _countof(szAsciiHost), T2A(lpszHost));
            StringCchCopyA(szAsciiHost, MAX_PATH, T2A(lpszHost));
           
			
			// 如果是IP形式字符串, 则会转换成功
			ULONG ulHostIP = inet_addr(szAsciiHost);		
			if(ulHostIP == INADDR_NONE) // 转换失败, 需要域名解析
			{
				m_usPort = ushPort;
				//GetHostName(lpszHost);
			// 需要域名解析的时候当前只能先返回成功了,如果返回失败,上层可能会当做错误来处理
			bSuccess = TRUE;
			}
			else
			{
				bSuccess = Connect(ulHostIP, ushPort);
			}
		}
		else if(!bSuccess)
		{
			m_dwErrorCode = ERROR_INVALID_PARAMETER;
		}
		else
		{
			//m_dwErrorCode = ERROR_INCORRECT_STATUS;
		}

		SetLastErrorCode(m_dwErrorCode);
		
		return bSuccess;
	}
	
	// Interface
	BOOL CTCPClient::Connect(ULONG ulHostIP, USHORT ushPort)
	{
		m_ulHostIP = ulHostIP;
		m_usPort = ushPort;
		m_nSocketStatus = ESS_CONNECTTING;
		return m_SocketThread.Connect(ulHostIP, ushPort);
	}
	
	// Interface
	BOOL CTCPClient::Send(LPBYTE lpBytes, int nLen)
	{
		return m_SocketThread.Send(lpBytes, nLen);
	}

	void CTCPClient::SetOption(PSOCKETOPTION pSocketOption)
	{
		m_SocketThread.SetOption(pSocketOption);
	}

	void CTCPClient::SetBandWidth(DWORD dwBandWidth)
	{
		m_SocketThread.SetBandWidth(dwBandWidth);
	}

	void CTCPClient::SetMaxPacketSize(BYTE byPacketSize)
	{
		m_SocketThread.SetMaxPacketSize(byPacketSize);
	}
	
	// Interface
	BOOL CTCPClient::Disconnect()
	{
		//CALLSTACKGUARD;
		
		
		BOOL bSuccess = FALSE;
		
		m_pEventHandler = NULL;
		
		if(m_nSocketStatus == ESS_CONNECTTING ||
			m_nSocketStatus == ESS_ISCONNECTED)
		{
			m_nSocketStatus = ESS_DISCONNECTTING;
			
			m_SocketThread.ShutDown();

			bSuccess = TRUE;
		}
		else
		{
			//SetLastError(ERROR_INCORRECT_STATUS);
		}
		
		return bSuccess;
	}

    void CTCPClient::Release()
    {
        if (m_nSocketStatus == ESS_CONNECTTING ||
			m_nSocketStatus == ESS_ISCONNECTED)
        {
            Disconnect();
        }
        delete this;
    }
	
	int CTCPClient::CloseSocket()
	{
		//CALLSTACKGUARD;
		
		m_SocketThread.CloseSocket();
		return 0;
	}
	
	// Member Function
	void CTCPClient::CleanUp()
	{
		//CALLSTACKGUARD;
		
		if(m_nSocketStatus == ESS_ISCONNECTED || 
			m_nSocketStatus == ESS_CONNECTTING)
		{
			Disconnect();
		}
		
		CloseSocket();
		
		m_nSocketStatus = ESS_NOTPREPARED;
		
		if( IsWindow(m_hTCPWindow) )
		{
			MSG msg = {0};
			while(PeekMessage(&msg, m_hTCPWindow, TCPM_SOCKET_EVENT, TCPM_SOCKET_EVENT, PM_REMOVE))
			{				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			::DestroyWindow(m_hTCPWindow);
		}
		else
		{
		}
		m_pEventHandler = NULL;
	}
	
	void CTCPClient::FireOnConnectFail(int nErrorCode)
	{
		//CALLSTACKGUARD;
		
		
		if(m_pEventHandler != NULL)
		{
			m_pEventHandler->OnConnectFail(nErrorCode);
		}
	}
	
	void CTCPClient::FireOnDisconnect(int nErrorCode)
	{
		//CALLSTACKGUARD;
		
		
		if(m_pEventHandler != NULL)
		{
			m_pEventHandler->OnClosed(nErrorCode);
		}
	}
	
	void CTCPClient::ShowLastErrorInfo()
	{
		ShowLastErrorInfo(WSAGetLastError());
	}
	
	void CTCPClient::ShowLastErrorInfo(int nErrorCode)
	{
		LPTSTR lpszErrorDescribe = NULL;
		
		// 获取与nErrorCode对应的错误描述
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, 
			nErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpszErrorDescribe,
			0,
			NULL 
			);
		
		
		LocalFree(lpszErrorDescribe);
	}
	
	BOOL CTCPClient::CreateSocket()
	{
		//CALLSTACKGUARD;
		
		m_SocketThread.SetShareData(this, m_hTCPWindow);
		return TRUE;
	}
	
	// Event Interface
	void CTCPClient::OnConnect(int nErrorCode)
	{
		//CALLSTACKGUARD;
		
		
		if(m_nSocketStatus != ESS_CONNECTTING)
		{
		}
		
		if(nErrorCode == 0)
		{
			m_nSocketStatus = ESS_ISCONNECTED;
			
			if(m_pEventHandler != NULL)
			{
				m_pEventHandler->OnConnectSuccess();
			}
		}
		else
		{
			m_nSocketStatus = ESS_NOTCONNECTED;
			//ShowLastErrorInfo(nErrorCode);
			FireOnConnectFail(-1);
		}
	}
	
	void CTCPClient::OnRead(LPBYTE pBytes, int nLen)
	{
		//CALLSTACKGUARD;

		
		if(m_nSocketStatus != ESS_ISCONNECTED && 
			m_nSocketStatus != ESS_DISCONNECTTING)
		{
			//SetLastError(ERROR_INCORRECT_STATUS);
		}
		
		if(m_pEventHandler != NULL)
		{
			m_pEventHandler->OnReceived(pBytes, nLen);
		}
	}
	
	// Event interface
	
	void CTCPClient::OnClose()
	{
		//CALLSTACKGUARD;
				
		
		/*
		if(m_nSocketStatus != ESS_ISCONNECTED && 
		m_nSocketStatus != ESS_DISCONNECTTING)
		{
		OutputErrorInfo(TEXT("status error to onclose:%d\n"), m_nSocketStatus);
		}
		
		  m_nSocketStatus = ESS_NOTCONNECTED;
		  CloseSocket();
		FireOnDisconnect(0);*/
		
		if(m_nSocketStatus == ESS_CONNECTTING ||
			m_nSocketStatus == ESS_ISCONNECTED)
		{
			m_nSocketStatus = ESS_DISCONNECTTING;
			
			CloseSocket();
			FireOnDisconnect(0);
		}
		else
		{
			//SetLastError(ERROR_INCORRECT_STATUS);
		}
	}
	
	// 在多个IP中随机选择一个
	ULONG CTCPClient::SelectOneRandomIP(LPHOSTENT pHostent)
	{
		//CALLSTACKGUARD;
		
		
		int nIPCount = 0;
		
		for(int i=0; pHostent!=NULL && pHostent->h_addr_list[i]!= NULL; i++) 	
		{
			PSTR szIPString = NULL;
			szIPString=inet_ntoa (*(struct in_addr *)pHostent->h_addr_list[i]);
			
			nIPCount++;
		}
		
		
		// 此域名对应多个IP, 随机选择其中一个
		int nSelectIndex = 0;
		if(nIPCount > 1)
		{
			srand(time(0));
			nSelectIndex = rand() % nIPCount;
		}
		
		
		return ((LPIN_ADDR)pHostent->h_addr_list[nSelectIndex])->s_addr;
	}
	
	void CTCPClient::OnHostLookupSuccess(ULONG* pulIPs, int nIPNum)
	{
		//CALLSTACKGUARD;
		
		ULONG ulIP = 0;
		if (nIPNum > 0)
		{
			ulIP = pulIPs[rand() % nIPNum];
			Connect(ulIP, m_usPort);
		}
		else
		{
		}
	}
	void CTCPClient::OnHostLookupFail(int nErrorCode)
	{
		//CALLSTACKGUARD;
		
		FireOnConnectFail(nErrorCode);
	}

	void CTCPClient::SetLastErrorCode(DWORD dwErrCode)
	{
		if(dwErrCode != ERROR_SUCCESS)
		{
			SetLastError(dwErrCode);
		}
	}

    ITCPSocketObject* CreateSocketObject()
    {
        return new CTCPClient;
    }
};

