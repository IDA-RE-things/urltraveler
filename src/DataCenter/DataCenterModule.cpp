#include "stdafx.h"
#include "DataCenterModule.h"
#include "MiscHelper.h"
#include "DatabaseDefine.h"
#include "WebDefine.h"
#include "ImageHelper.h"
#include "TxConfig.h"
#include "MiscHelper.h"
#include "StringHelper.h"
#include <algorithm>

using namespace datacenter;
using namespace database;
using namespace web;

namespace datacenter
{
	DataCenterModule*	g_DataCenterModule = NULL;
	DataCenterModuleFactory*	g_DataCenterModuleFactory = NULL;
}


EXPORT_GETMODULEFACTORY(DataCenterModule)
EXPORT_RELEASEMODULEFACTORY(DataCenterModule)

unsigned char szDefaultIcon[] = {
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x00, 0x00, 
	0x01, 0x00, 0x20, 0x00, 0x68, 0x04, 0x00, 0x00, 0x16, 0x00, 
	0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 
	0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 0xAE, 0x4E, 
	0xCE, 0xC0, 0xAE, 0xB7, 0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 
	0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 
	0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 
	0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 
	0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xB7, 0xCE, 0xC0, 
	0xAE, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xCE, 0xC0, 0xAE, 0xB7, 0xD1, 0xC4, 0xB2, 0xFF, 0xBF, 0xAC, 
	0x94, 0xFF, 0xBC, 0xA8, 0x8F, 0xFF, 0xBC, 0xA8, 0x8F, 0xFF, 
	0xBC, 0xA8, 0x8F, 0xFF, 0xBC, 0xA8, 0x8F, 0xFF, 0xBC, 0xA8, 
	0x8F, 0xFF, 0xBC, 0xA8, 0x8F, 0xFF, 0xBC, 0xA8, 0x8F, 0xFF, 
	0xBC, 0xA8, 0x8F, 0xFF, 0xBF, 0xAC, 0x94, 0xFF, 0xD1, 0xC4, 
	0xB2, 0xFF, 0xCE, 0xC0, 0xAE, 0xB7, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 0xAE, 0xFF, 0xEC, 0xE7, 
	0xE0, 0xFF, 0xFE, 0xFD, 0xFB, 0xFF, 0xFE, 0xFC, 0xF9, 0xFF, 
	0xFD, 0xFB, 0xF8, 0xFF, 0xFD, 0xFA, 0xF6, 0xFF, 0xFC, 0xF9, 
	0xF4, 0xFF, 0xFC, 0xF7, 0xF1, 0xFF, 0xFB, 0xF6, 0xEF, 0xFF, 
	0xFB, 0xF5, 0xED, 0xFF, 0xFA, 0xF4, 0xEB, 0xFF, 0xF9, 0xF2, 
	0xE8, 0xFF, 0xEA, 0xE0, 0xD4, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 
	0xAE, 0xFF, 0xFE, 0xFD, 0xFB, 0xFF, 0xFD, 0xFB, 0xF8, 0xFF, 
	0xFD, 0xFA, 0xF6, 0xFF, 0xFC, 0xF9, 0xF4, 0xFF, 0xFC, 0xF7, 
	0xF1, 0xFF, 0xFB, 0xF6, 0xEF, 0xFF, 0xFB, 0xF5, 0xED, 0xFF, 
	0xFA, 0xF4, 0xEB, 0xFF, 0xF9, 0xF2, 0xE8, 0xFF, 0xF9, 0xF1, 
	0xE6, 0xFF, 0xF8, 0xEF, 0xE4, 0xFF, 0xF9, 0xF2, 0xE9, 0xFF, 
	0xCE, 0xC0, 0xAE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xCE, 0xC0, 0xAE, 0xFF, 0xFE, 0xFB, 0xF8, 0xFF, 
	0xFC, 0xF9, 0xF4, 0xFF, 0xFC, 0xF7, 0xF1, 0xFF, 0xFB, 0xF6, 
	0xEF, 0xFF, 0xFB, 0xF5, 0xED, 0xFF, 0xFA, 0xF4, 0xEB, 0xFF, 
	0xF5, 0xED, 0xE2, 0xFF, 0xCA, 0xB7, 0x9F, 0xFF, 0xB6, 0x9F, 
	0x82, 0xFF, 0xC2, 0xAE, 0x94, 0xFF, 0xF4, 0xEB, 0xDD, 0xFF, 
	0xF8, 0xF0, 0xE6, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 0xAE, 0xFF, 
	0xFD, 0xF9, 0xF5, 0xFF, 0xFB, 0xF6, 0xEF, 0xFF, 0xFB, 0xF5, 
	0xED, 0xFF, 0xFA, 0xF4, 0xEB, 0xFF, 0xF9, 0xF2, 0xE8, 0xFF, 
	0xF3, 0xEA, 0xDD, 0xFF, 0xB5, 0x9E, 0x80, 0xFF, 0xA5, 0x8B, 
	0x69, 0xFF, 0xA5, 0x8B, 0x69, 0xFF, 0xA7, 0x8D, 0x6C, 0xFF, 
	0xF2, 0xE8, 0xD9, 0xFF, 0xF6, 0xEF, 0xE3, 0xFF, 0xCE, 0xC0, 
	0xAE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xCE, 0xC0, 0xAE, 0xFF, 0xFC, 0xF8, 0xF2, 0xFF, 0xFA, 0xF4, 
	0xEB, 0xFF, 0xF9, 0xF2, 0xE8, 0xFF, 0xF9, 0xF1, 0xE6, 0xFF, 
	0xF8, 0xEF, 0xE4, 0xFF, 0xC5, 0xB1, 0x98, 0xFF, 0xA5, 0x8B, 
	0x69, 0xFF, 0xA5, 0x8B, 0x69, 0xFF, 0xA5, 0x8B, 0x69, 0xFF, 
	0xBA, 0xA4, 0x88, 0xFF, 0xF2, 0xE7, 0xD8, 0xFF, 0xF4, 0xEC, 
	0xE0, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 0xAE, 0xFF, 0xFB, 0xF5, 
	0xEE, 0xFF, 0xF9, 0xF1, 0xE6, 0xFF, 0xE7, 0xDB, 0xCB, 0xFF, 
	0xBB, 0xA6, 0x8A, 0xFF, 0xEA, 0xDE, 0xCE, 0xFF, 0xAC, 0x93, 
	0x73, 0xFF, 0xA5, 0x8B, 0x69, 0xFF, 0xA5, 0x8B, 0x69, 0xFF, 
	0xA5, 0x8B, 0x69, 0xFF, 0xD8, 0xC8, 0xB3, 0xFF, 0xEF, 0xE4, 
	0xD4, 0xFF, 0xF2, 0xE9, 0xDD, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 
	0xAE, 0xFF, 0xFA, 0xF3, 0xEB, 0xFF, 0xF7, 0xEE, 0xE2, 0xFF, 
	0xAE, 0x95, 0x76, 0xFF, 0xAE, 0x96, 0x77, 0xFF, 0xEF, 0xE4, 
	0xD5, 0xFF, 0xD0, 0xBF, 0xA7, 0xFF, 0xA5, 0x8B, 0x69, 0xFF, 
	0xA5, 0x8B, 0x69, 0xFF, 0xB6, 0xA0, 0x82, 0xFF, 0xED, 0xE2, 
	0xD2, 0xFF, 0xEC, 0xE0, 0xD0, 0xFF, 0xEF, 0xE6, 0xDA, 0xFF, 
	0xCE, 0xC0, 0xAE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xCE, 0xC0, 0xAE, 0xFF, 0xF8, 0xF2, 0xE8, 0xFF, 
	0xF5, 0xEB, 0xDD, 0xFF, 0xE4, 0xD7, 0xC5, 0xFF, 0xEA, 0xDE, 
	0xCD, 0xFF, 0xBA, 0xA4, 0x87, 0xFF, 0xCB, 0xB9, 0xA0, 0xFF, 
	0xCE, 0xBD, 0xA5, 0xFF, 0xBE, 0xAB, 0x90, 0xFF, 0xDE, 0xD0, 
	0xBD, 0xFF, 0xE0, 0xD3, 0xC0, 0xFF, 0xDE, 0xD1, 0xBF, 0xFF, 
	0xE6, 0xDC, 0xCE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 0xAE, 0xFF, 
	0xF7, 0xEF, 0xE5, 0xFF, 0xF3, 0xE9, 0xDA, 0xFF, 0xF2, 0xE7, 
	0xD8, 0xFF, 0xC6, 0xB2, 0x98, 0xFF, 0xA6, 0x8C, 0x6A, 0xFF, 
	0xD2, 0xC2, 0xAB, 0xFF, 0xAC, 0x94, 0x76, 0xFF, 0xC4, 0xB1, 
	0x9A, 0xFF, 0xCD, 0xBC, 0xA6, 0xFF, 0xC9, 0xB8, 0xA2, 0xFF, 
	0xCA, 0xB9, 0xA4, 0xFF, 0xDE, 0xD4, 0xC7, 0xFF, 0xCE, 0xC0, 
	0xAE, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0xCE, 0xC0, 0xAE, 0xFF, 0xF5, 0xED, 0xE2, 0xFF, 0xF0, 0xE5, 
	0xD6, 0xFF, 0xEF, 0xE4, 0xD4, 0xFF, 0xE1, 0xD4, 0xC1, 0xFF, 
	0xE0, 0xD2, 0xBF, 0xFF, 0xD7, 0xC7, 0xB3, 0xFF, 0x95, 0x7B, 
	0x59, 0xFF, 0xE8, 0xE2, 0xDA, 0xFF, 0xFB, 0xFA, 0xF9, 0xFF, 
	0xF6, 0xF3, 0xF0, 0xFF, 0xF3, 0xEF, 0xEA, 0xFF, 0xE1, 0xD8, 
	0xCD, 0xFF, 0xCE, 0xC0, 0xAE, 0x96, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 0xAE, 0xFF, 0xF3, 0xEB, 
	0xDF, 0xFF, 0xED, 0xE2, 0xD2, 0xFF, 0xEC, 0xE0, 0xD0, 0xFF, 
	0xEA, 0xDE, 0xCE, 0xFF, 0xE8, 0xDC, 0xCC, 0xFF, 0xDC, 0xCF, 
	0xBD, 0xFF, 0xB4, 0xA0, 0x86, 0xFF, 0xFC, 0xFB, 0xFA, 0xFF, 
	0xF7, 0xF5, 0xF2, 0xFF, 0xF4, 0xF0, 0xEC, 0xFF, 0xE1, 0xD9, 
	0xCE, 0xFF, 0xCE, 0xC0, 0xAE, 0x96, 0xCE, 0xC0, 0xAE, 0x15, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 
	0xAE, 0xFF, 0xF3, 0xEC, 0xE2, 0xFF, 0xEA, 0xDF, 0xCF, 0xFF, 
	0xE8, 0xDC, 0xCC, 0xFF, 0xE6, 0xDA, 0xCA, 0xFF, 0xE4, 0xD9, 
	0xC8, 0xFF, 0xDA, 0xCC, 0xB9, 0xFF, 0xC6, 0xB4, 0x9E, 0xFF, 
	0xF8, 0xF6, 0xF3, 0xFF, 0xF4, 0xF1, 0xED, 0xFF, 0xE1, 0xD9, 
	0xCE, 0xFF, 0xCE, 0xC0, 0xAE, 0x96, 0xCE, 0xC0, 0xAE, 0x15, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0xCE, 0xC0, 0xAE, 0xBD, 0xEF, 0xE9, 0xE1, 0xFF, 
	0xF6, 0xF2, 0xEC, 0xFF, 0xF5, 0xF1, 0xEB, 0xFF, 0xF5, 0xF0, 
	0xEA, 0xFF, 0xF4, 0xEF, 0xE9, 0xFF, 0xEC, 0xE6, 0xDD, 0xFF, 
	0xDD, 0xD4, 0xC7, 0xFF, 0xF7, 0xF4, 0xF1, 0xFF, 0xE2, 0xD9, 
	0xCF, 0xFF, 0xCE, 0xC0, 0xAE, 0x96, 0xCE, 0xC0, 0xAE, 0x15, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCE, 0xC0, 0xAE, 0x54, 
	0xCE, 0xC0, 0xAE, 0xBD, 0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 
	0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 
	0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0xFF, 0xCE, 0xC0, 
	0xAE, 0xFF, 0xCE, 0xC0, 0xAE, 0x96, 0xCE, 0xC0, 0xAE, 0x15, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 
	0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 
	0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 
	0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 
	0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 
	0x80, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x80, 0x03, 
	0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x80, 0x0F, 0x00, 0x00
};


