#include "stdafx.h"
#include "DataCenterModule.h"
#include "MiscHelper.h"
#include "DatabaseDefine.h"
#include "WebDefine.h"
#include "ImageHelper.h"

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

DataCenterModule::DataCenterModule()
{

}

DataCenterModule::~DataCenterModule()
{

}

BEGIN_EVENT_MAP(DataCenterModule)
	ON_EVENT(EVENT_VALUE_WEB_GET_FAVICON_RESP, OnEvent_FavoriteIconArrive)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(DataCenterModule)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_VECTOR, OnService_GetFavoriteVector)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA,OnService_GetFavoriteData)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_ICON,OnService_GetFavoriteIcon)
END_SERVICE_MAP()

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

	pGetFavoriteDataService->pFavoriteData = &m_vFavoriteLineData[0];
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

	// 如果数据库中也不存在，则从网络拉取，执行Web模块
	web::Web_GetFavIconReqEvent* pGetFavIconEvent = new web::Web_GetFavIconReqEvent();
	pGetFavIconEvent->srcMId = MODULE_ID_DATACENTER;

	wstring	wstrFavIconUrl = wstrDomain + wstring(L"/favicon.ico");
	STRNCPY(pGetFavIconEvent->szFavoriteUrl, wstrFavIconUrl.c_str());
	m_pModuleManager->PushEvent(*pGetFavIconEvent);
}