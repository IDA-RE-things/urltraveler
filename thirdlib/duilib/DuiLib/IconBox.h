#pragma  once
#include "UIlib.h"

namespace DuiLib {

class CIconBoxUI : public CControlUI 
{
public:
	CIconBoxUI();
	~CIconBoxUI();
public:
	LPVOID GetInterface(LPCTSTR pstrName);
	LPCTSTR GetClass() const;
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);
	/*iocn box�������*/
	bool AddIcon(HICON hIcon);
	bool DelIcon(HICON hIcon);
	bool AddBitmap(HBITMAP hBitmap);
	bool DelBitmap(HBITMAP hBitmap);
	int  GetIconCount();

	/*�滭���*/
	void DoPaint(HDC hDC, const RECT& rcPaint);;
	void PaintIcon(HDC hDC);
	void PaintButton1(HDC hDC);
	void PaintButton2(HDC hDC);

	bool IsButton1Show();
	void ShowButton1(bool bShow);
	LPCTSTR GetButton1NormalImage();
	void SetButton1NormalImage(LPCTSTR pStrImage);
	LPCTSTR GetButton1HotImage();
	void SetButton1HotImage(LPCTSTR pStrImage);
	LPCTSTR GetButton1PushedImage();
	void SetButton1PushedImage(LPCTSTR pStrImage);
	LPCTSTR GetButton1DisabledImage();
	void SetButton1DisabledImage(LPCTSTR pStrImage);

	bool IsButton2Show();
	void ShowButton2(bool bShow);
	LPCTSTR GetButton2NormalImage();
	void SetButton2NormalImage(LPCTSTR pStrImage);
	LPCTSTR GetButton2HotImage();
	void SetButton2HotImage(LPCTSTR pStrImage);
	LPCTSTR GetButton2PushedImage();
	void SetButton2PushedImage(LPCTSTR pStrImage);
	LPCTSTR GetButton2DisabledImage();
	void SetButton2DisabledImage(LPCTSTR pStrImage);
private:
	CStdPtrArray m_arrIcons;
	CStdPtrArray m_arrBitmaps;
    /*��߰�ť��״̬*/
	bool m_bShowButton1;
	RECT m_rcButton1;
	UINT m_uButton1State;
	CStdString m_sButton1NormalImage;
	CStdString m_sButton1HotImage;
	CStdString m_sButton1PushedImage;
	CStdString m_sButton1DisabledImage;
	/*�ұ߰�ť��״̬*/
	bool m_bShowButton2;
	RECT m_rcButton2;
	UINT m_uButton2State;
	CStdString m_sButton2NormalImage;
	CStdString m_sButton2HotImage;
	CStdString m_sButton2PushedImage;
	CStdString m_sButton2DisabledImage;

	CStdString m_sImageModify;
};
}