DataCenterModule::DataCenterModule()
{
	m_hDefaultIcon = ImageHelper::CreateIconFromBuffer((LPBYTE)szDefaultIcon, sizeof(szDefaultIcon), 16);
	m_bAutoUpdate = FALSE;
}

DataCenterModule::~DataCenterModule()
{
	if (m_hDefaultIcon)
	{
		DestroyIcon(m_hDefaultIcon);
	}
}

BEGIN_EVENT_MAP(DataCenterModule)
	ON_EVENT(EVENT_VALUE_WEB_GET_FAVICON_RESP, OnEvent_FavoriteIconArrive)
	ON_EVENT(EVENT_VALUE_DATACENTER_ADD_FAVORITE, OnEvent_AddFavorite)
	ON_EVENT(EVENT_VALUE_DATACENTER_DELETE_FAVORITE, OnEvent_DeleteFavorite)
	ON_EVENT(EVENT_VALUE_DATACENTER_ADD_FAVORITE_FOLD, OnEvent_AddFavoriteFolder)
	ON_EVENT(EVENT_VALUE_DATACENTER_DELETE_FAVORITE_FOLD, OnEvent_DeleteFavoriteFolder)
	ON_EVENT(EVENT_VALUE_DATACENTER_SET_AUTOUPDATE, OnEvent_SetAutoUpdate)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(DataCenterModule)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_VECTOR, OnService_GetFavoriteVector)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA,OnService_GetFavoriteData)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_ICON,OnService_GetFavoriteIcon)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_CHECK_EXIST_SUBFOLD, OnService_CheckExistSubFolder)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_SUBFOLD_ID, OnService_GetSubFolderId)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_AUTOUPDATE, OnService_GetAutoUpdate)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_REARRANGE_FAVORITE,OnService_ReArrangeFavorite)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_NUM_ATFOLD,OnService_GetFavoriteNumAtFold)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_INIT_FAVORITE,OnService_InitFavoriteData)
	
