#pragma once

// Login�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "UrlTravelerHeader.h"


namespace login
{
	// Login���⹫�����¼�
	enum E_LoginEventValue
	{
		EVENT_VALUE_LOGIN_OPEN = EVENT_VALUE_LOGIN_BEGIN,		//	�򿪵�½����
		
	};

	// Login�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_LoginMessageValue
	{
	};

	// Login�ܹ������ֱ�ӵ���
	enum E_LoginServiceValue
	{
		SERVICE_VALUE_TEST	=	SERVICE_VALUE_LOGIN_BEGIN,
	};

	//===========================================//
	//                   Login����ʹ�õ���event�ṹ								 	  //
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