#pragma once

#include "Menu.h"

class CTreeListMenu : public CMenuWnd
{
public:
	CTreeListMenu(wchar_t*	 pSkinXml) : CMenuWnd(pSkinXml)
	{
	}

    void Notify(TNotifyUI& msg)
    {
        if( msg.sType == _T("itemselect") ) {
            Close();
        }
        else if( msg.sType == _T("itemclick") ) {
            if( msg.pSender->GetName() == _T("menu_Delete") ) {
                if( m_pOwner ) {
                    CListUI* pList = static_cast<CListUI*>(m_pOwner);
                    int nSel = pList->GetCurSel();
                    if( nSel < 0 ) return;
/*
                    pList->RemoveAt(nSel);
                    domain.erase(domain.begin() + nSel);
                    desc.erase(desc.begin() + nSel);
*/
                    MessageBox( _T("≤‚ ‘"), _T("≤‚ ‘"), MB_OK);
                }
            }
        }
    }
};