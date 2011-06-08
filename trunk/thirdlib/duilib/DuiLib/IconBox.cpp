#include "stdafx.h"
#include "IconBox.h"

namespace DuiLib {

CIconBoxUI::CIconBoxUI()
{

}

CIconBoxUI::~CIconBoxUI()
{

}

LPVOID CIconBoxUI::GetInterface( LPCTSTR pstrName )
{
	return NULL;
}

LPCTSTR CIconBoxUI::GetClass() const
{
	return _T("IconBoxUI");
}

void CIconBoxUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{

}

void CIconBoxUI::DoEvent( TEventUI& event )
{

}

bool CIconBoxUI::IsButton1Show()
{
	return m_bShowButton1;
}

void CIconBoxUI::ShowButton1( bool bShow )
{
	m_bShowButton1 = bShow;
}

LPCTSTR CIconBoxUI::GetButton1NormalImage()
{
	return m_sButton1NormalImage;
}

void CIconBoxUI::SetButton1NormalImage( LPCTSTR pStrImage )
{
	m_sButton1NormalImage = pStrImage;
}

LPCTSTR CIconBoxUI::GetButton1HotImage()
{
	return m_sButton1HotImage;
}

void CIconBoxUI::SetButton1HotImage( LPCTSTR pStrImage )
{
	m_sButton1HotImage = pStrImage;
}

LPCTSTR CIconBoxUI::GetButton1PushedImage()
{
	return m_sButton1PushedImage;
}

void CIconBoxUI::SetButton1PushedImage( LPCTSTR pStrImage )
{
	m_sButton1PushedImage = pStrImage;
}

LPCTSTR CIconBoxUI::GetButton1DisabledImage()
{
	return m_sButton1DisabledImage;
}

void CIconBoxUI::SetButton1DisabledImage( LPCTSTR pStrImage )
{
	m_sButton1DisabledImage = pStrImage;
}

bool CIconBoxUI::IsButton2Show()
{
	return m_bShowButton2;
}

void CIconBoxUI::ShowButton2( bool bShow )
{
	m_bShowButton2 = bShow;
}

LPCTSTR CIconBoxUI::GetButton2NormalImage()
{
	return m_sButton1NormalImage;
}

void CIconBoxUI::SetButton2NormalImage( LPCTSTR pStrImage )
{
	m_sButton2NormalImage = pStrImage;
}

LPCTSTR CIconBoxUI::GetButton2HotImage()
{
	return m_sButton2HotImage;
}

void CIconBoxUI::SetButton2HotImage( LPCTSTR pStrImage )
{
	m_sButton2HotImage = pStrImage;
}

LPCTSTR CIconBoxUI::GetButton2PushedImage()
{
	return m_sButton2PushedImage;
}

void CIconBoxUI::SetButton2PushedImage( LPCTSTR pStrImage )
{
	m_sButton2PushedImage = pStrImage;
}

LPCTSTR CIconBoxUI::GetButton2DisabledImage()
{
	return m_sButton2DisabledImage;
}

void CIconBoxUI::SetButton2DisabledImage( LPCTSTR pStrImage )
{
	m_sButton2DisabledImage = pStrImage;
}

}