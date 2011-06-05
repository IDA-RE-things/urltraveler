#pragma once

// Web�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace web
{
	// Web���⹫�����¼�
	enum E_WebEventValue
	{
		// Favicon��Ӧ��ͼ��
		EVENT_VALUE_WEB_GET_FAVICON = EVENT_VALUE_WEB_BEGIN,		// ֪ͨȥ��ȡFavoriteͼ��
	};

	// Web�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_WebMessageValue
	{
	};

	// Web�ܹ������ֱ�ӵ���
	enum E_WebServiceValue
	{
		SERVICE_VALUE_WEB_FAVICON_LOAD = SERVICE_VALUE_WEB_BEGIN,					//	�����ض���URL���ղؼ�ͼ��
	};

	//============================================================================//
	//                   Web����ʹ�õ������ݽṹ			                  //
	//============================================================================//



	//============================================================================//
	//                   Web����ʹ�õ���event�ṹ  			              //
	//============================================================================//
	struct WebEvent	:	public ExtraInfo
	{
		WebEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_WEB;
		}
	};

	// �����������ղؼ�ICONͼ��
	struct Web_GetFavIconEvent	:	public WebEvent
	{
		Web_GetFavIconEvent()
		{
			eventValue	=	 EVENT_VALUE_WEB_GET_FAVICON;
			ZeroMemory(szFavoriteUrl, MAX_PATH);
		}

		// �ղؼ�ͼ���URL
		wchar_t	szFavoriteUrl[MAX_PATH];		//	URL
	};
};