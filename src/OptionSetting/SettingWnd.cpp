#include "stdafx.h"
#include "SettingWnd.h"

#include "MiscHelper.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "XString.h"
#include "OptionCenter.h"
#include "SettingModule.h"

CSettingWnd::CSettingWnd()
{
	m_eCurrentTab	=	TAB_NORMAL;
}

CSettingWnd::~CSettingWnd()
{
}

void	CSettingWnd::PrepareCommon()
{
	COptionUI* pAutoStart = static_cast<COptionUI*>(m_pm.FindControl(_T("AutoStart")));
	pAutoStart->Selected(OPTION_CENTER.m_bAutoStart);

	COptionUI* pAutoLogin = static_cast<COptionUI*>(m_pm.FindControl(_T("AutoLogin")));
	pAutoLogin->Selected(OPTION_CENTER.m_bAutoLogin);

	COptionUI* pRememberPasswd = static_cast<COptionUI*>(m_pm.FindControl(_T("RememberPwd")));
	pRememberPasswd->Selected(OPTION_CENTER.m_bRememberPwd);

	COptionUI* pMinWnd = static_cast<COptionUI*>(m_pm.FindControl(_T("MinWnd")));
	COptionUI* pCloseWnd = static_cast<COptionUI*>(m_pm.FindControl(_T("CloseWnd")));
	if( OPTION_CENTER.m_bMinToTray == TRUE)
		pMinWnd->Selected(TRUE);
	else
		pCloseWnd->Selected(TRUE);

	COptionUI* pAutoLocalSync = static_cast<COptionUI*>(m_pm.FindControl(_T("AutoLocalSync")));
	pAutoLocalSync->Selected(OPTION_CENTER.m_bAutoLocalSync);

	COptionUI* pAutoRemoteSync = static_cast<COptionUI*>(m_pm.FindControl(_T("AutoRemoteSync")));
	pAutoRemoteSync->Selected(OPTION_CENTER.m_bAutoRemotingSync);
}

void	CSettingWnd::SaveCommon()
{
		COptionUI* pAutoStart = static_cast<COptionUI*>(m_pm.FindControl(_T("AutoStart")));
		OPTION_CENTER.m_bAutoStart = pAutoStart->IsSelected();

		COptionUI* pAutoLogin = static_cast<COptionUI*>(m_pm.FindControl(_T("AutoLogin")));
		OPTION_CENTER.m_bAutoLogin = pAutoLogin->IsSelected();

		COptionUI* pRememberPasswd = static_cast<COptionUI*>(m_pm.FindControl(_T("RememberPwd")));
		OPTION_CENTER.m_bRememberPwd = pRememberPasswd->IsSelected();

		COptionUI* pMinWnd = static_cast<COptionUI*>(m_pm.FindControl(_T("MinWnd")));
		OPTION_CENTER.m_bMinToTray = pMinWnd->IsSelected();

		COptionUI* pAutoLocalSync = static_cast<COptionUI*>(m_pm.FindControl(_T("AutoLocalSync")));
		OPTION_CENTER.m_bAutoLocalSync = pAutoLocalSync->IsSelected();

		COptionUI* pAutoRemoteSync = static_cast<COptionUI*>(m_pm.FindControl(_T("AutoRemoteSync")));
		OPTION_CENTER.m_bAutoRemotingSync = pAutoRemoteSync->IsSelected();

		OPTION_CENTER.SaveSetting();
		OPTION_CENTER.m_bNormalSettingChange = FALSE;
}

