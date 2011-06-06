#pragma once

// DataCenter�Ķ����ͷ�ļ���Ϣ

#include "SndaBase.h"
#include "UrlTravelerHeader.h"
#include <vector>

using namespace std;


namespace datacenter
{
	// DataCenter���⹫�����¼�
	enum E_DataCenterEventValue
	{
		EVENT_VALUE_DATACENTER_OPEN = EVENT_VALUE_DATACENTER_BEGIN,		//��������
		EVENT_VALUE_DATACENTER_HIDE,							//	���ص�ǰ������
		EVENT_VALUE_DATACENTER_CLOSE,						//	�رյ�ǰ������

		EVENT_VALUE_DATACENTER_END = EVENT_VALUE_DATACENTER_END ,			//���е��¼�����
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
		SERVICE_VALUE_DATACENTER_GET_FAVORITE_ICON,		//	��ȡ����Domain��Ӧ��URL
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
			nAddTimes	=	0;
			nLastModifyTime	=	0;
			nClickTimes	=	0;
			nHashId	=	0;
			nOrder	=	0;
			bDelete =   false;
		}

		bool operator < (const FavoriteLineData& x)const
		{
			if (nPid < x.nPid)
			{
				return true;
			}

			return false;
		}

	} FAVORITELINEDATA, *PFAVORITELINEDATA; 

	// ��ʷ��¼��ÿһ����ʷ��¼������
	typedef struct HistoryLineData
	{
		wchar_t	    szTitle;			//	�����ı�������Ϊ�������ƣ�����ΪURL��title
		int32		nCatId;				//	����ID
		int32		nParentCatId;		//	�����ID�����nParentCatId=0��ʾ��ǰID��Ŀ¼���
		wchar_t	    szUrl[MAX_URL_LEN];				//	�ղص�URL
		int32		nAddTimes;			//	���ӵ�ʱ��
		int32		nLastModifyTime;	//	����ĸ���ʱ��
		int32		nClickTimes;		//	�������
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
	struct DataCenterEvent	:	public ExtraInfo
	{
	public:
		DataCenterEvent()
		{
			srcMId	=	MODULE_ID_INVALID;
			desMId	=	MODULE_ID_DATACENTER;
		}
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

		std::vector<FAVORITELINEDATA>*	pvFavoriteData;
	};

	struct DataCenter_GetFavoriteService : public Service
	{
		DataCenter_GetFavoriteService()
		{
			serviceId = SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA;
			nNum = 0;
			pFavoriteData	=	NULL;
		}

		int nNum;										//	
		FAVORITELINEDATA*		pFavoriteData;			//	�ղؼе�����	
	};

	// ��ȡ������URL ���Ӧ��ICON
	struct DataCenter_GetFavoriteIconService : public Service
	{
		DataCenter_GetFavoriteIconService()
		{
			serviceId = SERVICE_VALUE_DATACENTER_GET_FAVORITE_ICON;

			ZEROMEM(szDomain);
			nIconSize = 0;
			pIconData = NULL;
		}

		wchar_t	szDomain[MAX_PATH];

		int nIconSize;	
		char*	pIconData;
	};

};