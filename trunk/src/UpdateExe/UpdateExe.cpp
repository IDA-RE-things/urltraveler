#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "ControlEx.h"
#include "resource.h"
#include "shellapi.h"
#include "XString.h"
#include "XUnzip.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "FileHelper.h"
#include "PathHelper.h"
#include "atlconv.h"
#include "json/json.h"

HINSTANCE	hGolobalInstance = NULL;
String	strUpdatePackage = _T("");


class CUpdateExeWnd : public CWindowWnd, public INotifyUI
{
public:
	CUpdateExeWnd() { };
	LPCTSTR GetWindowClassName() const { return _T("URUpdateExe"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init() 
	{
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	}

	void OnPrepare() 
	{
	}

	void Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("windowinit") ) OnPrepare();
		else if( msg.sType == _T("click") ) 
		{
			if( msg.pSender == m_pCloseBtn ) 
			{
				PostQuitMessage(0);
				return; 
			}
		}
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 设置任务栏图标
		HICON hSmallIcon = ::LoadIcon(hGolobalInstance,(LPCTSTR)IDI_APPICON16);
		HICON hBigIcon = ::LoadIcon(hGolobalInstance,(LPCTSTR)IDI_APPICON32);

		SendMessage(WM_SETICON,ICON_SMALL,(LPARAM)hSmallIcon);
		SendMessage(WM_SETICON,ICON_BIG,(LPARAM)hBigIcon); 

		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CDialogBuilderCallbackEx cb;
		CControlUI* pRoot = builder.Create(_T("install.xml"), (UINT)0,  &cb, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		Init();
		return 0;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0L);

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( ::IsIconic(*this) ) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		RECT rcCaption = m_pm.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
				return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE )
		{
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) 
		{
			if( !bZoomed )
			{
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else 
			{
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg ) 
		{
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
		default:
			bHandled = FALSE;
		}

		if( bHandled ) return lRes;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;
};

int UnzipPackage(wchar_t*	pPackageName)
{
	if( pPackageName == NULL)
		return -1;

	// 启动线程，进行更新
	HZIP hz = OpenZip((void*)pPackageName, 0, 2);

	unsigned int nZipItemNum;
	ZRESULT zResult = GetZipItemNum(hz, &nZipItemNum);
	if( zResult != ZR_OK)
		return -1;

	MiscHelper::DeleteUnpackagePath();

	size_t i=0;
	for( ; i<nZipItemNum; i++)
	{
		ZIPENTRY ze; 
		zResult = GetZipItemA(hz, i, &ze);
		if( zResult != ZR_OK)
			continue;

		USES_CONVERSION;
		wchar_t* pName = wcsdup(A2W(ze.name));

		wchar_t szName[MAX_PATH];
		memset(szName, 0x0, MAX_PATH);
		swprintf(szName, L"%s%s", MiscHelper::GetUnpackagePath(),pName);

		zResult = UnzipItem(hz, i, (void*)szName, wcslen(szName),ZIP_FILENAME);

		// 解压失败
		if( zResult != ZR_OK)
			break;
	}
	CloseZip(hz);

	// 更新失败
	if( i < nZipItemNum)
	{
		::MessageBox(NULL, L"安装包解压失败，无法正确进行更新", L"更新提示", MB_OK);
		return -1;
	}

	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE  hPrevInstance , LPSTR  lpCmdLine, int nCmdShow)
{
	hGolobalInstance = hInstance;

	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (szArgList == NULL)
	{
		return -1;
	}
	strUpdatePackage = szArgList[1];
	LocalFree(szArgList);


	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("\\skin\\UrlTraveler"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CUpdateExeWnd* pFrame = new CUpdateExeWnd();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("3+收藏夹漫游大师更新安装"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	/*
	int nRet = UnzipPackage((wchar_t*)strUpdatePackage.GetData());
	if( nRet == -1)
	return 0;
	*/

	// 找到update.json

	String	strUpdateJson  = MiscHelper::GetUnpackagePath();
	strUpdateJson += L"update.json";
	if( PathHelper::IsFileExist(strUpdateJson.GetData()) == FALSE)
		return 0;

	char* pOutBuffer = NULL;
	int nBufLen = 0;
	BOOL bRet = FileHelper::File2Buffer(strUpdateJson, (BYTE**)&pOutBuffer, nBufLen);
	if(bRet == FALSE)
		return 0;

	Json::Reader reader;
	Json::Value root;
	if(! reader.parse(pOutBuffer, root)) 
		return 0;

	Json::Value& fileList = root["filelist"];
	ASSERT(fileList.isArray() == TRUE);

	for( size_t i=0; i<fileList.size(); i++)
	{
		Json::Value& fileNode = fileList[i];

		string strAction = fileNode["action"].asString();
		string strFileName = fileNode["filename"].asString();
		string strMD5 = fileNode["md5"].asString();
		string strPath = fileNode["locatepath"].asString();

		// 拷贝文件
		if( strAction == "copy")
		{

		}


	}

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}