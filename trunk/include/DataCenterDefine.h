#pragma once

// DataCenter�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>
#include "time.h"

using namespace std;


namespace datacenter
{
	// DataCenter���⹫�����¼�
	enum E_DataCenterEventValue
	{
		EVENT_VALUE_DATACENTER_OPEN = EVENT_VALUE_DATACENTER_BEGIN,		//��������
		EVENT_VALUE_DATACENTER_FAVORITE_ICON_ARRIVE,						//�ղؼ�ͼ���Ѿ�����
		EVENT_VALUE_DATACENTER_ADD_FAVORITE,				// ����һ���µ��ղؼ�¼
		EVENT_VALUE_DATACENTER_ADD_FAVORITE_RESP,		// ����һ���µ��ղؼ�¼����Ӧ
		EVENT_VALUE_DATACENTER_DELETE_FAVORITE,			// ɾ��ĳ���ղص�URL
		EVENT_VALUE_DATACENTER_ADD_FAVORITE_FOLD,		// ����һ���ղؼ�Ŀ¼
		EVENT_VALUE_DATACENTER_DELETE_FAVORITE_FOLD,		// ɾ��ĳ���ղؼе���������, param0Ϊ��Ҫɾ�����ղؼ�id
		EVENT_VALUE_DATACENTER_TREELIST_SELECT,			// ������ĳ����㱻ɾ������Ҫ�������ƶ�����һ�������߸������
		EVENT_VALUE_DATACENTER_SET_AUTOUPDATE,			// ��ȡ�Զ����µ���Ϣ

		EVENT_VALUE_DATACENTER_END = EVENT_VALUE_DATACENTER_END ,		//���е��¼�����
	};

	// DataCenter�ܹ����ܶ��ⷢ�͵Ĺ㲥��Ϣ
	enum E_DataCenterMessageValue
	{
	};

	// DataCenter�ܹ������ֱ�ӵ���
	enum E_DataCenterServiceValue
	{
		SERVICE_VALUE_DATACENTER_GET_FAVORITE_VECTOR	=	SERVICE_VALUE_DATACENTER_BEGIN,
		SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA,
		SERVICE_VALUE_DATACENTER_GET_FAVORITE_ICON,			//	��ȡ����Domain��Ӧ��URL
		SERVICE_VALUE_DATACENTER_CHECK_EXIST_SUBFOLD,		//	�������ļ������Ƿ�������ļ���
		SERVICE_VALUE_DATACENTER_GET_SUBFOLD_ID,				//	��ȡ�����ļ����µ��������ļ��е�id
		SERVICE_VALUE_DATACENTER_GET_AUTOUPDATE,				//	��ȡ�Զ����µ���Ϣ
		SERVICE_VALUE_DATACENTER_REARRANGE_FAVORITE,		//	���ղؼ����ݽ�������
		SERVICE_VALUE_DATACENTER_GET_FAVORITE_NUM_ATFOLD,	//	��ȡָ���ղؼ�Ŀ¼�µ����е��ղؼе���Ŀ
		SERVICE_VALUE_DATACENTER_INIT_FAVORITE,			//	��ʼ���ղؼ��ڴ�
	};

	//===========================================//
	//                   DataCenter����ʹ�õ������ݽṹ			              //
	//===========================================//
	// �����ղؼ����ݽṹ

#define MAX_URL_LEN	1024

	// �ղؼ���ÿһ���ղؼ�¼������
	typedef struct FavoriteLineData
	{
		int32       nId;                //  ID
		wchar_t	    szTitle[MAX_PATH];	//	�����ı�������Ϊ�������ƣ�����ΪURL��title
		int32		nCatId;				//  ����ID
		int32		nPid;		        //	�����ID
		bool        bFolder;            //  �Ƿ���Ŀ¼��㡣�����true����ʾ��Ŀ¼��㣬�������ӽ��
		wchar_t	    szUrl[MAX_URL_LEN];		//	�ղص�URL
		time_t		nAddTimes;		    //	���ӵ�ʱ��
		time_t		nLastModifyTime;	//	����ĸ���ʱ��
		int32		nClickTimes;		//	�������
		uint32		nHashId;			//  Hash
		int32		nOrder;				//	��ǰ����ڵ�ǰ����е�˳��
		bool        bDelete;

		FavoriteLineData()
		{
			nId	=	0;
			nCatId	=	0;
			nPid	=	0;
			bFolder	=	false;
			nAddTimes	=	time(NULL);
			nLastModifyTime	=	time(NULL);
			nClickTimes	=	0;
			nHashId	=	0;
			nOrder	=	0;
			bDelete =   false;

			ZEROMEM(szTitle);
			ZEROMEM(szUrl);
		}

		bool operator < (const FavoriteLineData& x)const
		{
			if (nPid < x.nPid)
			{
				return true;
			}

			return false;
		}

	} FAVORITELINEDATA, *PFAVORITELINEDATA, **PPFAVORITELINEDATA;

