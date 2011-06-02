#include "stdafx.h"
#include "DataCenterModule.h"

using namespace datacenter;

namespace datacenter
{
	DataCenterModule*	g_DataCenterModule = NULL;
	DataCenterModuleFactory*	g_DataCenterModuleFactory = NULL;
}

// �������ʵ��
IModuleFactory*	GetModuleFactory()
{
	if( g_DataCenterModuleFactory == NULL)
	{
		g_DataCenterModuleFactory = new DataCenterModuleFactory();
		g_DataCenterModuleFactory->QueryModulePoint(1, (IModule*&)g_DataCenterModule);
		
		ASSERT( g_DataCenterModule != NULL);
	}

	return g_DataCenterModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_DataCenterModuleFactory == p);
	if( g_DataCenterModuleFactory  != NULL)
	{
		delete g_DataCenterModuleFactory;
		g_DataCenterModuleFactory = NULL;
	}
}

DataCenterModule::DataCenterModule()
{

}

DataCenterModule::~DataCenterModule()
{

}

BEGIN_EVENT_MAP(DataCenterModule)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(DataCenterModule)
	ON_SERVICE(SERVICE_VALUE__DATACENTER_GET_FAVORITE_VECTOR, OnService_GetFavoriteVector)
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
int32 DataCenterModule::CallDirect(const param lparam, param wparam) 
{
	return -1;
}

//----------------------------------------------------------------------------------------
//����: PaybackExtraInfo
//����: ĳ��ģ��������Զ���ĸ��ӵ�������Ҫ���������ģ�飬��ô�����Թ���һ��ExtraInfo�ṹ
//	�������ģ��ʹ����Ϻ󣬸ýṹ���뱻�ͷţ����������ڴ�й¶���ͷű�����ģ��������ɡ�
//	ĳ��ģ�鶼�����ṩһ��PaybackExtraInfo�ӿڣ��ͷ��Լ����Զ������������
//����: 
//		@param	valudId			��Ӧ��pExtraInfo��ֵ���ڲ����ݸ�ֵ���ж�Ӧ���ͷţ���ֵֻ��ģ���Լ����
//		@param	pExtraInfo	��Ҫ�ͷŵ�ExtraInfo����
//----------------------------------------------------------------------------------------
void DataCenterModule::PaybackExtraInfo(uint32 valudId, void* pExtraInfo)
{
	return;
}


void	DataCenterModule::OnService_GetFavoriteVector(ServiceValue lServiceValue, param	lParam)
{
	DataCenter_GetFavoriteVectorService* pGetFavoriteServiceVector = (DataCenter_GetFavoriteVectorService*)lParam;
	ASSERT( pGetFavoriteServiceVector != NULL);

	pGetFavoriteServiceVector->pvFavoriteData = &m_vFavoriteLineData;
}
