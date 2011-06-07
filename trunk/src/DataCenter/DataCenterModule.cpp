#include "stdafx.h"
#include "DataCenterModule.h"
#include "MiscHelper.h"

using namespace datacenter;

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
END_EVENT_MAP()

BEGIN_SERVICE_MAP(DataCenterModule)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_VECTOR, OnService_GetFavoriteVector)
	ON_SERVICE(SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA,OnService_GetFavoriteData)
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
	wstrDomain = MiscHelper::GetTopDomainUrl((wchar_t*)wstrDomain.c_str());

	// ���m_mapDomain2Icon�Ƿ���ڸ�Domain��Ӧ��HICON
}