END_SERVICE_MAP()

//----------------------------------------------------------------------------------------
//名称: Load
//描述: 主程序通过该方法对模块进行加载
//参数: 
//		@param	pManager			主模块总线的指针	
//返回: 
//		如果加载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
BOOL DataCenterModule::Load(IModuleManager* pManager)
{
	__super::Load(pManager);

	wchar_t* pConfig = MiscHelper::GetConfig();
	ASSERT(pConfig != NULL);

	CTxConfig txConfig;
	txConfig.ParseConfig(StringHelper::UnicodeToANSI(pConfig).c_str());

	string strAutoUpdate = txConfig.GetValue(L"AutoUpdate");
	if( strAutoUpdate == "0" || StringHelper::TrimAll(strAutoUpdate) == "")
		m_bAutoUpdate = FALSE;
	else 
		m_bAutoUpdate = TRUE;

	return TRUE;
}

BOOL DataCenterModule::Unload()
{
	__super::Unload();

	for(size_t i=0; i<m_vFavoriteLineData.size(); i++)
	{
		PFAVORITELINEDATA pData = m_vFavoriteLineData[i];
		if( pData == NULL)
			continue;

		delete pData;
	}

	return TRUE;
}


//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* DataCenterModule::GetModuleName() 
{
	return L"DataCenterModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const DataCenterModule::GetModuleId()
{
	return MODULE_ID_DATACENTER;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void DataCenterModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//名称: ProcessMessage
//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
//参数: 
//		@param	msg			需要处理的广播消息
//----------------------------------------------------------------------------------------
void DataCenterModule::ProcessMessage(const Message& msg) 
{
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lparam			参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 DataCenterModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void	DataCenterModule::OnEvent_FavoriteIconArrive(Event* pEvent)
{
	Web_GetFavIconRespEvent*	pGetFavIconRespEvent = (Web_GetFavIconRespEvent*)pEvent->m_pstExtraInfo;
	if( pGetFavIconRespEvent == NULL)
		return;

	wstring	wstrUrl = pGetFavIconRespEvent->szFavoriteUrl;
	int nIconSize = pGetFavIconRespEvent->nIconSize;
	char* pIconData = pGetFavIconRespEvent->pIconData;

	HICON hIcon = ImageHelper::CreateIconFromBuffer((LPBYTE)pIconData, nIconSize, 16);
	if( hIcon != NULL)
	{
		// 将该ICON加入内存
		wstring wstrDomain = MiscHelper::GetDomainFromUrl(wstrUrl.c_str());
		m_mapDomain2Icon[wstrDomain] = hIcon;

		//	通知Database模块
		database::Database_FavIconSaveEvent* pSaveIconEvent = new database::Database_FavIconSaveEvent();
		pSaveIconEvent->srcMId = MODULE_ID_DATACENTER;
		STRNCPY(pSaveIconEvent->szFavoriteUrl, wstrDomain.c_str());
		pSaveIconEvent->nIconDataLen = nIconSize;
		pSaveIconEvent->pIconData = new char[pSaveIconEvent->nIconDataLen];
		memcpy((void*)pSaveIconEvent->pIconData,pIconData, pSaveIconEvent->nIconDataLen);
		m_pModuleManager->PushEvent(*pSaveIconEvent);

		// 通知主界面
		DataCenter_FavoriteIconArriveEvent*	pFavoriteIconArriveEvent = new DataCenter_FavoriteIconArriveEvent();
		pFavoriteIconArriveEvent->desMId = MODULE_ID_MAINFRAME;
		STRNCPY(pFavoriteIconArriveEvent->szDomain, wstrDomain.c_str());
		pFavoriteIconArriveEvent->hIcon = hIcon;
		m_pModuleManager->PushEvent(*pFavoriteIconArriveEvent);
	}
}

bool CompareFavoriteData (const FAVORITELINEDATA* i,  const FAVORITELINEDATA* j) 
{ 
	return i->nPid < j->nPid;
}

void	DataCenterModule::OnEvent_AddFavorite(Event* pEvent)
{
	DataCenter_AddFavoriteEvent* pAddFavoriteEvent = (DataCenter_AddFavoriteEvent*)pEvent->m_pstExtraInfo;
	if( pAddFavoriteEvent == NULL)
		return;

	int nParentId = pAddFavoriteEvent->nParentFavoriteId;
	wchar_t* pszTitle = pAddFavoriteEvent->szTitle;
	wchar_t* pszUrl = pAddFavoriteEvent->szUrl;

	PFAVORITELINEDATA pFavoriteData = new FAVORITELINEDATA();

	PFAVORITELINEDATA pLastData = m_vFavoriteLineData[m_vFavoriteLineData.size() - 1];
	if( pLastData == NULL)
		pFavoriteData->nId = 0;
	else
		pFavoriteData->nId = m_vFavoriteLineData[m_vFavoriteLineData.size() - 1]->nId + 1;

	pFavoriteData->nPid = nParentId;
	STRNCPY(pFavoriteData->szTitle, pszTitle);
	STRNCPY(pFavoriteData->szUrl, pszUrl);
	m_vFavoriteLineData.insert(m_vFavoriteLineData.begin(),1,pFavoriteData);

	// 进行广度遍历排序
	std::sort(m_vFavoriteLineData.begin(), m_vFavoriteLineData.end(), CompareFavoriteData);

	// 进行排序
	DataCenter_ReArrangeFavoriteService service;
	GetModuleManager()->CallService(service.serviceId, (param)&service);

	// 然后通知增加成功
	DataCenter_AddFavoriteResultEvent * pRespEvent = new DataCenter_AddFavoriteResultEvent();
	pRespEvent->srcMId = MODULE_ID_DATACENTER;
	pRespEvent->desMId = pEvent->srcMId;
	pRespEvent->pFavoriteData = pFavoriteData;
	GetModuleManager()->PushEvent(*pRespEvent);
}

void	DataCenterModule::OnEvent_DeleteFavorite(Event* pEvent)
{
	DataCenter_DeleteFavoriteEvent* pDeleteFavoriteEvent = (DataCenter_DeleteFavoriteEvent*)pEvent->m_pstExtraInfo;
	if( pDeleteFavoriteEvent == NULL)
		return;

	int nId = pDeleteFavoriteEvent->nFavoriteId;
	wstring	wstrUrl = pDeleteFavoriteEvent->szUrl;

	for( size_t i = 0; i<m_vFavoriteLineData.size(); i++)
	{
		FAVORITELINEDATA* pData = m_vFavoriteLineData[i];
		if( pData == NULL)
			continue;

		if( pData->nPid == nId && wstrUrl == pData->szUrl)
		{
			pData->bDelete = true;
		}
	}
}

void	DataCenterModule::OnEvent_AddFavoriteFolder(Event* pEvent)
{
	DataCenter_AddFavoriteFoldEvent* pAddFavoriteFoldEvent = (DataCenter_AddFavoriteFoldEvent*)pEvent->m_pstExtraInfo;
	if( pAddFavoriteFoldEvent == NULL)
		return;

	int nParentId = pAddFavoriteFoldEvent->nParentFavoriteId;
	wchar_t* pszTitle = pAddFavoriteFoldEvent->szTitle;

	PFAVORITELINEDATA pFavoriteData = new FAVORITELINEDATA();
	pFavoriteData->bFolder = true;

	PFAVORITELINEDATA pLastData = m_vFavoriteLineData[m_vFavoriteLineData.size() - 1];
	if( pLastData == NULL)
		pFavoriteData->nId = 0;
	else
		pFavoriteData->nId = m_vFavoriteLineData[m_vFavoriteLineData.size() - 1]->nId + 1;

	pFavoriteData->nPid = nParentId;
	STRNCPY(pFavoriteData->szTitle, pszTitle);
	m_vFavoriteLineData.push_back(pFavoriteData);

	// 进行广度遍历排序
	std::sort(m_vFavoriteLineData.begin(), m_vFavoriteLineData.end(), CompareFavoriteData);

	// 进行排序
	DataCenter_ReArrangeFavoriteService service;
	GetModuleManager()->CallService(service.serviceId, (param)&service);

	// 然后通知增加成功
	DataCenter_AddFavoriteFoldResultEvent * pRespEvent = new DataCenter_AddFavoriteFoldResultEvent();
	pRespEvent->srcMId = MODULE_ID_DATACENTER;
	pRespEvent->desMId = pEvent->srcMId;
	pRespEvent->nParentFavoriteId =	nParentId;
	pRespEvent->pFavoriteData = pFavoriteData;
	GetModuleManager()->PushEvent(*pRespEvent);	
}

void	DataCenterModule::OnEvent_DeleteFavoriteFolder(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_DATACENTER_DELETE_FAVORITE_FOLD)
	{
		ASSERT(0);
		return;
	}

	int nSelectIndex = 0;
	int nDeleteId = pEvent->param0;

	FAVORITELINEDATA*	pData = m_vFavoriteLineData[nDeleteId-1];
	if( pData == NULL)
	{
		ASSERT(0);
		return;
	}

	int nParentId = pData->nPid;

	// 找到当前结点的前一个兄弟结点
	int nPid = 0;
	int i = nDeleteId - 2;
	for( ; i>=0; i--)
	{
		pData = m_vFavoriteLineData[i];
		if(pData == NULL)
			continue;

		if( pData->nPid == nParentId && pData->bFolder == true && pData->bDelete == false)
			break;

		if( pData->nId == nParentId)
		{
			nPid = pData->nId;
		}
	}

	// 没有找到前一个兄弟结点，则找到它的父结点
	if( i == -1)
	{
		if( nPid == 0)
		{
			// 选中收藏夹根结点
			GetModuleManager()->PushEvent(
				MakeEvent<MODULE_ID_DATACENTER>()(EVENT_VALUE_DATACENTER_TREELIST_SELECT, 
				MODULE_ID_MAINFRAME, 0));
		}
		else
			 nSelectIndex = nPid-1;
	}
	else 
		nSelectIndex = i;

	DeleteFavoriteFold(nDeleteId);
	pData = m_vFavoriteLineData[nSelectIndex];
	if( pData != NULL)
	{
		// 删除之后，如果移动到当前结点的上一个结点。如果上一个结点不存在，则移动到父结点。
		GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_DATACENTER>()(EVENT_VALUE_DATACENTER_TREELIST_SELECT, 
			MODULE_ID_MAINFRAME, pData->nId));
	}
}


void	DataCenterModule::OnEvent_SetAutoUpdate(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_DATACENTER_SET_AUTOUPDATE)
	{
		ASSERT(0);
		return;
	}

	int nParam0 = (int)pEvent->param0;
	if( nParam0 == 0)
		m_bAutoUpdate = FALSE;
	else
		m_bAutoUpdate = TRUE;
}

