#pragma once

// Database�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace database
{
	// Database���⹫�����¼�
	enum E_DatabaseEventValue
	{
		// Favicon��Ӧ��ͼ��
		EVENT_VALUE_DATABASE_FAVICON_SAVE = EVENT_VALUE_DATABASE_BEGIN,		//��������
	};

	// Database�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_DatabaseMessageValue
	{
	};

	// Database�ܹ������ֱ�ӵ���
	enum E_DatabaseServiceValue
	{
		SERVICE_VALUE_DATABASE_FAVICON_LOAD = SERVICE_VALUE_DATABASE_BEGIN,					//	�����ض���URL���ղؼ�ͼ��
	};

	/** ��ģ����ʹ�õ�һЩȫ�ֺ궨�峣��
	�����ĺ궨�峣����ʵ������,��Щ�����ڴ����в���Ϊ������ĳ���ʹ��
	��:�ַ�����󳤶ȡ�������󳤶ȵ�
	*/	
#define DB_MAX_LEN_BINARY_BUFFER			(20480+1)

	//============================================================================//
	//                   Database����ʹ�õ������ݽṹ			                  //
	//============================================================================//



	//============================================================================//
	//                   Database����ʹ�õ���event�ṹ  			              //
	//============================================================================//
	struct DatabaseEvent	:	public ExtraInfo
	{
		DatabaseEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_DATABASE;
		}
	};

	// �洢�ղؼ�ͼ��Ľӿ�
	struct Database_FavIconSaveEvent	:	public DatabaseEvent
	{
		Database_FavIconSaveEvent()
		{
			eventValue	=	 EVENT_VALUE_DATABASE_FAVICON_SAVE;
			ZeroMemory(szFavoriteUrl, MAX_PATH);

			nIconDataLen = 0;
			pIconData = NULL;
		}

		~Database_FavIconSaveEvent()
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
	//                   Database����ʹ�õ���Service�ṹ			              //
	//============================================================================//
	struct Database_GetFavoriteIconService : public Service
	{
		Database_GetFavoriteIconService()
		{
			serviceId = SERVICE_VALUE_DATABASE_FAVICON_LOAD;
			ZeroMemory(szFavoriteUrl, MAX_PATH);
			hcon	=	NULL;
		}

		wchar_t szFavoriteUrl[MAX_PATH];										//	
		HICON	hcon;				//	���ص�ICON	
	};

};