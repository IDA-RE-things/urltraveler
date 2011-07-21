#pragma once

// ���ý���Ķ����ͷ�ļ���Ϣ
#include "SndaBase.h"
#include "UrlTravelerHeader.h"

namespace setting
{
	// ���µ�����
	enum EUpdateType
	{
		UPDATE_AUTO,					//	�и��µ�ʱ���Զ����²���װ
		UPDATE_INSTALL_QUERY,	//	�Զ����أ���װ��ʱ��ѯ��
		UPDATE_TIP,							//	���Զ����£��и��µ�ʱ�������ʾ
		UPDATE_CLOSE,					//	�ر��Զ�����
	};

	// Setting���⹫�����¼�
	enum E_SettingEventValue
	{
		EVENT_VALUE_SETTING_OPEN = EVENT_VALUE_SETTING_BEGIN,		//������������
		EVENT_VALUE_SETTING_CLOSE,						//	�رյ�ǰ������

		EVENT_VALUE_SETTING_END = EVENT_VALUE_SETTING_END ,			//���е��¼�����
	};

	// SETTING�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_SettingMessageValue
	{

	};

	// Setting�ܹ������ֱ�ӵ���
	enum E_SettingServiceValue
	{
		SERVICE_VALUE_SETTING_GET_AUTO_START	=	SERVICE_VALUE_SETTING_BEGIN,	//	��ȡ�Ƿ����window�Զ�����
		SERVICE_VALUE_SETTING_GET_REMEMBER_PWD,					//	�Ƿ��ס����
		SERVICE_VALUE_SETTING_GET_AUTO_LOGIN,									//	�Ƿ��Զ���¼
		SERVICE_VALUE_SETTING_GET_CLOSE_WND_WHEN_EXIT,				//	�˳�Ӧ�ó����ʱ���Ƿ�رմ���
		SERVICE_VALUE_SETTING_GET_LOCAL_SYNC,									// �����Զ�ͬ��
		SERVICE_VALUE_SETTING_GET_REMOTE_SYNC,								//	�Ƿ������ͬ��
		SERVICE_VALUE_SETTING_GET_UPDATE_TYPE,									//	��ȡ���µ�����
	};

	//===========================================//
	//                   Setting����ʹ�õ���event�ṹ								 	  //
	//===========================================//
	struct SettingEvent	:	public EventExtraInfo
	{
		SettingEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_SETTING;
		}
	};

	// ����������Ҫ�Ķ�Ӧ��Event
	struct Setting_OpenEvent	:	public	SettingEvent
	{
		Setting_OpenEvent()
		{
			eventValue	=	 EVENT_VALUE_SETTING_OPEN;
		}
	};

	//	�ر���������Ҫ�Ķ�Ӧ��Event
	struct Setting_CloseEvent	:	public	SettingEvent
	{
		Setting_CloseEvent()
		{
			eventValue	=	 EVENT_VALUE_SETTING_CLOSE;
		}
	};

	//===========================================//
	//                   Setting����ʹ�õ���Service�ṹ								 		//
	//===========================================//
	struct Setting_GetStartWithWindowService	:	public Service
	{
		Setting_GetStartWithWindowService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_AUTO_START;
		}

		BOOL	bStart;
	};

#define MAX_ACCOUNT_NAME 64
	struct Setting_GetRememberPwdService	:	public Service
	{
		Setting_GetRememberPwdService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_REMEMBER_PWD;
			ZeroMemory(szAccount, MAX_ACCOUNT_NAME*sizeof(wchar_t));
		}

		wchar_t	szAccount[MAX_ACCOUNT_NAME];			//	��ס������˺�
		BOOL	bStart;
	};

	// �������˺��Ƿ��Զ���¼
	struct Setting_GetAutoLoginService	:	public Service
	{
		Setting_GetAutoLoginService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_AUTO_LOGIN;
			ZeroMemory(szAccount, MAX_ACCOUNT_NAME*sizeof(wchar_t));
		}

		wchar_t	szAccount[MAX_ACCOUNT_NAME];			//	��ס������˺�
		BOOL	bAutoLogin;
	};

	// �ر������ڵ�ʱ���Ƿ��˳����򣬻�����С����������
 	struct Setting_GetExitWhenCloseWndService	:	public Service
	{
		Setting_GetExitWhenCloseWndService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_CLOSE_WND_WHEN_EXIT;
			bExit = FALSE;
		}

		BOOL	bExit;
	};

	// �Ƿ���б����ղؼе��Զ�ͬ��
  	struct Setting_GetAutoLocalSyncService	:	public Service
	{
		Setting_GetAutoLocalSyncService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_LOCAL_SYNC;
			ZeroMemory(szAccount, MAX_ACCOUNT_NAME*sizeof(wchar_t));
			bAutoLocalSync = TRUE;
		}

		wchar_t	szAccount[MAX_ACCOUNT_NAME];			//	��ס������˺�
		BOOL	bAutoLocalSync;
	};

	// �Ƿ��Զ����з������˱���
  	struct Setting_GetAutoRemoteSyncService	:	public Service
	{
		Setting_GetAutoRemoteSyncService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_REMOTE_SYNC;
			ZeroMemory(szAccount, MAX_ACCOUNT_NAME*sizeof(wchar_t));
			bAutoRemoteSync = TRUE;
		}

		wchar_t	szAccount[MAX_ACCOUNT_NAME];			//	��ס������˺�
		BOOL	bAutoRemoteSync;
	};

	// ��������
  	struct Setting_GetUpateTypeService	:	public Service
	{
		Setting_GetUpateTypeService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_UPDATE_TYPE;
			eUpdateType = UPDATE_TIP;
		}

		EUpdateType	eUpdateType;
	};
};