	// ��ʷ��¼��ÿһ����ʷ��¼������
	typedef struct HistoryLineData
	{
		wchar_t		szTitle;				//	�����ı�������Ϊ�������ƣ�����ΪURL��title
		int32		nCatId;				//	����ID
		int32		nParentCatId;			//	�����ID�����nParentCatId=0��ʾ��ǰID��Ŀ¼���
		wchar_t		szUrl[MAX_URL_LEN];	//	�ղص�URL
		int32		nAddTimes;			//	���ӵ�ʱ��
		int32		nLastModifyTime;		//	����ĸ���ʱ��
		int32		nClickTimes;			//	�������
		int32		nOrder;				//	��ǰ����ڵ�ǰ����е�˳��

	} HISTORYLINEDATA, *PHISTORYLINEDATA;

#define MAX_COOKIE_NAME_LEN	128
#define MAX_COOKIE_VALUE_LEN	128
#define MAX_COOKIE_HOST_LEN	128

	// ÿһ��Cookie��¼
	typedef struct CookieLineData
	{
		int nId;			
		wchar_t	szName[MAX_COOKIE_NAME_LEN];
		wchar_t	szValue[MAX_COOKIE_VALUE_LEN];
		wchar_t	szHost[MAX_COOKIE_HOST_LEN];
		wchar_t	szPath[MAX_PATH];
		int	nExpiry;
		int	nLastAccessed;
		bool	bIsSecure;
		bool	bIsHttpOnly;

	}COOKIELINEDATA, *PCOOKIELINEDATA;


	//===========================================//
	//                   DataCenter����ʹ�õ���event�ṹ			              //
	//===========================================//
	struct DataCenterEvent	:	public EventExtraInfo
	{
	public:
		DataCenterEvent()
		{
			srcMId	=	MODULE_ID_DATACENTER;
			desMId	=	MODULE_ID_INVALID;
		}
	};

	struct DataCenter_FavoriteIconArriveEvent	: DataCenterEvent 
	{
		DataCenter_FavoriteIconArriveEvent()
		{
			eventValue	=	EVENT_VALUE_DATACENTER_FAVORITE_ICON_ARRIVE;
		}

		wchar_t	szDomain[MAX_PATH];
		HICON	hIcon;
	};

	struct DataCenter_AddFavoriteEvent	: DataCenterEvent 
	{
		DataCenter_AddFavoriteEvent()
		{
			eventValue	=	EVENT_VALUE_DATACENTER_ADD_FAVORITE;
			nParentFavoriteId = 0;
			ZEROMEM(szUrl);
		}

		int nParentFavoriteId;				//	��ǰ�ղ����ڵ��ղ�Ŀ¼
		wchar_t	szTitle[MAX_PATH];			//	�ղصı���
		wchar_t	szUrl[MAX_PATH];			//	�ղص�URL
	};

	// ����DataCenter_AddFavoriteEvent���첽��Ϣ���ڷ��ͺ󣬷�����û����ý�������
	// ����ͨ��ResultEvent֪ͨ
	struct DataCenter_AddFavoriteResultEvent	: DataCenterEvent 
	{
		DataCenter_AddFavoriteResultEvent()
		{
			eventValue	=	EVENT_VALUE_DATACENTER_ADD_FAVORITE_RESP;
			pFavoriteData = NULL;
		}

		FAVORITELINEDATA*	pFavoriteData;
	};

	struct DataCenter_DeleteFavoriteEvent	: DataCenterEvent 
	{
		DataCenter_DeleteFavoriteEvent()
		{
			eventValue	=	EVENT_VALUE_DATACENTER_DELETE_FAVORITE;
			nFavoriteId = 0;
			ZEROMEM(szUrl);
		}

