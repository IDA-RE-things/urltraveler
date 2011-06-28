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
		}

		~Update_FavIconSaveEvent()
		{
		}
	};

	//============================================================================//
	//                   Update����ʹ�õ���Service�ṹ			              //
	//============================================================================//
};