void	DataCenterModule::OnService_GetFavoriteVector(ServiceValue lServiceValue, param	lParam)
{
	DataCenter_GetFavoriteVectorService* pGetFavoriteServiceVector = (DataCenter_GetFavoriteVectorService*)lParam;
	ASSERT( pGetFavoriteServiceVector != NULL);

	pGetFavoriteServiceVector->pvFavoriteData = &m_vFavoriteLineData;
}

void DataCenterModule::OnService_GetFavoriteData(ServiceValue lServiceValue, param	lParam)
{
	DataCenter_GetFavoriteService* pGetFavoriteDataService = (DataCenter_GetFavoriteService*)lParam;
	ASSERT( pGetFavoriteDataService != NULL);

	pGetFavoriteDataService->ppFavoriteData = &m_vFavoriteLineData[0];
	pGetFavoriteDataService->nNum = m_vFavoriteLineData.size();
}

void	DataCenterModule::OnService_GetFavoriteIcon(ServiceValue lServiceValue, param lParam)
{
	DataCenter_GetFavoriteIconService* pGetFavoriteIconService = (DataCenter_GetFavoriteIconService*)lParam;
	ASSERT(pGetFavoriteIconService != NULL);

	wstring	wstrDomain = pGetFavoriteIconService->szDomain;
	if( wstrDomain == L"")
		return;

	wstrDomain = MiscHelper::GetDomainFromUrl((wchar_t*)wstrDomain.c_str());

	// 检查m_mapDomain2Icon是否存在该Domain对应的HICON
	std::map<wstring, HICON>::iterator itr = m_mapDomain2Icon.find(wstrDomain.c_str());
	if( itr != m_mapDomain2Icon.end())
	{
		pGetFavoriteIconService->hIcon = itr->second;
		return;
	}

	// 如果内存里面没有找到，则向数据库中进行查找，找到后加载到内存中
	database::Database_GetFavoriteIconService getDBFavoriteIconService;
	STRNCPY(getDBFavoriteIconService.szFavoriteUrl, (wchar_t*)wstrDomain.c_str());
	m_pModuleManager->CallService(getDBFavoriteIconService.serviceId, (param)&getDBFavoriteIconService);
	if( getDBFavoriteIconService.hcon != NULL)
	{
		m_mapDomain2Icon[wstrDomain] = getDBFavoriteIconService.hcon;
		pGetFavoriteIconService->hIcon = getDBFavoriteIconService.hcon;
		return;
	}
	//如果db也没有查找到,直接返回默认的hIcon
	else
	{
		pGetFavoriteIconService->hIcon = m_hDefaultIcon;
	}

	// 如果数据库中也不存在，则从网络拉取，执行Web模块
	web::Web_GetFavIconqService getFavIconService ;
	getFavIconService.srcMId = MODULE_ID_DATACENTER;

	wstring	wstrFavIconUrl = wstrDomain + wstring(L"/favicon.ico");
	STRNCPY(getFavIconService.szFavoriteUrl, wstrFavIconUrl.c_str());
	m_pModuleManager->CallService(getFavIconService.serviceId, (param)&getFavIconService);
}

