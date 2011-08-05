#pragma once

#include <stdafx.h>
#include "UIlib.h"

using namespace DuiLib;

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass);
};