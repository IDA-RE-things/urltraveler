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
		SERVICE_VALUE_DATABASE_FAVICON_LOAD,					//	�����ض���URL���ղؼ�ͼ��
	};

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
		}

		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
		HICON	hIcon;					//	���ص�ͼ������
	};

	//============================================================================//
	//                   Database����ʹ�õ���Service�ṹ			              //
	//============================================================================//
	struct Database_GetFavoriteIconService
	{
		Database_GetFavoriteIconService()
		{
			ZeroMemory(szFavoriteUrl, MAX_PATH);
			hcon	=	NULL;
		}

		wchar_t szFavoriteUrl[MAX_PATH];										//	
		HICON	hcon;				//	���ص�ICON	
	};

};