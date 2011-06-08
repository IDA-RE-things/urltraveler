#pragma once

#include "Menu.h"
#include "MainFrameDefine.h"

using namespace mainframe;

class CFavoriteListMenu : public CMenuWnd
{
public:
	CFavoriteListMenu(wchar_t*	 pSkinXml) : CMenuWnd(pSkinXml)
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
                    pList->RemoveAt(nSel);
					pList->Invalidate();

					// 从数据中心中删除该Item
					MainFrame_DeleteFavoriteEvent* pEvent = new MainFrame_DeleteFavoriteEvent();
					pEvent->desMId = MODULE_ID_MAINFRAME;
					pEvent->nDeleteIndex = nSel;
					g_MainFrameModule->GetModuleManager()->PushEvent(*pEvent);
                }
            }
        }
    }
};