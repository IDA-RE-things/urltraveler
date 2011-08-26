#include "stdafx.h"
#include "UIIconBox.h"

namespace DuiLib {

	CIconBoxUI::CIconBoxUI()
	{
		m_nDistanceOfEachIcon = 5;
		m_stIconSize.cx = 16;
		m_stIconSize.cy = 16;
		m_nShowIndex = 0;
		m_nSelectIndex = -1;

		m_uButton1State = 0;
		m_uButton2State = 0;

		m_dwIconBolderColor = 0xFF85E4FF;
		m_nIconBolderSize = 1;
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
		if( _tcscmp(pstrName, _T("button1normalimage")) == 0 ) SetButton1NormalImage(pstrValue);
		else if( _tcscmp(pstrName, _T("button1hotimage")) == 0 ) SetButton1HotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("button1pushedimage")) == 0 ) SetButton1PushedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("button1disabledimage")) == 0 ) SetButton1DisabledImage(pstrValue);
		else if( _tcscmp(pstrName, _T("button2normalimage")) == 0 ) SetButton2NormalImage(pstrValue);
		else if( _tcscmp(pstrName, _T("button2hotimage")) == 0 ) SetButton2HotImage(pstrValue);
		else if( _tcscmp(pstrName, _T("button2pushedimage")) == 0 ) SetButton2PushedImage(pstrValue);
		else if( _tcscmp(pstrName, _T("button2disabledimage")) == 0 ) SetButton2DisabledImage(pstrValue);
		else if( _tcscmp(pstrName, _T("iconsize")) == 0 )
		{
			LPTSTR pstr = NULL;
			m_stIconSize.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			m_stIconSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		}
		else if( _tcscmp(pstrName, _T("distanceoficon")) == 0 )
		{
			LPTSTR pstr = NULL;

			m_nDistanceOfEachIcon = _tcstol(pstrValue, &pstr, 10);
		}
		else if( _tcscmp(pstrName, _T("iconboldercolor")) == 0 )
		{
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			m_dwIconBolderColor = _tcstoul(pstrValue, &pstr, 16);
		}
		else if( _tcscmp(pstrName, _T("iconboldersize")) == 0 )
		{
			LPTSTR pstr = NULL;

			m_nIconBolderSize = _tcstol(pstrValue, &pstr, 10);
		}
		else CControlUI::SetAttribute(pstrName, pstrValue);
	}

	void CIconBoxUI::DoEvent( TEventUI& event )
	{
		if(event.Type == UIEVENT_MOUSEMOVE)
		{
			//如果显示左右按钮，则先判断鼠标是否落入在按钮区域
			if (m_bShowButton1 && ::PtInRect(&m_rcButton1, event.ptMouse))
			{
				m_nSelectIndex = -1;
				SetToolTip(L"");
				m_uButton1State |= UISTATE_HOT;
			}
			else if (m_bShowButton2 && ::PtInRect(&m_rcButton2, event.ptMouse))
			{
				m_nSelectIndex = -1;
				SetToolTip(L"");
				m_uButton2State |= UISTATE_HOT;
			}
			else
			{
				int nIndex = 0;
				if (m_bShowButton1 && m_bShowButton2)
				{
					nIndex = (event.ptMouse.x - m_rcButton1.right) / (m_stIconSize.cx + m_nDistanceOfEachIcon);
				}
				else
				{
					nIndex = (event.ptMouse.x - m_rcItem.left) / (m_stIconSize.cx + m_nDistanceOfEachIcon);
				}
				m_nSelectIndex = nIndex;

				ICONITEMINFO *pIconInfo = (ICONITEMINFO *)m_arrIcons.GetAt(nIndex + m_nShowIndex);
				if (pIconInfo)
				{
					SetToolTip(pIconInfo->szTip);
				}
			}

			Invalidate();
		}
		else if(event.Type == UIEVENT_BUTTONDOWN)
		{
			if (::PtInRect(&m_rcButton1, event.ptMouse))
			{
				m_uButton1State = 0;
				m_uButton1State |= UISTATE_PUSHED;
				Invalidate();
			}

			if (::PtInRect(&m_rcButton2, event.ptMouse))
			{
				m_uButton2State = 0;
				m_uButton2State |= UISTATE_PUSHED;
				Invalidate();
			}
		}
		else if(event.Type == UIEVENT_BUTTONUP)
		{


			if( ::PtInRect(&m_rcButton1, event.ptMouse) ) {
				m_uButton1State = 0;
				m_uButton2State = 0;

				if (m_nShowIndex > 0)
				{
					m_nShowIndex -= 1;
					Invalidate();
				}
			}
			if( ::PtInRect(&m_rcButton2, event.ptMouse) ) {
				m_uButton1State = 0;
				m_uButton2State = 0;

				if (m_nShowIndex < GetIconCount() - 1)
				{
					m_nShowIndex += 1;
					Invalidate();
				}
			}
		}
		else if(event.Type == UIEVENT_MOUSELEAVE)
		{
			m_uButton1State = 0;
			m_uButton2State = 0;
			m_nSelectIndex = -1;
			Invalidate();
		}
	}

	bool CIconBoxUI::IsButton1Show()
	{
		return m_bShowButton1;
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

		PaintIcon(hDC);

		if (m_bShowButton1)
		{
			PaintButton1(hDC);
		}

		if (m_bShowButton2)
		{
			PaintButton2(hDC);
		}
	}

	void CIconBoxUI::PaintIcon(HDC hDC)
	{
		//获取box的宽度
		CRect rcIconBolder;
		int nWidth = GetWidth();
		int nHoldIcons = nWidth / (m_stIconSize.cx + m_nDistanceOfEachIcon);
		CRect rc;
		int i = 0;

		rc = GetPos();
		rc.top += (GetHeight() - m_stIconSize.cy) / 2;

		if (nHoldIcons > GetIconCount())
		{
			m_bShowButton1 = false;
			m_bShowButton2 = false;
			rc.left += m_nDistanceOfEachIcon;
		}
		else
		{
			m_bShowButton1 = true;
			m_bShowButton2 = true;
			rc.left += (m_rcButton1.right - m_rcButton1.left) + m_nDistanceOfEachIcon;
			nHoldIcons = (nWidth - (m_rcButton1.right - m_rcButton1.left + m_rcButton2.right - m_rcButton2.left)) / (m_stIconSize.cx + m_nDistanceOfEachIcon);
		}

		rcIconBolder.left = rc.left;
		rcIconBolder.top = rc.top;
		rcIconBolder.bottom = rcIconBolder.top + m_stIconSize.cy;

		i = m_nShowIndex;

		int nIcons = m_arrIcons.GetSize();

		for (; i < nIcons && i < nHoldIcons + m_nShowIndex; i++)
		{
			ICONITEMINFO *pIconInfo = (ICONITEMINFO *)m_arrIcons.GetAt(i);
			DrawIconEx(hDC, rc.left, rc.top, pIconInfo->hIcon, m_stIconSize.cx, m_stIconSize.cy, 1, NULL, DI_NORMAL);

			rc.left += m_stIconSize.cx + m_nDistanceOfEachIcon;
		}

		//用户有选中icon，且选中的Icon不能超过当前显示的Icon数 且选中的Icon不能抽IconBox所能容纳的Icons数
		if (m_nSelectIndex != -1 && m_nSelectIndex < nIcons - m_nShowIndex && m_nSelectIndex < nHoldIcons)
		{
			rcIconBolder.left += m_nSelectIndex * m_nDistanceOfEachIcon + m_nSelectIndex * m_stIconSize.cx;

			rcIconBolder.right = rcIconBolder.left + m_stIconSize.cx;

			rcIconBolder.left -= m_nDistanceOfEachIcon / 2;
			rcIconBolder.right += m_nDistanceOfEachIcon / 2;
			rcIconBolder.top -= m_nDistanceOfEachIcon / 2;
			rcIconBolder.bottom += m_nDistanceOfEachIcon / 2;

			CRenderEngine::DrawRect(hDC, rcIconBolder, m_nIconBolderSize, m_dwIconBolderColor);
		}
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
		int nBorderSize = 1;
		CRenderEngine::DrawRect(hDC, m_rcButton1, nBorderSize, dwBorderColor);
	}

	void CIconBoxUI::SetPos(RECT rc)
	{
		CControlUI::SetPos(rc);
		rc = m_rcItem;

		m_nShowIndex = 0;

		int cx = rc.right - rc.left;
		m_rcButton1.left = rc.left;
		m_rcButton1.top = rc.top;

		m_rcButton1.right = rc.left + m_cxyFixed.cy / 2;
		m_rcButton1.bottom = rc.top + m_cxyFixed.cy;

		m_rcButton2.top = rc.top;
		m_rcButton2.right = rc.right;
		m_rcButton2.left = rc.right - m_cxyFixed.cy / 2;
		m_rcButton2.bottom = rc.top + m_cxyFixed.cy;
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
		int nBorderSize = 1;
		CRenderEngine::DrawRect(hDC, m_rcButton2, nBorderSize, dwBorderColor);
	}

	bool CIconBoxUI::AddIcon( HICON hIcon, LPCTSTR lpszTip, BOOL bRepaint)
	{
		ICONITEMINFO *pIconInfo = new ICONITEMINFO;

		pIconInfo->hIcon = hIcon;
		_tcscpy_s(pIconInfo->szTip, MAX_PATH - 1, lpszTip);
		bool bRet = m_arrIcons.Add((LPVOID)pIconInfo);

		if (bRet && bRepaint)
		{
			Invalidate();
		}

		return bRet;
	}

	bool CIconBoxUI::DelIcon( HICON hIcon )
	{
		int nSize = m_arrIcons.GetSize();

		for (int i = 0; i < nSize; i++)
		{
			ICONITEMINFO *pIconInfo = (ICONITEMINFO *)m_arrIcons.GetAt(i);

			if (pIconInfo && pIconInfo->hIcon == hIcon)
			{
				m_arrIcons.Remove(i);
				return true;
			}
		}

		return false;
	}

	void	CIconBoxUI::DelAllIcon()
	{
		int nSize = m_arrIcons.GetSize();
		for (int i = 0; i < nSize; i++)
		{
			m_arrIcons.Remove(i);
		}
	}

	int CIconBoxUI::GetIconCount()
	{
		return m_arrIcons.GetSize();
	}

	void CIconBoxUI::SetIconSize( SIZE stIconSize )
	{
		m_stIconSize = stIconSize;
	}

	SIZE CIconBoxUI::GetIconSize()
	{
		return m_stIconSize;
	}

}