void	CSettingWnd::PrepareUpdate()
{
	COptionUI* pUpateAutoComplete = static_cast<COptionUI*>(m_pm.FindControl(_T("UpdateAutoComplete")));
	COptionUI* pUpdateInstallHint = static_cast<COptionUI*>(m_pm.FindControl(_T("UpdateInstallHint")));
	COptionUI* pUpdateHint = static_cast<COptionUI*>(m_pm.FindControl(_T("UpdateHint")));
	COptionUI* pUpdateClose = static_cast<COptionUI*>(m_pm.FindControl(_T("UpdateClose")));

	if(OPTION_CENTER.m_eUpdateType == UPDATE_AUTO) 
		pUpateAutoComplete->Selected(TRUE);
	else if( OPTION_CENTER.m_eUpdateType == UPDATE_INSTALL_QUERY  )
		pUpdateInstallHint->Selected(TRUE);
	else if( OPTION_CENTER.m_eUpdateType == UPDATE_TIP  )
		pUpdateHint->Selected(TRUE);
	else if( OPTION_CENTER.m_eUpdateType == UPDATE_CLOSE  )
		pUpdateClose->Selected(TRUE);
}

void	CSettingWnd::SaveUpdate()
{
	COptionUI* pUpateAutoComplete = static_cast<COptionUI*>(m_pm.FindControl(_T("UpdateAutoComplete")));
	COptionUI* pUpdateInstallHint = static_cast<COptionUI*>(m_pm.FindControl(_T("UpdateInstallHint")));
	COptionUI* pUpdateHint = static_cast<COptionUI*>(m_pm.FindControl(_T("UpdateHint")));
	COptionUI* pUpdateClose = static_cast<COptionUI*>(m_pm.FindControl(_T("UpdateClose")));

	if( pUpateAutoComplete->IsSelected())
		OPTION_CENTER.m_eUpdateType = UPDATE_AUTO;
	else if(pUpdateInstallHint->IsSelected())
		OPTION_CENTER.m_eUpdateType = UPDATE_INSTALL_QUERY;
	else if(pUpdateHint->IsSelected())
		OPTION_CENTER.m_eUpdateType = UPDATE_TIP;
	else if(pUpdateClose->IsSelected())
		OPTION_CENTER.m_eUpdateType = UPDATE_CLOSE;

	OPTION_CENTER.SaveSetting();
	OPTION_CENTER.m_bUpdateSettingChange = FALSE;
}

void	CSettingWnd::PrepareProxy()
{
}

void	CSettingWnd::SaveProxy()
{
}

void CSettingWnd::OnPrepare(TNotifyUI& msg) 
{ 
	PrepareCommon();
	PrepareProxy();
	PrepareUpdate();
}

void	CSettingWnd::OnBtnOK()
{
	OnBtnApply();
	Close();
}

void	CSettingWnd::OnBtnApply()
{
	if( m_eCurrentTab == TAB_NORMAL)
	{
		SaveCommon();
	}
	else if( m_eCurrentTab == TAB_PROXY)
	{
		SaveProxy();
	}
	if( m_eCurrentTab == TAB_UPDATE)
	{
		SaveUpdate();
	}
}

void	CSettingWnd::OnBtnClose()
{
	if( m_eCurrentTab == TAB_NORMAL )
	{
		if( OPTION_CENTER.m_bNormalSettingChange == TRUE)
		{
			int nRet = ::MessageBoxW(NULL, L"您已经修改了设置，需要保存吗?",L"提示", MB_OKCANCEL);
			if( nRet == MB_OK)
				OnBtnApply();
		}
	}
	else if( m_eCurrentTab == TAB_PROXY )
	{
		if( OPTION_CENTER.m_bProxySettingChange == TRUE)
		{
			int nRet = ::MessageBoxW(NULL, L"您已经修改了设置，需要保存吗?",L"提示", MB_OKCANCEL);
			if( nRet == MB_OK)
				OnBtnApply();
		}
	}
	else if( m_eCurrentTab == TAB_UPDATE )
	{
		if( OPTION_CENTER.m_bUpdateSettingChange == TRUE)
		{
			int nRet = ::MessageBoxW(NULL, L"您已经修改了设置，需要保存吗?",L"提示", MB_OKCANCEL);
			if( nRet == MB_OK)
				OnBtnApply();
		}
	}

	Close();
}

void CSettingWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") )
	{
		OnPrepare(msg);
	}
	else if( msg.sType == _T("click") ) 
	{
		// 关闭按钮
		if( msg.pSender->GetName() == L"closebtn" || msg.pSender->GetName() == L"Cancel") 
		{
			OnBtnClose();
		}
		else if(msg.pSender->GetName() == L"OK")
		{
			OnBtnOK();
		}
		// 应用
		else if(msg.pSender->GetName() == L"Apply")
		{
			OnBtnApply();
		}
		else if( msg.pSender->GetName() == L"AutoStart" 
			|| msg.pSender->GetName() == L"RememberPwd"
			|| msg.pSender->GetName() == L"AutoLogin"
			|| msg.pSender->GetName() == L"AutoSync"
			|| msg.pSender->GetName() == L"AutoRemoteSaving"
			|| msg.pSender->GetName() == L"MinWnd"
			|| msg.pSender->GetName() == L"CloseWnd"
			) 
		{ 
			OPTION_CENTER.m_bNormalSettingChange = TRUE; 
		}
		else if( msg.pSender->GetName() == L"UpdateAutoComplete" 
			|| msg.pSender->GetName() == L"UpdateInstallHint"
			|| msg.pSender->GetName() == L"UpdateHint"
			|| msg.pSender->GetName() == L"UpdateClose"
			) 
		{ 
			OPTION_CENTER.m_bUpdateSettingChange = TRUE; 
		}
	}
	else if( msg.sType == _T("itemclick") ) 
	{
	}
	else if(msg.sType==_T("setfocus"))
	{
		CStdString name = msg.pSender->GetName();
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
		if(name==_T("NormalTab"))
		{
			if( m_eCurrentTab != TAB_NORMAL)
			{
				m_eCurrentTab = TAB_NORMAL;

				// 检测代理和更新是否更改过，更改过则提醒
				if( OPTION_CENTER.m_bProxySettingChange == TRUE 
					|| OPTION_CENTER.m_bUpdateSettingChange == TRUE)
				{
					int nRet = ::MessageBoxW(NULL, L"您已经修改了设置，需要保存吗?",L"提示", MB_OKCANCEL);
					if( nRet == MB_OK)
					{
						OnBtnApply();
					}
				}
				pControl->SelectItem(0);
			}
		}
		else if(name==_T("ProxyTab"))
		{
			if( m_eCurrentTab != TAB_PROXY)
			{
				m_eCurrentTab = TAB_PROXY;

				// 检测常规和更新是否更改过，更改过则提醒
				if( OPTION_CENTER.m_bNormalSettingChange == TRUE 
					|| OPTION_CENTER.m_bUpdateSettingChange == TRUE)
				{
					int nRet = ::MessageBoxW(NULL, L"您已经修改了设置，需要保存吗?",L"提示", MB_OKCANCEL);
					if( nRet == MB_OK)
					{
						OnBtnApply();
					}
				}
				pControl->SelectItem(1);
			}
		}
		else if(name==_T("UpdateTab"))
		{
			if( m_eCurrentTab != TAB_UPDATE)
			{
				m_eCurrentTab = TAB_UPDATE;

				// 检测常规和代理是否更改过，更改过则提醒
				if( OPTION_CENTER.m_bNormalSettingChange == TRUE 
					|| OPTION_CENTER.m_bProxySettingChange == TRUE)
				{
					int nRet = ::MessageBoxW(NULL, L"您已经修改了设置，需要保存吗?",L"提示", MB_OKCANCEL);
					if( nRet == MB_OK)
					{
						OnBtnApply();
					}
				}
				pControl->SelectItem(2);
			}
		}
	}
}

LRESULT CSettingWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("setting.xml"), (UINT)0,  NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CSettingWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CSettingWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CSettingWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CSettingWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CSettingWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CSettingWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0// &&
				/*_tcscmp(pControl->GetClass(), _T("TextUI")) != 0*/ )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CSettingWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CSettingWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-rcWork.left, -rcWork.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x		= rcWork.right;
	lpMMI->ptMaxSize.y		= rcWork.bottom;

	bHandled = FALSE;
	return 0;
}

LRESULT CSettingWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CSettingWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int wmId    = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);

	return 0;
}


LRESULT CSettingWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_COMMAND:       lRes = OnCommand(uMsg, wParam, lParam, bHandled); break;
		default:
			bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}