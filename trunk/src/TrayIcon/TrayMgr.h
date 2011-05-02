#pragma once

#include <vector>
#include <stack>
#include "shellapi.h"


//#pragma comment(lib, "shellapi.lib")

using namespace std;

#define Use_Default 0
#define Int_Default 0
#define Default 0

#define No_Care -1
#define No_Care_HWND (HWND)-1

#define Use_Current_Int -1
#define Use_Current_Pointer -1
#define Use_Current_SZ (TCHAR*)(-1)

#define Use_Current Use_Current_Int
#define Use_Current_HICON (HICON)(-1)


enum ETrayNotifyMode
{
	eTNMode_Null = 0,
	eTNMode_Event,
	eTNMode_Msg,
};

enum ETrayFlashStopMode
{
	eTFSMode_Click = 0,
	eTFSMode_DoubleClick,
};

enum ETrayEventId
{
	WM_LBUTTONDOWN_LATE = WM_USER + 8765,
	WM_TRAYFLASHSTOPPED,
	WM_TRAYALLFLASHSTOPPED,
	WM_MOUSEMOVE_POST,
};

class ITrayEvent
{
public:
	virtual void OnTrayEvent(WPARAM w, LPARAM l) = 0;
	virtual void OnFlashStart(DWORD dwParam) = 0;
	virtual void OnFlashEnd(DWORD dwParam) = 0;
};

struct STrayFlashStackItem
{
	HICON hicon;
	DWORD dwParam;

	STrayFlashStackItem()
	{
		hicon = NULL;
		dwParam = 0;
	}
};

class  CTrayMgr
{
public:
	CTrayMgr();
	~CTrayMgr(void);

	void Init(HWND hwndAttach = No_Care_HWND, ETrayNotifyMode = eTNMode_Event, 
		ETrayFlashStopMode = eTFSMode_DoubleClick);
	BOOL Add(HICON hicon, LPCTSTR szTip, UINT uCallbackMsgId = No_Care);
	BOOL ReAdd(HICON hicon, LPCTSTR szTip, UINT uCallbackMsgId = No_Care);
	BOOL Delete(BOOL bClearStatus = false);
	HICON GetIcon(){return m_nid.hIcon;}

	BOOL Change(HICON hiconNew, LPCTSTR szTipNew = Use_Current_SZ, 
		UINT uCallbackMsgIdNew = Use_Current)
	{
		return InnerChange(hiconNew, szTipNew, uCallbackMsgIdNew);
	};

	BOOL ShowTips(bool bShow);

	BOOL ChangeIcon(HICON hiconNew)
	{
		return InnerChange(hiconNew, Use_Current_SZ, Use_Current);
	}

	BOOL ChangeTip(LPCTSTR szTipNew)
	{
		return InnerChange(Use_Current_HICON, szTipNew, Use_Current);
	}

	BOOL Flash(HICON hiconNew = Use_Current_HICON, DWORD dwParam = 0);
	BOOL StopFlash(int nIndex = -1);
	BOOL StopAllFlash();

	int GetFlashStatckSize(){return m_stackFlash.size();}

	// ÊÂ¼þ¼àÌý
	void AddEventHandler(ITrayEvent* pHandler);
	void DeleteEventHandler(ITrayEvent* pHandler);
	void FireOnTrayEvent(WPARAM w, LPARAM l);
	void FireOnFlashStart(DWORD dwParam);
	void FireOnFlashEnd(DWORD dwParam);

	BOOL IsFlashing()
	{
		if (m_bFlashing == false)
		{
			ASSERT(m_stackFlash.size() == 0);
		}

		return m_stackFlash.size() != 0;
	}

	vector<STrayFlashStackItem>* GetFlashStack()
	{
		if (m_stackFlash.empty())
		{
			return NULL;
		}
		else
		{
			return &m_stackFlash;
		}
	}
	HWND GetWindowHandle();

	void FakePickMsg();
	void FakePickAllMsg();
	void ClearAllFlash();

protected:

	std::wstring m_strCacheTipsString;

	NOTIFYICONDATA m_nid;
	vector<STrayFlashStackItem> m_stackFlash;
	BOOL m_bFlashing;
	HWND m_hwndTray;
	ETrayNotifyMode m_eTNMode;
	vector<ITrayEvent*> m_vecTrayEventHandler;
	BOOL m_bWaitForDblClk;
	ETrayFlashStopMode m_eTFSMode;
	UINT m_uTrayFlashTimerId;

	static void CALLBACK OnFlashTimer(HWND hwnd, UINT uMsg, UINT nEvent, DWORD dwTime);
	static LRESULT CALLBACK TrayWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static ATOM RegisterClassForTrayWnd(/*HINSTANCE hInstance*/);
	static HICON m_hTrayBlank;

	BOOL InnerChange(HICON hicon = Use_Current_HICON, LPCTSTR szTip = Use_Current_SZ, 
		UINT uCallbackMsgId = Use_Current);
	void Sort();
};