void	DataCenterModule::OnService_CheckExistSubFolder(ServiceValue	lServiceValue, param	 lParam)
{
	DataCenter_CheckExistSubFoldService* pCheckExistSubFolderService = (DataCenter_CheckExistSubFoldService*)lParam;
	ASSERT(pCheckExistSubFolderService != NULL);

	int nId = pCheckExistSubFolderService->nFoldId;
	for(size_t i=0; i<m_vFavoriteLineData.size(); i++)
	{
		FAVORITELINEDATA* pData = m_vFavoriteLineData[i];
		if( pData == NULL)
			continue;

		if( pData->nPid == nId)
		{
			if( pData->bFolder == TRUE)
			{
				pCheckExistSubFolderService->bExistSubFolder = TRUE;
			}
			else
			{
				pCheckExistSubFolderService->bExistFavorite	=	TRUE;
			}
		}

		if( pCheckExistSubFolderService->bExistSubFolder == TRUE 
			&& pCheckExistSubFolderService->bExistFavorite == TRUE)
			break;
	}
}

void	DataCenterModule::GetSubFolderId(std::vector<int>* pvId, int nId)
{
	if( pvId == NULL)
		return;

	for( size_t i=0; i<m_vFavoriteLineData.size(); i++)
	{
		FAVORITELINEDATA* pData = m_vFavoriteLineData[i];
		if( pData == NULL)
			continue;

		if( pData->bDelete == TRUE)
			continue;

		if( pData->nPid == nId)
		{
			if( pData->bFolder == true)
			{
				pvId->push_back(pData->nId);
				GetSubFolderId(pvId, pData->nId);
			}
		}
	}
}

