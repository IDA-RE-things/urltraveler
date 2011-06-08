#pragma once

// MainFrame�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"


namespace mainframe
{
	// MainFrame���⹫�����¼�
	enum E_MainFrameEventValue
	{
		EVENT_VALUE_MAINFRAME_OPEN = EVENT_VALUE_MAINFRAME_BEGIN,		//��������
		EVENT_VALUE_MAINFRAME_CLOSE,						//	�رյ�ǰ������
		EVENT_VALUE_MAINFRAME_SHOW,						//	��ʾ���Ի���
		EVENT_VALUE_MAINFRAME_HIDE,							//	���ص�ǰ������
		EVENT_VALUE_MAINFRAME_OPENLOGIN,				//	�򿪵�¼�Ի���
		EVENT_VALUE_MAINFRAME_DELETE_FAVORITE,				//	ɾ��ĳ���������ղؼм�¼
		EVENT_VALUE_MAINFRAME_DELETE_FAVORITE_FOLD,	//	ɾ��ĳ���������ղؼ�Ŀ¼
		EVENT_VALUE_MAINFRAME_OPEN_URL,				//	param0 Ϊ��Ҫ�򿪵�URL��index
		EVENT_VALUE_MAINFRAME_COPY_URL,				//	param0 Ϊ��Ҫ������URL��index
		EVENT_VALUE_MAINFRAME_EDIT_URL,
		EVENT_VALUE_MAINFRAME_RENAME_URL,
		EVENT_VALUE_MAINFRAME_SHARE_URL,

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
			srcMId	=	MODULE_ID_MAINFRAME;
			desMId	=	MODULE_ID_INVALID;
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

	// ɾ��ĳ���ղؼ�¼
	struct MainFrame_DeleteFavoriteEvent	:	public MainFrameEvent
	{
		MainFrame_DeleteFavoriteEvent()
		{
			eventValue	=	EVENT_VALUE_MAINFRAME_DELETE_FAVORITE;
			nDeleteNodeId = -1;
		}

		//	��Ҫɾ����URL������
		int nDeleteNodeId;	
	};

	//===========================================//
	//                   MainFrame����ʹ�õ���Service							 	  //
	//===========================================//
	struct MainFrame_GetWndService
	{
		void*		pBaseWnd;			//	��ǰ�����ڵľ��
	};
};