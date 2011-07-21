#pragma once

// Login的对外的头文件信息

#include "SndaBase.h"
#include "UrlTravelerHeader.h"


namespace login
{
	// Login对外公开的事件
	enum E_LoginEventValue
	{
		EVENT_VALUE_LOGIN_OPEN = EVENT_VALUE_LOGIN_BEGIN,		//	打开登陆界面
		
	};

	// Login能够可能对外发送的广播消息
	enum E_LoginMessageValue
	{
	};

	// Login能够处理的直接调用
	enum E_LoginServiceValue
	{
		SERVICE_VALUE_TEST	=	SERVICE_VALUE_LOGIN_BEGIN,
	};

	//===========================================//
	//                   Login中所使用到的event结构								 	  //
	//===========================================//
	struct LoginEvent	:	public EventExtraInfo
	{
	public:
		LoginEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_LOGIN;
		}
	};



};