void	DataCenterModule::OnService_GetSubFolderId(ServiceValue lServiceValue, param lParam)
{
	DataCenter_GetSubFolderIdService* pGetSubFolderIdService = (DataCenter_GetSubFolderIdService*)lParam;
	ASSERT(pGetSubFolderIdService != NULL);

	std::vector<int>	vSubFolderId;
	int nId = pGetSubFolderIdService->nFoldId;
	GetSubFolderId(&vSubFolderId, nId);

	pGetSubFolderIdService->nIdNum = vSubFolderId.size();
	pGetSubFolderIdService->pIdNum = new int[vSubFolderId.size()];
	for(size_t i=0; i<vSubFolderId.size(); i++)
	{
		pGetSubFolderIdService->pIdNum[i] = vSubFolderId[i];	
	}
}

void	DataCenterModule::OnService_GetAutoUpdate(ServiceValue lServiceValue, param lParam)
{
	DataCenter_GetIsAutoUpdateService* pGetAutoUpdateService = (DataCenter_GetIsAutoUpdateService*)lParam;
	ASSERT(pGetAutoUpdateService != NULL);

	pGetAutoUpdateService->bAutoUpdate = m_bAutoUpdate;
}

void	DataCenterModule::OnService_ReArrangeFavorite(ServiceValue lServiceValue, param lParam)
{
	DataCenter_ReArrangeFavoriteService* pReArrangeService = (DataCenter_ReArrangeFavoriteService*)lParam;
	ASSERT(pReArrangeService != NULL);

	PFAVORITELINEDATA* ppData = &m_vFavoriteLineData[0];
	int nLen = m_vFavoriteLineData.size();

	//最坏时间复杂度O(N^2)
	for (int i = 0; i < nLen; i++)
	{
		//如果该结点的nId不是数组下标+1,则需要修正
		if ((ppData[i]->nId != i + 1))
		{
			//扫描所有以该结点为父结点的结点，并修正这些结点的nPid
			for (int j = 0; j < nLen; j++)
			{
				if (ppData[j]->nPid == ppData[i]->nId)
				{
					ppData[j]->nPid = i + 1;
				}
			}

			ppData[i]->nId = i + 1;
		}
	}

	pReArrangeService->nNum = nLen;
	pReArrangeService->ppFavoriteData  = &m_vFavoriteLineData[0];
}

