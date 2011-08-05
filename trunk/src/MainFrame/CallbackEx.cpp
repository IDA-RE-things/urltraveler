#include "CallbackEx.h"
#include "TreeList.h"
#include "DragList.h"

CControlUI* CDialogBuilderCallbackEx::CreateControl(LPCTSTR pstrClass) 
{
	if( _tcscmp(pstrClass, _T("TreeList")) == 0 ) 
	{
		return new TreeListUI;
	}
	else if( _tcscmp(pstrClass, _T("DragList")) == 0 ) 
	{
		return new DragListUI;
	}

	return NULL;
}