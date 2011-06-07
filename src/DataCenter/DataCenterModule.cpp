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
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* DataCenterModule::GetModuleName() 
{
	return L"DataCenterModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const DataCenterModule::GetModuleId()
{
	return MODULE_ID_DATACENTER;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void DataCenterModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//����: ProcessMessage
//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
//����: 
//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
//----------------------------------------------------------------------------------------
void DataCenterModule::ProcessMessage(const Message& msg) 
{
}

//----------------------------------------------------------------------------------------
//����: CallDirect
//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
//			����������Ҫͨ�����ߡ�
//����: 
//		@param	lparam			����1
//		@param	rparam			����2
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
		// ����ICON�����ڴ�
		wstring wstrDomain = MiscHelper::GetDomainFromUrl(wstrUrl.c_str());
		m_mapDomain2Icon[wstrDomain] = hIcon;

		//	֪ͨDatabaseģ��
		database::Database_FavIconSaveEvent* pSaveIconEvent = new database::Database_FavIconSaveEvent();
		pSaveIconEvent->srcMId = MODULE_ID_DATACENTER;
		STRNCPY(pSaveIconEvent->szFavoriteUrl, wstrDomain.c_str());
		pSaveIconEvent->nIconDataLen = nIconSize;
		pSaveIconEvent->pIconData = new char[pSaveIconEvent->nIconDataLen];
		memcpy((void*)pSaveIconEvent->pIconData,pIconData, pSaveIconEvent->nIconDataLen);
		m_pModuleManager->PushEvent(*pSaveIconEvent);

		// ֪ͨ������
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

	// ���m_mapDomain2Icon�Ƿ���ڸ�Domain��Ӧ��HICON
	std::map<wstring, HICON>::iterator itr = m_mapDomain2Icon.find(wstrDomain.c_str());
	if( itr != m_mapDomain2Icon.end())
	{
		pGetFavoriteIconService->hIcon = itr->second;
		return;
	}

	// ����ڴ�����û���ҵ����������ݿ��н��в��ң��ҵ�����ص��ڴ���
	database::Database_GetFavoriteIconService getDBFavoriteIconService;
	STRNCPY(getDBFavoriteIconService.szFavoriteUrl, (wchar_t*)wstrDomain.c_str());
	m_pModuleManager->CallService(getDBFavoriteIconService.serviceId, (param)&getDBFavoriteIconService);
	if( getDBFavoriteIconService.hcon != NULL)
	{
		m_mapDomain2Icon[wstrDomain] = getDBFavoriteIconService.hcon;
		pGetFavoriteIconService->hIcon = getDBFavoriteIconService.hcon;
		return;
	}

	// ������ݿ���Ҳ�����ڣ����������ȡ��ִ��Webģ��
	web::Web_GetFavIconReqEvent* pGetFavIconEvent = new web::Web_GetFavIconReqEvent();
	pGetFavIconEvent->srcMId = MODULE_ID_DATACENTER;

	wstring	wstrFavIconUrl = wstrDomain + wstring(L"/favicon.ico");
	STRNCPY(pGetFavIconEvent->szFavoriteUrl, wstrFavIconUrl.c_str());
	m_pModuleManager->PushEvent(*pGetFavIconEvent);
}