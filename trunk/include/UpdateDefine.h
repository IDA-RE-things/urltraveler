#pragma once

// Update�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace update
{
	// Update���⹫�����¼�
	enum E_UpdateEventValue
	{
		EVENT_VALUE_UPDATE_CHECK_UPDATEINFO = EVENT_VALUE_UPDATE_BEGIN,		// ��鵱ǰӦ�ó����Ƿ���ڸ���
		EVENT_VALUE_UPDATE_SHOW_UPDATE_HINT_WND,			//	��ʾ������ʾ����
		EVENT_VALUE_UPDATE_SHOW_DOWNLOADING_WND,			//	��ʾ���ؽ��ȴ���
	};

	// Update�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_UpdateMessageValue
	{
	};

	// Update�ܹ������ֱ�ӵ���
	enum E_UpdateServiceValue
	{
		SERVICE_VALUE_UPDATE_FAVICON_LOAD = SERVICE_VALUE_UPDATE_BEGIN,					//	�����ض���URL���ղؼ�ͼ��
	};

	//============================================================================//
	//                   Update����ʹ�õ������ݽṹ			                  //
	//============================================================================//



	//============================================================================//
	//                   Update����ʹ�õ���event�ṹ  			              //
	//============================================================================//
	struct UpdateEvent	:	public ExtraInfo
	{
		UpdateEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_UPDATE;
		}
	};

	// ֪ͨ��ʾ��ʾ������Ϣ�Ի���
#define MAX_UPDATE_VERSION_LEN			(32+1)
	struct Update_ShowUpdateInfoEvent	:	public UpdateEvent
	{
		Update_ShowUpdateInfoEvent()
		{
			eventValue = EVENT_VALUE_UPDATE_SHOW_UPDATE_HINT_WND;
			nVersion = 0;
		}

		int nVersion;
	};

	struct Update_ShowUpdateDownloadingEvent	:	public UpdateEvent
	{
		Update_ShowUpdateDownloadingEvent()
		{
			eventValue = EVENT_VALUE_UPDATE_SHOW_DOWNLOADING_WND;
			nLastestVersion = 0;
			ZeroMemory(szDownloadUrl, MAX_PATH*sizeof(wchar_t));
			ZeroMemory(szSavePath, MAX_PATH*sizeof(wchar_t));
		}

		int nLastestVersion;		//	����İ汾

		wchar_t	szDownloadUrl[MAX_PATH];	// ���ص�URL
		wchar_t	szSavePath[MAX_PATH];
	};

	//============================================================================//
	//                   Update����ʹ�õ���Service�ṹ			              //
	//============================================================================//
};