		int nFavoriteId;
		wchar_t	szUrl[MAX_PATH];
	};

	struct DataCenter_AddFavoriteFoldEvent	: DataCenterEvent 
	{
		DataCenter_AddFavoriteFoldEvent()
		{
			eventValue	=	EVENT_VALUE_DATACENTER_ADD_FAVORITE_FOLD;
			nParentFavoriteId = 0;
			ZEROMEM(szTitle);
		}

		int nParentFavoriteId;				//	��ǰ�ղؼ�Ŀ¼�ĸ��ղؼ�Ŀ¼
		wchar_t	szTitle[MAX_PATH];			//	�ղؼеı���
	};

	//===========================================//
	//                   DataCenter����ʹ�õ���Service�ṹ			              //
	//===========================================//
	struct DataCenter_GetFavoriteVectorService : public Service
	{
		DataCenter_GetFavoriteVectorService()
		{
			serviceId	=	SERVICE_VALUE_DATACENTER_GET_FAVORITE_VECTOR;
			pvFavoriteData = NULL;
		}

		std::vector<FAVORITELINEDATA*>*	pvFavoriteData;
	};

	struct DataCenter_GetFavoriteService : public Service
	{
		DataCenter_GetFavoriteService()
		{
			serviceId = SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA;
			nNum = 0;
			ppFavoriteData	=	NULL;
		}

		int nNum;										//	
		FAVORITELINEDATA**		ppFavoriteData;			//	�ղؼе�����	
	};

	// ��ȡ������URL ���Ӧ��ICON
	struct DataCenter_GetFavoriteIconService : public Service
	{
		DataCenter_GetFavoriteIconService()
		{
			serviceId = SERVICE_VALUE_DATACENTER_GET_FAVORITE_ICON;

			ZEROMEM(szDomain);
			hIcon = NULL;
		}

		wchar_t	szDomain[MAX_PATH];
		HICON	hIcon;
	};


	// ���������ղؼ����Ƿ�������ļ���
	struct DataCenter_CheckExistSubFoldService : public Service
	{
		DataCenter_CheckExistSubFoldService()
		{
			serviceId = SERVICE_VALUE_DATACENTER_CHECK_EXIST_SUBFOLD;
			nFoldId	=	0;
			bExistSubFolder	=	FALSE;
			bExistFavorite	=	FALSE;
		}

		int nFoldId;					//	�ղؼе�ID
		BOOL	bExistSubFolder;		//	�Ƿ�������ļ���
		BOOL	bExistFavorite;			//	���ļ������Ƿ�����ղؼ�¼
	};

	struct DataCenter_GetSubFolderIdService : public Service
	{
		DataCenter_GetSubFolderIdService()
		{
			serviceId =  SERVICE_VALUE_DATACENTER_GET_SUBFOLD_ID;
			
			pIdNum = NULL;
			nIdNum = 0;
			nFoldId = 0;
		}

		int nFoldId;

		int nIdNum;
		int*	pIdNum;
	};

	struct DataCenter_GetIsAutoUpdateService : public Service
	{
		DataCenter_GetIsAutoUpdateService()
		{
			serviceId =  SERVICE_VALUE_DATACENTER_GET_AUTOUPDATE;
			bAutoUpdate = FALSE;
		}

		BOOL	bAutoUpdate;
	};

	struct DataCenter_ReArrangeFavoriteService : public Service
	{
		DataCenter_ReArrangeFavoriteService()
		{
			serviceId =  SERVICE_VALUE_DATACENTER_REARRANGE_FAVORITE;
			
			nNum = 0;
			ppFavoriteData = NULL;
		}

		int nNum;										//	
		FAVORITELINEDATA**		ppFavoriteData;			//	�ղؼе�����	
	};

	struct DataCenter_GetFavoriteNumAtFoldService : public Service
	{
		DataCenter_GetFavoriteNumAtFoldService()
		{
			serviceId =  SERVICE_VALUE_DATACENTER_GET_FAVORITE_NUM_ATFOLD;
			
			nFolderId = 0;
			nNum = 0;
		}

		int nFolderId;
		int nNum;	
	};

	struct DataCenter_InitFavoriteDataService : public Service
	{
		DataCenter_InitFavoriteDataService()
		{
			serviceId =  SERVICE_VALUE_DATACENTER_INIT_FAVORITE;
			nNum = 0;
		}

		int nNum;	
	};

};