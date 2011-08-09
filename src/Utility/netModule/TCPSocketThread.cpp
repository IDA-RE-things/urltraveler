// EventSocketEx.cpp: implementation of the CTCPSocketThread class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)
// #define CRTDBG_MAP_ALLOC
// #include <stdlib.h>
//#include <crtdbg.h> 
#include "stdafx.h"
#include <assert.h>
#include "TcpSocketThread.h"
#include "TCPClient.h"
#include "process.h"

namespace NetModule
{
    static CInitClass sg_InitClass;

#define THREAD_RECV_LEN (10 * 1024)

    typedef struct tagThreadParam
    {
        CTCPSocketThread* pSocketThread;
        HWND              hTCPWindow;
        ULONG             ulIP;
        UINT              uPort;
    }THREADPARAM;


    CTCPSocketThread::CTCPSocketThread()
    {
        m_hSocket     = INVALID_SOCKET;
        m_bThreadExit = FALSE;
        m_hThread     = NULL;

        m_dwThreadID = 0;

        m_bSleep = FALSE;

        m_bConnecting = FALSE;

        memset(&m_stSocketOption, 0, sizeof(m_stSocketOption));

        //m_dwMode = 0;

        //m_dwBandWidth = 0;
    }

    CTCPSocketThread::~CTCPSocketThread()
    {
        CloseSocket();
    }

    //=============================================================================
    // ������ӳٶ�ģʽ����δ������sleep,��ȴ��߳̽���
    //=============================================================================
    void CTCPSocketThread::WaitThreadExit()
    {
        if ( m_hThread != NULL )
        {
            if ((!m_bConnecting)
                ||((m_stSocketOption.dwOption & SOCKETOPT_DELAYRECEIVE)
                   && !m_bSleep))
            {
                //Log(_T("CTCPSocketThread begin to WaitForSingleObject"));
                WaitForSingleObject(m_hThread, INFINITE);
                CloseHandle(m_hThread);

// 				TCHAR szLog[MAX_PATH] = {0};
// 				sprintf(szLog, _T("CTCPSocketThread Thread Self Exit 0x%x"), m_dwThreadID);
// 				OutputDebugString(szLog);
                //Log(_T("CTCPSocketThread WaitForSingleObject end"));
            }
            else
            {
                TerminateThread(m_hThread, 0);
// 				TCHAR szLog[MAX_PATH] = {0};
// 				sprintf(szLog, _T("CTCPSocketThread Terminate Thread 0x%x"), m_dwThreadID);
// 				OutputDebugString(szLog);
            }

//			CloseHandle(m_hThread);
            m_hThread = NULL;
        }
    }

    BOOL CTCPSocketThread::ShutDown()
    {
        CloseSocket();
        return TRUE;
    }

    BOOL CTCPSocketThread::Send(const void *pBuf, int nLen)
    {
        if (m_hSocket == INVALID_SOCKET)
        {
            //SetLastError(ERROR_INVALID_SOCKET);
            return FALSE;
        }

        if (m_bThreadExit == TRUE )
        {
            //SetLastError(ERROR_SOCKETTHREAD_EXIT);
            return FALSE;
        }

        int nSendLen = send(m_hSocket, (char*)pBuf, nLen, 0);

        //Log(_T("CTCPSocketThread::Send �� Sokcet = %d �������� nSendLen =%d, Ӧ������Ϊ nLen =%d\n"), m_hSocket, nSendLen,nLen);
        return( nSendLen == nLen ) ? TRUE : FALSE ;
    }

    void CTCPSocketThread::CloseSocket()
    {
        m_bThreadExit = TRUE;
        if ( m_hSocket != INVALID_SOCKET )
        {
            //closesocket(m_hSocket);
            //shutdown(m_hSocket, SD_BOTH);
        }
        WaitThreadExit();

    }

    void CTCPSocketThread::SetOption(PSOCKETOPTION pSocketOption)
    {
        if (pSocketOption != NULL)
        {
            memcpy(&m_stSocketOption, pSocketOption, sizeof(SOCKETOPTION));
        }
    }

// 	void CTCPSocketThread::SetMode(DWORD dwMode)
// 	{
// 		m_dwMode = dwMode;
// 	}
// 	
    void CTCPSocketThread::SetBandWidth(DWORD dwBandWidth)
    {
        if (m_stSocketOption.dwOption  & SOCKETOPT_DELAYRECEIVE)
        {
            CGuard guard(m_mutex);           
            m_stSocketOption.dwBandWidth = dwBandWidth;
        }
    }

