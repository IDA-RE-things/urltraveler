#pragma  once


class CIconBoxUI : public CHorizontalLayoutUI
{
public:
	CIconBoxUI();
	~CIconBoxUI();
public:
	LPVOID GetInterface(LPCTSTR pstrName);
	LPCTSTR GetClass() const;
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);

};