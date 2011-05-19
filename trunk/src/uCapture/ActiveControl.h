#pragma once

#include "Base.h"

class CWindowWnd
{
public:
	CWindowWnd();

	HWND GetHWND() const;
	operator HWND() const;

	bool RegisterWindowClass();
	bool RegisterSuperclass();

	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	HWND Subclass(HWND hWnd);
	void Unsubclass();
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);
	bool ShowModal(HWND hWnd);
	int MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType); // 目前仅支持MB_OK
	void Close();
	void CenterWindow();
	void SetIcon(UINT nRes);

	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	void ResizeClient(int cx = -1, int cy = -1);
	HINSTANCE GetInstance();

protected:
	virtual LPCTSTR GetWindowClassName() const = 0;
	virtual LPCTSTR GetSuperClassName() const;
	virtual UINT GetClassStyle() const;

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hWnd);

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	WNDPROC m_OldWndProc;
	bool m_bSubclassed;
};


struct IOleObject;

class CActiveXCtrl;

template< class T >
class CSafeRelease
{
public:
	CSafeRelease(T* p) : m_p(p) { };
	~CSafeRelease() { if( m_p != NULL ) m_p->Release(); };
	T* Detach() { T* t = m_p; m_p = NULL; return t; };
	T* m_p;
};

class  CActiveXUI
{
	friend CActiveXCtrl;
public:
	CActiveXUI();
	virtual ~CActiveXUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	bool IsDelayCreate() const;
	void SetDelayCreate(bool bDelayCreate = true);
	void SetMainFrame(HWND hWnd);
	void SetRect(CRect rc);

	bool CreateControl(const CLSID clsid);
	bool CreateControl(LPCTSTR pstrCLSID);
	HRESULT GetControl(const IID iid, LPVOID* ppRet);
	CLSID GetClisd() const;

	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	void SetPos(RECT rc);
	void DoPaint(HDC hDC, const RECT& rcPaint);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

protected:
	void ReleaseControl();
	bool DoCreateControl();

protected:
	CLSID m_clsid;
	bool m_bCreated;
	bool m_bDelayCreate;
	IOleObject* m_pUnk;
	CActiveXCtrl* m_pControl;
	HWND m_hwndHost;
public:
	HWND m_hFrameWnd;
	CRect m_rcItem;
};

