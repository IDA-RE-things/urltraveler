#pragma once

#include "SndaBase.h"
#include "TrayMgr.h"

class CTrayEvent : public ITrayEvent
{
public:
	CTrayEvent(void);
	~CTrayEvent(void);

	virtual void OnTrayEvent(WPARAM w, LPARAM l);
	virtual void OnFlashStart(DWORD dwParam);
	virtual void OnFlashEnd(DWORD dwParam);
};