/*
void	DataCenterModule::OnService_ReArrangeFavorite(ServiceValue lServiceValue, param lParam)
{
	DataCenter_ReArrangeFavoriteService* pReArrangeService = (DataCenter_ReArrangeFavoriteService*)lParam;
	ASSERT(pReArrangeService != NULL);

	int nLen = m_vFavoriteLineData.size();

	//最坏时间复杂度O(N^2)
	for (int i = 0; i < nLen; i++)
	{
		FAVORITELINEDATA* pData = m_vFavoriteLineData[i];
		if( pData == NULL)
			continue;

		//如果该结点的nId不是数组下标+1,则需要修正
		if (pData->nId != i + 1)
		{
			//扫描所有以该结点为父结点的结点，并修正这些结点的nPid
			for (int j = 0; j < nLen; j++)
			{
				if (m_vFavoriteLineData[j]->nPid == pData->nId)
				{
					pData->nPid = i + 1;
				}
			}

			pData->nId = i + 1;
		}
	}

	pReArrangeService->nNum = nLen;
	pReArrangeService->ppFavoriteData  = &m_vFavoriteLineData[0];
}
*/

void	DataCenterModule::OnService_GetFavoriteNumAtFold(ServiceValue lServiceValue, param lParam)
{
	DataCenter_GetFavoriteNumAtFoldService* pService = (DataCenter_GetFavoriteNumAtFoldService*)lParam;
	ASSERT(pService != NULL);

	int nFolderId = pService->nFolderId;

	std::vector<int>	vId;
	GetSubFolderId(&vId, nFolderId);

	 pService->nNum = vId.size();
}

void	DataCenterModule::OnService_InitFavoriteData(ServiceValue lServiceValue, param lParam)
{
	DataCenter_InitFavoriteDataService* pService = (DataCenter_InitFavoriteDataService*)lParam;
	ASSERT(pService != NULL);

	int nNum = pService->nNum;

	m_vFavoriteLineData.resize(nNum);
	for(int i=0; i<nNum; i++)
	{
		m_vFavoriteLineData[i] = new FAVORITELINEDATA();
	}
}

void DataCenterModule::DeleteFavoriteFold( int nId )
{
	for( size_t i=0; i<m_vFavoriteLineData.size(); i++)
	{
		FAVORITELINEDATA* pData = m_vFavoriteLineData[i];
		if( pData == NULL)
			continue;

		if( pData->bDelete == TRUE)
			continue;

		if( pData->nId == nId)
		{
			pData->bDelete = true;
		}

		if( pData->nPid == nId)
		{
			if( pData->bFolder == true)
			{
				pData->bDelete = true;
				DeleteFavoriteFold(pData->nId);
			}
			else
			{
				pData->bDelete = true;
			}
		}
	}
}