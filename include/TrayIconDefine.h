#pragma once

// ͼ�����ģ��Ķ����ͷ�ļ���Ϣ
#include "SndaBase.h"

namespace trayicon
{
	// TrayIcon���⹫�����¼�
	enum E_TrayIconEventValue
	{
		EVENT_VALUE_TRAYICON_SHOW = EVENT_VALUE_TRAYICON_BEGIN,		//	֪ͨ��ʾ������ͼ��
		EVENT_VALUE_TRAYICON_SHOW_UPDATE_WND,										//	����������ʾ��
	};

	// TrayIcon�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_TrayIconMessageValue
	{

	};

	// TrayIcon�ܹ������ֱ�ӵ���
	enum E_TrayIconServiceValue
	{
	};

	//===========================================//
	//                   TrayIcon����ʹ�õ���event�ṹ					      //
	//===========================================//
	struct TrayIconEvent	:	public EventExtraInfo
	{
	public:
		TrayIconEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_TRAYICON;
		}
	};

	struct TrayIcon_ShowUpdateWndEvent	:	public	TrayIconEvent
	{
	public:
		   TrayIcon_ShowUpdateWndEvent()
		   {
			   nNewestVersion	=	0;
		   }

		   //	��ǰ�����°汾
		   int	nNewestVersion;
	};
};