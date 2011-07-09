#include "stdafx.h"
#include "UpdateHintWnd.h"

#include "MiscHelper.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "XString.h"
#include "UpdateDefine.h"
#include "UpdateModule.h"

using namespace  update;


CUpdateHintWnd::CUpdateHintWnd()
{
}

CUpdateHintWnd::~CUpdateHintWnd()
{
}

void CUpdateHintWnd::OnPrepare(TNotifyUI& msg) 
{ 
	CTextUI * pVersion = static_cast<CTextUI *>(m_pm.FindControl(_T("version")));
	if( pVersion == NULL)
		return;
	pVersion->SetText(m_strVersion.GetData());

	CTextUI * pSize = static_cast<CTextUI *>(m_pm.FindControl(_T("size")));
	if( pSize == NULL)
		return;
	wchar_t	szSize[32];
	swprintf(szSize, L"%.2fM", m_fSize);
	pSize->SetText(szSize);

	// 更新内容：{n}{n} {x 2}{i ball.png}  增加了对Chrome浏览器的支持 {n}{n} {x 2}{i ball.png}  修改了FireFox插件的bug
	String	strUpdateInfo = L"更新内容：";
	for( int i=0; i<m_vUpdateDetail.size(); i++)
	{
		strUpdateInfo += L"{n}{n}{x 2}{i ball.png}  ";
		strUpdateInfo += m_vUpdateDetail[i].c_str();
	}

	CTextUI * pUpdateDetail = static_cast<CTextUI *>(m_pm.FindControl(_T("update_detail")));
	if( pUpdateDetail == NULL)
		return;
	pUpdateDetail->SetText(strUpdateInfo.GetData());
}

void CUpdateHintWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) 
	{
		OnPrepare(msg);
	}
	else if( msg.sType == _T("click") )
	{
		if( msg.pSender->GetName() == L"closebtn" ) 
		{
			Close();
			return; 
		}
		// 立即更新
		else if( msg.pSender->GetName() == L"UpdateNow" ) 
		{ 
			Update_ShowUpdateDownloadingEvent* pEvent = new Update_ShowUpdateDownloadingEvent();
			pEvent->srcMId = MODULE_ID_UPDATE;
			pEvent->bForce = FALSE;
			STRNCPY(pEvent->szDownloadUrl, m_strDownloadUrl.GetData());
			STRNCPY(pEvent->szSavePath, m_strSavePath.GetData());
			pEvent->nLastestVersion = MiscHelper::GetVersionFromString(
				StringHelper::UnicodeToANSI(m_strVersion.GetData()).c_str()); 
			g_UpdateModule->GetModuleManager()->PushEvent(*pEvent);

			Close();
		}
		else if( msg.pSender->GetName() == L"RemindNow" )
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; 
		}
	}
}

LRESULT CUpdateHintWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("update_tip.xml"), (UINT)0,  NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CUpdateHintWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	return 0;
}

LRESULT CUpdateHintWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CUpdateHintWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUpdateHintWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUpdateHintWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CUpdateHintWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CUpdateHintWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CUpdateHintWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		PostMessageW(WM_CLOSE,0,0);
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CUpdateHintWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int wmId    = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);

	switch (wmId)
	{
		// 弹出登录对话框
	case IDM_LOGIN:
		{
			/*
			CLoginFrameWnd* pLoginFrame = new CLoginFrameWnd();
			if( pLoginFrame == NULL ) { Close(); return 0; }
			pLoginFrame->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
			pLoginFrame->CenterWindow();
			ShowModal(*pLoginFrame);
			*/
		}

		break;

	default:
		break;
	}

	return 0;
}


LRESULT CUpdateHintWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
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

void CUpdateHintWnd::SetDownloadUrl(String strDownloadUrl)
{
	m_strDownloadUrl = strDownloadUrl;
}

void	CUpdateHintWnd::SetSavePath(String strSavePath)
{
	m_strSavePath = strSavePath;
}

void	CUpdateHintWnd::SetVersion(String	strVersion)
{
	m_strVersion = strVersion;
}

void	CUpdateHintWnd::SetSize(float fSize)
{
	m_fSize = fSize;
}

void	CUpdateHintWnd::AddUpdateDetail(String strDetail)
{
	m_vUpdateDetail.push_back(strDetail.GetData());
}
