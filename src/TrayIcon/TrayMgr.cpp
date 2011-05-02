#include "StdAfx.h"
#include "SndaBase.h"
#include "TrayMgr.h"
#include <algorithm>
#include "resource.h"
#include "tchar.h"

enum
{
	eTimerId_TrayFlash = 0,
	eTimerInterval_TrayFlash = 350,

	eTimerId_CheckTrayDblClk = 1,
	eTimerInterval_CheckTrayDblClk = 300,
};

enum
{
	WM_DefTrayCallbackMsgId = WM_USER + 1001,
};

static const TCHAR WndClassForTrayWnd[]  = _T("WndClassForTrayWnd");

static CTrayMgr* g_pThis = NULL;

HICON CTrayMgr::m_hTrayBlank = NULL;

CTrayMgr::CTrayMgr()
{
	memset(&m_nid, 0, sizeof m_nid);
	m_bFlashing = FALSE;
	m_hwndTray = NULL;

	m_eTNMode = eTNMode_Null;
	m_bWaitForDblClk = FALSE;
	m_eTFSMode = eTFSMode_DoubleClick;
	m_uTrayFlashTimerId = -1;
}

CTrayMgr::~CTrayMgr(void)
{
	if (m_nid.cbSize)
	{
		Delete();
	}

	DestroyWindow(m_hwndTray);
	m_hwndTray = NULL;
}