    void CTCPSocketThread::GetBandWidth(DWORD& dwBandWidth)
    {
        if (m_stSocketOption.dwOption  & SOCKETOPT_DELAYRECEIVE)
        {
            CGuard guard(m_mutex);

            m_stSocketOption.dwBandWidth = dwBandWidth;
        }
    }

    //��Ϊ��Byte, ��ȡ��д�붼��ԭ�Ӳ�����������ͬ������
    void CTCPSocketThread::SetMaxPacketSize(BYTE byPacketSize)
    {
        if (m_stSocketOption.dwOption  & SOCKETOPT_CACHEPACKET)
        {
            m_stSocketOption.byMaxPacketSize = byPacketSize;
        }
    }

    DWORD CTCPSocketThread::GetSleepTime(DWORD dwSize, DWORD dwElapsedTime, 
                                         DWORD dwBandWidth)
    {
        DWORD dwSleepTime = 0;

        //GetBandWidth(dwBandWidth);

        if (dwBandWidth > 0)
        {
            // �յ�dwSize�ֽڴ�С��������Ҫ��ʱ��dwSleepTimeΪ dwSleepTime = 1000 * dwSize / (dwBandWidth * 1024)
            dwSleepTime = (DWORD)(dwSize / (dwBandWidth * 1.024));

            if (dwSleepTime > dwElapsedTime)
            {
                dwSleepTime = dwSleepTime - dwElapsedTime;
            }
            else
            {
                dwSleepTime = 0;
            }
        }

        return dwSleepTime;
    }

    void* CTCPSocketThread::Alloc(size_t nBuffLen)
    {
        void *pBuffer = NULL;

        // TODO ; AllocMemory(&pBuffer, nBuffLen, TRUE);	

        pBuffer = new BYTE[nBuffLen];

// 		TCHAR szLog[MAX_PATH] = {0};
// 
// 		sprintf(szLog, _T("Alloc Memory : �� 0x%x ��"), pBuffer);
// 
// 		OutputDebugString(szLog);

        return pBuffer;
    }

    void CTCPSocketThread::Free(void* pBuffer)
    {
        // TODO : ReleaseMemory(pBuffer);

        delete [] (LPBYTE)pBuffer;

// 		TCHAR szLog[MAX_PATH] = {0};
// 
// 		sprintf(szLog, _T("Free Memory : �� 0x%x ��"), pBuffer);
// 
// 		OutputDebugString(szLog);
    }

    BOOL CTCPSocketThread::Connect(ULONG ulIP, USHORT usPort)
    {
        BOOL bSuccess = TRUE;
        if ( ulIP == 0 )
        {
            bSuccess = FALSE;
            SetLastError(ERROR_INVALID_PARAMETER);
        }
        if ( bSuccess )
        {
            THREADPARAM *pThreadParam = (THREADPARAM*)Alloc(sizeof(THREADPARAM));
            if ( pThreadParam )
            {
                pThreadParam->pSocketThread  = this;
                pThreadParam->hTCPWindow     = m_hTCPWindow;
                pThreadParam->ulIP           = ulIP;
                pThreadParam->uPort          = usPort;  
                UINT dwThreadID = 0;
                m_hThread  = (HANDLE)_beginthreadex(
                                                   NULL, 
                                                   0,
                                                   ConnAndRecvThread,
                                                   LPVOID(pThreadParam), 
                                                   0, 
                                                   &dwThreadID);

                m_dwThreadID = dwThreadID;
            }
        }
        return bSuccess;
    }

    void CTCPSocketThread::AddPackageToList(CTCPSocketThread* pTCPSocketThread,
                                            HWND              hTCPWindow,
                                            TCPPACKAGEINFO*   pSocketInfo)
    {
        pSocketInfo->pTCPClient = pTCPSocketThread->m_pTCPClient;
        BOOL bSuccess = FALSE;
        bSuccess = ::IsWindow(hTCPWindow);
        if ( bSuccess )
        {
            bSuccess = ::PostMessage(hTCPWindow, TCPM_SOCKET_EVENT, (WPARAM)pSocketInfo, 0);
        }

        if ( !bSuccess )
        {
            //Log(_T("CTCPSocketThread::AddPackageToList PostMessage Failed!\n"));
            if ( pSocketInfo->pszBuffer )
            {
                Free(pSocketInfo->pszBuffer);
            }
            Free(pSocketInfo);
        }
    }

