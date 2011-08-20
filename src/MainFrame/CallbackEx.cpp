#include "CallbackEx.h"
#include "FavTreeList.h"
#include "DragList.h"

CControlUI* CDialogBuilderCallbackEx::CreateControl(LPCTSTR pstrClass) 
{
	if( _tcscmp(pstrClass, _T("FavTreeList")) == 0 ) 
	{
		return new CFavoriteTreeListUI;
	}
	else if( _tcscmp(pstrClass, _T("DragList")) == 0 ) 
	{
		return new CDragListUI;
	}

	return NULL;
}