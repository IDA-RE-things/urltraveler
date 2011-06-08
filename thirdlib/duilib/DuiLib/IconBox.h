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
	/*iocn boxœ‡πÿ Ù–‘*/
	bool AddIcon(HICON hIcon);
	bool DelIcon(HICON hIcon);
	bool AddBitmap(HBITMAP hBitmap);
	bool DelBitmap(HBITMAP hBitmap);
	int  GetIconCount();
private:
	CStdPtrArray m_arrIcons;
	CStdPtrArray m_arrBitmaps;
};
}