LRESULT CTrayMgr::TrayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DefTrayCallbackMsgId)
	{
		if (lParam == WM_LBUTTONDOWN)
		{
			SetTimer(hWnd, eTimerId_CheckTrayDblClk, eTimerInterval_CheckTrayDblClk, NULL);
			g_pThis->m_bWaitForDblClk = TRUE;
		
			if (g_pThis->m_bFlashing)
			{
				if (g_pThis->m_eTFSMode == eTFSMode_Click)
				{
					//SendMessage(hWnd, WM_DefTrayCallbackMsgId, g_pThis->m_stackFlash.back().dwParam, WM_TRAYFLASHSTOPPED);
					g_pThis->FireOnTrayEvent(g_pThis->m_stackFlash.back().dwParam, WM_TRAYFLASHSTOPPED);
					g_pThis->StopFlash();

					if (!g_pThis->IsFlashing())
					{
						g_pThis->FireOnTrayEvent(0, WM_TRAYALLFLASHSTOPPED);
					}
				}
			}
			else
			{
				g_pThis->FireOnTrayEvent(wParam, lParam);
			}
		}
		else if (lParam == WM_LBUTTONDBLCLK)
		{
			KillTimer(hWnd, eTimerId_CheckTrayDblClk);
			g_pThis->m_bWaitForDblClk = FALSE;

			if (g_pThis->m_bFlashing)
			{
				if (g_pThis->m_eTFSMode == eTFSMode_DoubleClick)
				{
					g_pThis->FireOnTrayEvent(g_pThis->m_stackFlash.back().dwParam, WM_TRAYFLASHSTOPPED);
					g_pThis->StopFlash();

					if (!g_pThis->IsFlashing())
					{
						g_pThis->FireOnTrayEvent(0, WM_TRAYALLFLASHSTOPPED);
					}
				}
			}
			else
			{
				g_pThis->FireOnTrayEvent(wParam, lParam);
			}
		}
		else if (lParam == WM_LBUTTONDOWN_LATE || lParam == WM_RBUTTONDOWN 
			|| lParam == WM_RBUTTONUP || lParam == WM_MOUSEMOVE_POST)
		{
			g_pThis->FireOnTrayEvent(wParam, lParam);
		}
		else if (lParam == WM_MOUSEMOVE)
		{
			PostMessage(hWnd, WM_DefTrayCallbackMsgId, 0, WM_MOUSEMOVE_POST);
		}
	}
	else if (message == WM_TIMER)
	{
		UINT nIDEvent = wParam;

		if (nIDEvent == eTimerId_CheckTrayDblClk)
		{
			if (g_pThis->m_bWaitForDblClk)
			{
				KillTimer(hWnd, nIDEvent);
				g_pThis->m_bWaitForDblClk = FALSE;

				SendMessage(hWnd, WM_DefTrayCallbackMsgId, 0, WM_LBUTTONDOWN_LATE);
			}
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM CTrayMgr::RegisterClassForTrayWnd()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= 0;
	wcex.lpfnWndProc	= (WNDPROC)TrayWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= NULL;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= (LPCTSTR)NULL;
	wcex.lpszClassName	= WndClassForTrayWnd;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

void CTrayMgr::Init(HWND hwndAttach/* = No_Care_HWND*/, ETrayNotifyMode eTNMode/* = eTNMode_Event*/, 
		    ETrayFlashStopMode eTFSMode/* = eTFSMode_DoubleClick*/)
{
	m_eTNMode = eTNMode;
	m_eTFSMode = eTFSMode;

	if (eTNMode == eTNMode_Event)
	{
		if (!g_pThis)
		{
			g_pThis = this;
		}

		ASSERT(hwndAttach == No_Care_HWND);

		RegisterClassForTrayWnd();

		m_hwndTray = CreateWindow(WndClassForTrayWnd, NULL, WS_POPUP, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
		ASSERT(m_hwndTray);

		m_nid.hWnd = m_hwndTray;
	}
	else
	{
		ASSERT(hwndAttach != No_Care_HWND);
		ASSERT(hwndAttach);

		m_nid.hWnd = hwndAttach;
	}
}

BOOL CTrayMgr::Add(HICON hicon, LPCTSTR szTip, UINT uCallbackMsgId/* = No_Care*/)
{
	BOOL bRet = FALSE;

	ASSERT(m_nid.hWnd);
	ASSERT(!m_bFlashing);

	if (!m_nid.cbSize)
	{// no exist yet
		m_nid.cbSize = sizeof m_nid;
		m_nid.uFlags |= NIF_TIP | NIF_ICON | NIF_MESSAGE;
		m_nid.hIcon = hicon;
		m_nid.uID = 0;
		if (uCallbackMsgId == No_Care)
		{
			ASSERT(m_eTNMode == eTNMode_Event);
			m_nid.uCallbackMessage = WM_DefTrayCallbackMsgId;
		}
		else
		{
			m_nid.uCallbackMessage = uCallbackMsgId;
		}
		_tcsncpy(m_nid.szTip, szTip, _ARRAYSIZE( m_nid.szTip));

		if (!Shell_NotifyIcon(NIM_ADD, &m_nid))
		{
			//AfxMessageBox(L"Shell_NotifyIcon(NIM_ADD ʧ��");
		}
	}
	else
	{// exist already
		Change(hicon, szTip, uCallbackMsgId);
	}

	return bRet;
}

BOOL CTrayMgr::InnerChange(HICON hicon/* = Use_Current_HICON*/, LPCTSTR szTip/* = Use_Current_SZ*/,
			   UINT uCallbackMsgId/* = Use_Current*/)
{
	BOOL bRet = FALSE;
	
	ASSERT(m_nid.cbSize && m_nid.hWnd);

	BOOL bChangeIcon = (hicon != Use_Current_HICON);
	BOOL bChangeMsgId = (uCallbackMsgId != Use_Current);
	BOOL bChangeTip = (szTip != Use_Current_SZ);

	if (bChangeIcon)
	{
		m_nid.uFlags |= NIF_ICON;
	}
	else
	{
		m_nid.uFlags &= ~NIF_ICON;
	}

	if (bChangeMsgId)
	{
		m_nid.uFlags |= NIF_MESSAGE;
	}
	else
	{
		m_nid.uFlags &= ~NIF_MESSAGE;
	}

	if (bChangeTip)
	{
		m_nid.uFlags |= NIF_TIP;
	}
	else
	{
		m_nid.uFlags &= ~NIF_TIP;
	}
	
	if (bChangeIcon)
	{
		m_nid.hIcon = hicon;
	}
	if (bChangeMsgId)
	{
		m_nid.uCallbackMessage = uCallbackMsgId;
	}
	
	if (bChangeTip)
	{
		_tcscpy(m_nid.szTip, szTip);
	}

	bRet = Shell_NotifyIcon(NIM_MODIFY, &m_nid);

	if(!bRet)
	{
	}

	return bRet;
}

BOOL CTrayMgr::ShowTips(bool bShow)
{
	if(bShow)
	{
		_tcsncpy(m_nid.szTip, m_strCacheTipsString.c_str(), _ARRAYSIZE( m_nid.szTip));

		return ChangeTip(m_nid.szTip);
	}
	else
	{
		m_strCacheTipsString = m_nid.szTip;
		m_nid.szTip[0] = 0;

		return ChangeTip(m_nid.szTip);
	}
}

BOOL CTrayMgr::Delete(BOOL bClearStatus/* = false*/)
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
	if (bClearStatus)
	{
		memset(&m_nid, 0, sizeof m_nid);
	}

	return TRUE;
}


BOOL CTrayMgr::Flash(HICON hiconNew/* = Use_Current_HICON*/, DWORD dwParam/* = 0*/)
{
	if(hiconNew == NULL)
	{
		return FALSE;
	}

	if (!g_pThis)
	{
		g_pThis = this;
	}
	
	if (hiconNew == Use_Current_HICON && !m_nid.hIcon)
	{
		ASSERT(0);
		return FALSE;
	}

	if (hiconNew != Use_Current_HICON && !m_nid.cbSize)
	{
		ASSERT(0);
		return FALSE;
	}

	STrayFlashStackItem item;
	item.hicon = hiconNew == Use_Current_HICON ? m_nid.hIcon : hiconNew;
	item.dwParam = dwParam;
	m_stackFlash.push_back(item);
	Sort();

	if (!m_bFlashing)
	{
		m_uTrayFlashTimerId = SetTimer(NULL, eTimerId_TrayFlash, eTimerInterval_TrayFlash, CTrayMgr::OnFlashTimer);
		m_bFlashing = TRUE;
	}

	FireOnFlashStart(dwParam);

	return TRUE;
}

void CALLBACK CTrayMgr::OnFlashTimer(HWND hwnd, UINT uMsg, UINT nEvent, DWORD dwTime)
{
	static BOOL bToogle = TRUE;

	if (g_pThis->m_bFlashing)
	{
		bool bNeedChangeSilenceIcon = true;

/*
		bNeedChangeSilenceIcon = AfxGetMainWnd()->SendMessage(0x9843);

		if (bNeedChangeSilenceIcon)
		{
			HICON &h = g_pThis->m_stackFlash.back().hicon;
			g_pThis->ChangeIcon(bToogle ? g_pThis->m_stackFlash.back().hicon : m_hTrayBlank);
			bToogle = !bToogle;
		}
*/
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CTrayMgr::StopFlash(int nIndex/* = -1*/)
{
	if (!m_stackFlash.empty())
	{
		if (nIndex != -1)
		{
			g_pThis->FireOnTrayEvent(m_stackFlash[nIndex].dwParam, WM_TRAYFLASHSTOPPED);
		}

		if (nIndex == -1 || nIndex == m_stackFlash.size() - 1)
		{
			m_stackFlash.pop_back();

			if (!m_stackFlash.size())
			{
				KillTimer(NULL, m_uTrayFlashTimerId);
				m_bFlashing = FALSE;

				g_pThis->FireOnTrayEvent(0, WM_TRAYALLFLASHSTOPPED);
			}
		}
		else
		{
			m_stackFlash.erase(m_stackFlash.begin() + nIndex);
		}
	}
	else
	{
		ASSERT(0);
	}

	return TRUE;
}

BOOL CTrayMgr::StopAllFlash()
{
	if (!m_stackFlash.empty())
	{
		KillTimer(NULL, m_uTrayFlashTimerId);
		while (!m_stackFlash.empty())
		{
			m_stackFlash.pop_back();
		}

		m_bFlashing = FALSE;
		g_pThis->FireOnTrayEvent(0, WM_TRAYALLFLASHSTOPPED);
	}
	else
	{
		ASSERT(0);
	}

	return TRUE;
}

void CTrayMgr::AddEventHandler(ITrayEvent* pHandler)
{
	if (m_vecTrayEventHandler.empty() ||
		find(m_vecTrayEventHandler.begin(), m_vecTrayEventHandler.end() -1, pHandler) == m_vecTrayEventHandler.end()
		)
	{
		m_vecTrayEventHandler.push_back(pHandler);
	}
	
}

void CTrayMgr::DeleteEventHandler(ITrayEvent* pHandler)
{
	if (!m_vecTrayEventHandler.empty())
	{
		vector<ITrayEvent*>::iterator it = find(m_vecTrayEventHandler.begin(), m_vecTrayEventHandler.end() -1, pHandler);
		if (it != m_vecTrayEventHandler.end())
		{
			m_vecTrayEventHandler.erase(it);
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		ASSERT(0);
	}
}

void CTrayMgr::FireOnTrayEvent(WPARAM w, LPARAM l)
{
	int size = m_vecTrayEventHandler.size();
	for (vector<ITrayEvent*>::iterator it = m_vecTrayEventHandler.begin(); it != m_vecTrayEventHandler.end(); it++)
	{
		(*it)->OnTrayEvent(w, l);

		if (l == WM_TRAYFLASHSTOPPED)
		{
			FireOnFlashEnd(w);
		}
	}
}

void CTrayMgr::FireOnFlashStart(DWORD dwParam)
{
	int size = (int)m_vecTrayEventHandler.size();
	for (vector<ITrayEvent*>::iterator it = m_vecTrayEventHandler.begin(); it != m_vecTrayEventHandler.end(); it++)
	{
		(*it)->OnFlashStart(dwParam);
	}
}

void CTrayMgr::FireOnFlashEnd(DWORD dwParam)
{
	int size = (int)m_vecTrayEventHandler.size();
	for (vector<ITrayEvent*>::iterator it = m_vecTrayEventHandler.begin(); it != m_vecTrayEventHandler.end(); it++)
	{
		(*it)->OnFlashEnd(dwParam);
	}
}

HWND CTrayMgr::GetWindowHandle()
{
	return m_hwndTray;
}



/** �������ؽ�ʱ���õĺ���
Ŀǰ��Ҫ�����ؽ�����������ͼ��
@return BOOL,eturn TRUE:�ɹ�, FALSE:ʧ��
����:�»���  ����:2008-5-8
*/
BOOL CTrayMgr::ReAdd(HICON hicon, LPCTSTR szTip, UINT uCallbackMsgId)
{
	BOOL bRet = FALSE;

	ASSERT(m_nid.hWnd);

	m_nid.cbSize = sizeof m_nid;
	m_nid.uFlags |= NIF_TIP | NIF_ICON | NIF_MESSAGE;
	m_nid.hIcon = hicon;
	m_nid.uID = 0;
	if (uCallbackMsgId == No_Care)
	{
		ASSERT(m_eTNMode == eTNMode_Event);
		m_nid.uCallbackMessage = WM_DefTrayCallbackMsgId;
	}
	else
	{
		m_nid.uCallbackMessage = uCallbackMsgId;
	}
	_tcsncpy(m_nid.szTip, szTip, _ARRAYSIZE( m_nid.szTip));

	bRet = Shell_NotifyIcon(NIM_ADD, &m_nid);	

	return bRet;
}

void CTrayMgr::FakePickMsg()
{
	if (g_pThis->m_bFlashing)
	{
		g_pThis->FireOnTrayEvent(g_pThis->m_stackFlash.back().dwParam, WM_TRAYFLASHSTOPPED);
		g_pThis->StopFlash();

		if (!g_pThis->IsFlashing())
		{
			g_pThis->FireOnTrayEvent(0, WM_TRAYALLFLASHSTOPPED);
		}
	}
	else
	{
		g_pThis->FireOnTrayEvent(0, WM_LBUTTONDOWN);
	}
}

void CTrayMgr::FakePickAllMsg()
{
	if (g_pThis->m_bFlashing)
	{
		while(g_pThis->m_bFlashing)
		{
			g_pThis->FireOnTrayEvent(g_pThis->m_stackFlash.back().dwParam, WM_TRAYFLASHSTOPPED);
			g_pThis->StopFlash();

			if (!g_pThis->IsFlashing())
			{
				g_pThis->FireOnTrayEvent(0, WM_TRAYALLFLASHSTOPPED);
			}
		}
	}
	else
	{
		g_pThis->FireOnTrayEvent(0, WM_LBUTTONDBLCLK);
	}
}

void CTrayMgr::ClearAllFlash()
{
	g_pThis->m_stackFlash.clear();
	g_pThis->m_bFlashing = false;

	KillTimer(NULL, m_uTrayFlashTimerId);
	m_bFlashing = FALSE;

	g_pThis->FireOnTrayEvent(0, WM_TRAYALLFLASHSTOPPED);
}

void CTrayMgr::Sort()
{
	
}