#pragma once

// MainFrame�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"


namespace mainframe
{
	// MainFrame���⹫�����¼�
	enum E_MainFrameEventValue
	{
		EVENT_VALUE_MAINFRAME_OPEN = EVENT_VALUE_MAINFRAME_BEGIN,		//��������
		EVENT_VALUE_MAINFRAME_HIDE,							//	���ص�ǰ������
		EVENT_VALUE_MAINFRAME_CLOSE,						//	�رյ�ǰ������
		EVENT_VALUE_MAINFRAME_OPENLOGIN,				//	�򿪵�¼�Ի���

		EVENT_VALUE_MAINFRAME_END = EVENT_VALUE_MAINFRAME_END ,			//���е��¼�����
	};

	// MainFrame�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_MainFrameMessageValue
	{
	};

	// MainFrame�ܹ������ֱ�ӵ���
	enum E_MainFrameServiceValue
	{
		SERVICE_VALUE_TEST	=	SERVICE_VALUE_MAINFRAME_BEGIN,

		SERVICE_VALUE_GET_MAINWND,				//	��ȡ�����ڵľ����һЩģ����Ҫָ��������Ϊ������
	};

	//===========================================//
	//                   MainFrame����ʹ�õ���event�ṹ								 	  //
	//===========================================//
	struct MainFrameEvent	:	public ExtraInfo
	{
	public:
		MainFrameEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_MAINFRAME;
		}
	};

	enum	E_MainFrameTab
	{
		TAB_FAVORITE,
		TAB_HISTORY,	
	};

	// ����������Ҫ�Ķ�Ӧ��Event
	struct MainFrame_OpenEvent	:	public	MainFrameEvent
	{
	public:
		MainFrame_OpenEvent()
		{
			eventValue	=	 EVENT_VALUE_MAINFRAME_OPEN;
			eTabType		=	TAB_FAVORITE;
		}

		E_MainFrameTab	eTabType;			//	ָ���򿪺���ʾ��һ��Tabҳ��
	};

	//	������������Ҫ�Ķ�Ӧ��Event
	struct MainFrame_HideEvent	:	public	MainFrameEvent
	{
	public:
		MainFrame_HideEvent()
		{
			eventValue	=	 EVENT_VALUE_MAINFRAME_HIDE;
		}
	};

	//===========================================//
	//                   MainFrame����ʹ�õ���Service							 	  //
	//===========================================//
	struct MainFrame_GetWndService
	{
		HWND	hMainWnd;			//	��ǰ�����ڵľ��
	};


};