    BOOL CTCPSocketThread::PostPacket(
                                     CTCPSocketThread *pTCPSocketThread,
                                     HWND hTCPWindow,
                                     BYTE* pBuffer, 
                                     int nLen)
    {
        BOOL bSuccess = FALSE;
        TCPPACKAGEINFO* pstSocketPackInfo = 
        (TCPPACKAGEINFO*)Alloc(sizeof(TCPPACKAGEINFO));

        if (pstSocketPackInfo != NULL)
        {
            memset(pstSocketPackInfo, 0, sizeof(TCPPACKAGEINFO));

            pstSocketPackInfo->bBeginRecv = TRUE;
            pstSocketPackInfo->nLen       = nLen;
            pstSocketPackInfo->pszBuffer  = pBuffer;
            AddPackageToList(pTCPSocketThread, hTCPWindow, pstSocketPackInfo);
            bSuccess = TRUE;
        }

//  		TCHAR szLog[MAX_PATH] = {0};
//  		sprintf(szLog, _T("PostPacket, nLen = �� %d ��"), nLen);
//  		OutputDebugString(szLog);
        return bSuccess;
    }

    UINT CTCPSocketThread::ConnAndRecvThread(LPVOID lParam)
    {   
        THREADPARAM stThreadPara  = *(THREADPARAM*)lParam;
        Free(lParam);

        CTCPSocketThread* pSocketThread = stThreadPara.pSocketThread;
        SOCKETOPTION&     stSocketOption = pSocketThread->m_stSocketOption;
        SOCKETOPTION      stSocketOptionCopy = {0};
        BOOL              bDelayReceive = (stSocketOption.dwOption & SOCKETOPT_DELAYRECEIVE)
                                          && (stSocketOption.dwBandWidth > 0);
        BOOL              bCachePacket = (stSocketOption.dwOption & SOCKETOPT_CACHEPACKET)
                                         && (stSocketOption.byMaxPacketSize * 1000 > THREAD_RECV_LEN);
        HWND              hTCPWindow    = stThreadPara.hTCPWindow;
        sockaddr_in       sServeraddr   = {0};
        SOCKET            hSocket       = INVALID_SOCKET;
        TCPPACKAGEINFO*   pstSocketPackInfo = NULL;
        BYTE*             pCachedBuffer = NULL;
        int               nCacheDataSize = 0;

        DWORD             dwSleepTime = 0;
        DWORD             dwBeginTime = 0;                  // �����ٶ�ʱ����, ����Sleepʱ��
        DWORD             dwNowTime = 0;                    // �����ٶ�ʱ���ã���ʼ��������ʱ��
        DWORD             dwDownloadSize = 0;               // ���յ����ݴ�С
        DWORD             dwCachePacketBeginTime = 0;       // ����CachePacket��ʼʱ��

        DWORD             dwBandWidth = 0;
        //TCHAR			  szLog[MAX_PATH] = {0}; // test

        hSocket                     = socket(AF_INET, SOCK_STREAM, 0);
        sServeraddr.sin_family      = AF_INET;
        sServeraddr.sin_addr.s_addr = stThreadPara.ulIP;
        sServeraddr.sin_port        = htons((USHORT)stThreadPara.uPort);

        if ( pSocketThread->m_bThreadExit )
        {
            closesocket(hSocket);       
            return 0;
        }

        BOOL bConnected = FALSE;
        pSocketThread->m_bConnecting = TRUE;
        //Log(_T("socketthread start connect ip %s!\n"), inet_ntoa(*(struct in_addr*)&stThreadPara.ulIP));
        if ( connect(hSocket, (LPSOCKADDR)&sServeraddr, sizeof(sockaddr_in)) == SOCKET_ERROR )
        {
            //Log(_T("socketthread connect ip %s failed!\n"), inet_ntoa(*(struct in_addr*)&stThreadPara.ulIP));
            bConnected = FALSE;
        }
        else
        {
            //OutputDebugString(_T("connect success"));
            //Log(_T("socketthread connect ip %s success!"), inet_ntoa(*(struct in_addr*)&stThreadPara.ulIP));
            bConnected = TRUE;
        }

        pSocketThread->m_bConnecting = FALSE;

        if ( pSocketThread->m_bThreadExit )
        {
            closesocket(hSocket);       
            return 0;
        }
        pSocketThread->m_hSocket = hSocket;

        pstSocketPackInfo = 
        (TCPPACKAGEINFO*)Alloc(sizeof(TCPPACKAGEINFO));

        if ( !pstSocketPackInfo )
        {
            return 0;
        }

        memset(pstSocketPackInfo, 0, sizeof(TCPPACKAGEINFO));
        pstSocketPackInfo->bConnected = bConnected;
        AddPackageToList(pSocketThread,
                         hTCPWindow,
                         pstSocketPackInfo);
        pstSocketPackInfo= NULL;

        if (bConnected == FALSE)
        {
            //����ʧ��
            return 0;
        }

        // ���д������ɶ���,ע��֮
        //int nRecvTimeout = 200;
        //setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*) &nRecvTimeout, sizeof(int));

        //����closesocketʱ������ͻ��廹�����ݣ�
        //�����һ���ʱ����������
        //ͨ����Ϊ��leaveroom��Ϣ������������
        LINGER stLinger = {0};
        stLinger.l_onoff = 1;
        stLinger.l_linger = 1;
        setsockopt(hSocket,SOL_SOCKET,SO_LINGER,(const char*)&stLinger,sizeof(stLinger));

        //���ӳɹ����ȴ�����
        char szBuffer[THREAD_RECV_LEN] = {0};
        BOOL bExit = FALSE;

        fd_set stFdRead = {0};
        timeval stSelectTime = {0};
        stSelectTime.tv_sec = 0;
        stSelectTime.tv_usec = 500;

        if (bDelayReceive)
        {
            dwBeginTime = GetTickCount();
        }

        if (bCachePacket)
        {
            dwCachePacketBeginTime = GetTickCount();
        }

        while (!bExit)
        {
            //����select�жϣ����socket���Ƿ��пɶ�����
            FD_ZERO(&stFdRead);
            FD_SET(hSocket, &stFdRead);

            int nSelectResult = select(0, &stFdRead, NULL, NULL, &stSelectTime);

            stSocketOptionCopy = stSocketOption;
            BOOL bCachePacketAdjusted = (stSocketOptionCopy.dwOption & SOCKETOPT_CACHEPACKET)
                                        && (stSocketOptionCopy.byMaxPacketSize * 1000 > THREAD_RECV_LEN);

            // ����������ͻ����
            // 1 ���泬ʱ
            // 2 �������ݴ��ڸı���MaxPacketSize
            // 3 ԭ��CachePacket��Ч������ȡ��CachePacket
            if (bCachePacket
                && nCacheDataSize > 0
                && pCachedBuffer != NULL
                && (GetTickCount() - dwCachePacketBeginTime > CACHEPACKET_TIMEOUT 
                    || nCacheDataSize >= stSocketOptionCopy.byMaxPacketSize * 1000
                    || !bCachePacketAdjusted))
            {
                if (!PostPacket(pSocketThread, hTCPWindow, pCachedBuffer, nCacheDataSize))
                {
                    break;
                }

                //sprintf(szLog, _T("time : �� %d ��, PostPackets"), GetTickCount() - dwCachePacketBeginTime);
                //OutputDebugString(szLog);
                pCachedBuffer = NULL;
                nCacheDataSize = 0;
                dwCachePacketBeginTime = GetTickCount();
            }

            bCachePacket = bCachePacketAdjusted;

            int nLen = 0;
            int nSockErr = 0;
            if (nSelectResult != SOCKET_ERROR &&
                !FD_ISSET(hSocket, &stFdRead)) //Socketû��������û���ݰ�����
            {
                bExit = pSocketThread->m_bThreadExit;
                if (!bExit)
                {
                    //���û�п��Խ��յ����ݣ�����ѭ�����
                    continue;
                }
            }
            else if (nSelectResult == SOCKET_ERROR)  //Socket����
            {
                // ShowThreadTrackInfo(hSocket, _T("select error"));
            }
            else //�����ݰ�����
            {
                memset(szBuffer, 0, sizeof(szBuffer));

                if (bDelayReceive)
                {
                    dwNowTime = GetTickCount();

                    pSocketThread->GetBandWidth(dwBandWidth);

                    dwSleepTime = GetSleepTime(
                                              dwDownloadSize, 
                                              dwNowTime - dwBeginTime,
                                              dwBandWidth);

                    if (dwSleepTime > 0)
                    {
                        pSocketThread->m_bSleep = TRUE;
                        Sleep(dwSleepTime);
                        pSocketThread->m_bSleep = FALSE;
                    }
                }
                nLen = recv(hSocket, szBuffer, sizeof(szBuffer), 0);
                dwDownloadSize += nLen;

                nSockErr = WSAGetLastError();
                if ( pSocketThread->m_bThreadExit)
                {
                    bExit = TRUE;
                }
                else
                {
                    if (nLen == SOCKET_ERROR && nSockErr == WSAETIMEDOUT)
                    {
                        continue;
                    }
                }
            }

            if ( nLen > 0 && !bExit && IsWindow(hTCPWindow))
            {
                // Cache Packet
                if (bCachePacket)
                {
                    BOOL bDataCached = FALSE;
                    if (nCacheDataSize + nLen >= stSocketOptionCopy.byMaxPacketSize * 1000)
                    {
                        if (nCacheDataSize + nLen == stSocketOptionCopy.byMaxPacketSize * 1000)
                        {
                            memcpy(pCachedBuffer + nCacheDataSize, szBuffer, nLen);
                            nCacheDataSize += nLen;

                            bDataCached = TRUE;
                        }

                        //��ֹ���Ϳ�����
                        if (pCachedBuffer != NULL && nCacheDataSize > 0)
                        {
                            if (!PostPacket(pSocketThread, hTCPWindow, pCachedBuffer, nCacheDataSize))
                            {
                                break;
                            }
                            pCachedBuffer = NULL;
                            nCacheDataSize = 0;
                        }
                        dwCachePacketBeginTime = GetTickCount();
                    }

                    if (!bDataCached)
                    {
                        if (pCachedBuffer == NULL)
                        {
                            pCachedBuffer = (BYTE*)Alloc(stSocketOptionCopy.byMaxPacketSize * 1000);

                            if (pCachedBuffer == NULL)
                            {
                                break;
                            }
                            else
                            {
                                memset(pCachedBuffer, 0, stSocketOptionCopy.byMaxPacketSize * 1000);
                            }
                        }

                        if (pCachedBuffer != NULL)
                        {
                            memcpy(pCachedBuffer + nCacheDataSize, szBuffer, nLen);
                            nCacheDataSize += nLen;
                        }
                    }
                }
                else
                {
                    BYTE* pBuffer = (BYTE*)Alloc(nLen);

                    if (pBuffer != NULL)
                    {
                        memset(pBuffer, 0, nLen);
                        memcpy(pBuffer, szBuffer, nLen);
                    }

                    if (pBuffer == NULL || !PostPacket(pSocketThread, hTCPWindow, pBuffer, nLen))
                    {
                        break;
                    }
                }
            }
            else
            {
                if (bCachePacket
                    && nCacheDataSize > 0
                    && pCachedBuffer != NULL)
                {
                    if (!PostPacket(pSocketThread, hTCPWindow, pCachedBuffer, nCacheDataSize))
                    {
                        break;
                    }
                    pCachedBuffer = NULL;
                    nCacheDataSize = 0;
                    dwCachePacketBeginTime = GetTickCount();
                    //OutputDebugString(_T("3 nCacheDataSize = 0"));
                }

                pstSocketPackInfo = 
                (TCPPACKAGEINFO*)Alloc(sizeof(TCPPACKAGEINFO));

                if ( !pstSocketPackInfo )
                {
                    break;
                }
                memset(pstSocketPackInfo, 0, sizeof(TCPPACKAGEINFO));

                pstSocketPackInfo->bBeginRecv = TRUE;
                pstSocketPackInfo->bClose     = TRUE;

                AddPackageToList(pSocketThread, hTCPWindow, pstSocketPackInfo);
                pstSocketPackInfo = NULL;
                break;
            }
        }

        pSocketThread->m_hSocket = INVALID_SOCKET;
        closesocket(hSocket);

        return 0;
    }

    void CTCPSocketThread::SetShareData(CTCPClient* pTCPClient, HWND hTCPWindow)
    {
        m_pTCPClient = pTCPClient;
        m_hTCPWindow = hTCPWindow;
    }

};  

