#pragma once

// 设置界面的对外的头文件信息
#include "SndaBase.h"
#include "UrlTravelerHeader.h"

namespace setting
{
	// 更新的类型
	enum EUpdateType
	{
		UPDATE_AUTO,					//	有更新的时候自动更新并安装
		UPDATE_INSTALL_QUERY,	//	自动下载，安装的时候询问
		UPDATE_TIP,							//	不自动更新，有更新的时候进行提示
		UPDATE_CLOSE,					//	关闭自动更新
	};

	// Setting对外公开的事件
	enum E_SettingEventValue
	{
		EVENT_VALUE_SETTING_OPEN = EVENT_VALUE_SETTING_BEGIN,		//打开设置主界面
		EVENT_VALUE_SETTING_CLOSE,						//	关闭当前主界面

		EVENT_VALUE_SETTING_END = EVENT_VALUE_SETTING_END ,			//所有的事件结束
	};

	// SETTING能够可能对外发送的广播消息
	enum E_SettingMessageValue
	{

	};

	// Setting能够处理的直接调用
	enum E_SettingServiceValue
	{
		SERVICE_VALUE_SETTING_GET_AUTO_START	=	SERVICE_VALUE_SETTING_BEGIN,	//	获取是否跟随window自动启动
		SERVICE_VALUE_SETTING_GET_REMEMBER_PWD,					//	是否记住密码
		SERVICE_VALUE_SETTING_GET_AUTO_LOGIN,									//	是否自动登录
		SERVICE_VALUE_SETTING_GET_CLOSE_WND_WHEN_EXIT,				//	退出应用程序的时候是否关闭窗口
		SERVICE_VALUE_SETTING_GET_LOCAL_SYNC,									// 本地自动同步
		SERVICE_VALUE_SETTING_GET_REMOTE_SYNC,								//	是否服务器同步
		SERVICE_VALUE_SETTING_GET_UPDATE_TYPE,									//	获取更新的类型
	};

	//===========================================//
	//                   Setting中所使用到的event结构								 	  //
	//===========================================//
	struct SettingEvent	:	public EventExtraInfo
	{
		SettingEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_SETTING;
		}
	};

	// 打开主界面需要的对应的Event
	struct Setting_OpenEvent	:	public	SettingEvent
	{
		Setting_OpenEvent()
		{
			eventValue	=	 EVENT_VALUE_SETTING_OPEN;
		}
	};

	//	关闭主界面需要的对应的Event
	struct Setting_CloseEvent	:	public	SettingEvent
	{
		Setting_CloseEvent()
		{
			eventValue	=	 EVENT_VALUE_SETTING_CLOSE;
		}
	};

	//===========================================//
	//                   Setting中所使用到的Service结构								 		//
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

		wchar_t	szAccount[MAX_ACCOUNT_NAME];			//	记住密码的账号
		BOOL	bStart;
	};

	// 给定的账号是否自动登录
	struct Setting_GetAutoLoginService	:	public Service
	{
		Setting_GetAutoLoginService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_AUTO_LOGIN;
			ZeroMemory(szAccount, MAX_ACCOUNT_NAME*sizeof(wchar_t));
		}

		wchar_t	szAccount[MAX_ACCOUNT_NAME];			//	记住密码的账号
		BOOL	bAutoLogin;
	};

	// 关闭主窗口的时候是否退出程序，还是最小化到任务栏
 	struct Setting_GetExitWhenCloseWndService	:	public Service
	{
		Setting_GetExitWhenCloseWndService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_CLOSE_WND_WHEN_EXIT;
			bExit = FALSE;
		}

		BOOL	bExit;
	};

	// 是否进行本地收藏夹的自动同步
  	struct Setting_GetAutoLocalSyncService	:	public Service
	{
		Setting_GetAutoLocalSyncService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_LOCAL_SYNC;
			ZeroMemory(szAccount, MAX_ACCOUNT_NAME*sizeof(wchar_t));
			bAutoLocalSync = TRUE;
		}

		wchar_t	szAccount[MAX_ACCOUNT_NAME];			//	记住密码的账号
		BOOL	bAutoLocalSync;
	};

	// 是否自动进行服务器端备份
  	struct Setting_GetAutoRemoteSyncService	:	public Service
	{
		Setting_GetAutoRemoteSyncService()
		{
			serviceId	=	SERVICE_VALUE_SETTING_GET_REMOTE_SYNC;
			ZeroMemory(szAccount, MAX_ACCOUNT_NAME*sizeof(wchar_t));
			bAutoRemoteSync = TRUE;
		}

		wchar_t	szAccount[MAX_ACCOUNT_NAME];			//	记住密码的账号
		BOOL	bAutoRemoteSync;
	};

	// 更新类型
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