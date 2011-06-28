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
		// Favicon��Ӧ��ͼ��
		EVENT_VALUE_UPDATE_FAVICON_SAVE = EVENT_VALUE_UPDATE_BEGIN,		//��������
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

	/** ��ģ����ʹ�õ�һЩȫ�ֺ궨�峣��
	�����ĺ궨�峣����ʵ������,��Щ�����ڴ����в���Ϊ������ĳ���ʹ��
	��:�ַ�����󳤶ȡ�������󳤶ȵ�
	*/	
#define DB_MAX_LEN_BINARY_BUFFER			(20480+1)

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

	// �洢�ղؼ�ͼ��Ľӿ�
	struct Update_FavIconSaveEvent	:	public UpdateEvent
	{
		Update_FavIconSaveEvent()
		{
			eventValue	=	 EVENT_VALUE_UPDATE_FAVICON_SAVE;
			ZeroMemory(szFavoriteUrl, MAX_PATH);

			nIconDataLen = 0;
			pIconData = NULL;
		}

		~Update_FavIconSaveEvent()
		{
			if( pIconData != NULL)
			{
				delete pIconData;
				pIconData = NULL;
			}

			nIconDataLen = 0;
		}

		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
		int	nIconDataLen;					//	Icon�������Ĳ���
		const char*	pIconData;			//	ICON����
	};

	//============================================================================//
	//                   Update����ʹ�õ���Service�ṹ			              //
	//============================================================================//
	struct Update_GetFavoriteIconService : public Service
	{
		Update_GetFavoriteIconService()
		{
			serviceId = SERVICE_VALUE_UPDATE_FAVICON_LOAD;
			ZeroMemory(szFavoriteUrl, MAX_PATH);
			hcon	=	NULL;
		}

		wchar_t szFavoriteUrl[MAX_PATH];										//	
		HICON	hcon;				//	���ص�ICON	
	};

};