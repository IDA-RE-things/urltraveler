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

void CIconBoxUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	 if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;


}

void CIconBoxUI::PaintIcon(HDC hDC)
{

}

void CIconBoxUI::PaintButton1(HDC hDC)
{
	if( !IsEnabled() ) m_uButton1State |= UISTATE_DISABLED;
	else m_uButton1State &= ~ UISTATE_DISABLED;

	m_sImageModify.Empty();
	m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), m_rcButton1.left - m_rcItem.left, \
		m_rcButton1.top - m_rcItem.top, m_rcButton1.right - m_rcItem.left, m_rcButton1.bottom - m_rcItem.top);

	if( (m_uButton1State & UISTATE_DISABLED) != 0 ) {
		if( !m_sButton1DisabledImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sButton1DisabledImage, (LPCTSTR)m_sImageModify) ) m_sButton1DisabledImage.Empty();
			else return;
		}
	}
	else if( (m_uButton1State & UISTATE_PUSHED) != 0 ) {
		if( !m_sButton1PushedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sButton1PushedImage, (LPCTSTR)m_sImageModify) ) m_sButton1PushedImage.Empty();
			else return;
		}
	}
	else if( (m_uButton1State & UISTATE_HOT) != 0 ) {
		if( !m_sButton1HotImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sButton1HotImage, (LPCTSTR)m_sImageModify) ) m_sButton1HotImage.Empty();
			else return;
		}
	}

	if( !m_sButton1NormalImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sButton1NormalImage, (LPCTSTR)m_sImageModify) ) m_sButton1NormalImage.Empty();
		else return;
	}

	DWORD dwBorderColor = 0xFF85E4FF;
	int nBorderSize = 2;
	CRenderEngine::DrawRect(hDC, m_rcButton1, nBorderSize, dwBorderColor);
}

void CIconBoxUI::PaintButton2(HDC hDC)
{
	if( !IsEnabled() ) m_uButton2State |= UISTATE_DISABLED;
	else m_uButton2State &= ~ UISTATE_DISABLED;

	m_sImageModify.Empty();
	m_sImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), m_rcButton2.left - m_rcItem.left, \
		m_rcButton2.top - m_rcItem.top, m_rcButton2.right - m_rcItem.left, m_rcButton2.bottom - m_rcItem.top);

	if( (m_uButton2State & UISTATE_DISABLED) != 0 ) {
		if( !m_sButton2DisabledImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sButton2DisabledImage, (LPCTSTR)m_sImageModify) ) m_sButton2DisabledImage.Empty();
			else return;
		}
	}
	else if( (m_uButton2State & UISTATE_PUSHED) != 0 ) {
		if( !m_sButton2PushedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sButton2PushedImage, (LPCTSTR)m_sImageModify) ) m_sButton2PushedImage.Empty();
			else return;
		}
	}
	else if( (m_uButton2State & UISTATE_HOT) != 0 ) {
		if( !m_sButton2HotImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sButton2HotImage, (LPCTSTR)m_sImageModify) ) m_sButton2HotImage.Empty();
			else return;
		}
	}

	if( !m_sButton2NormalImage.IsEmpty() ) {
		if( !DrawImage(hDC, (LPCTSTR)m_sButton2NormalImage, (LPCTSTR)m_sImageModify) ) m_sButton2NormalImage.Empty();
		else return;
	}

	DWORD dwBorderColor = 0xFF85E4FF;
	int nBorderSize = 2;
	CRenderEngine::DrawRect(hDC, m_rcButton2, nBorderSize, dwBorderColor);
}

}