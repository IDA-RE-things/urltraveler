#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImp.h"
#include "PlugIn.h"
#include <vector>

using namespace std;


extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class DataCenterModule : public ModuleImpl
{
	DECLEAR_SERVICE_MAP(DataCenterModule)
	DECLEAR_EVENT_MAP(DataCenterModule)

public:
	DataCenterModule();
	~DataCenterModule();

	//----------------------------------------------------------------------------------------
	//����: GetModuleName
	//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	const wchar_t* GetModuleName();

 	//----------------------------------------------------------------------------------------
	//����: GetModuleId
	//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
	//����: 
	//		���ظ�ģ���Ψһ��ID
	//----------------------------------------------------------------------------------------
	uint32 const GetModuleId();

 	//----------------------------------------------------------------------------------------
	//����: ProcessEvent
	//����: ��ģ����Դ�������еĵ�Ե��¼�
	//����: 
	//		@param	evt			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	void ProcessEvent(const Event& evt);

 	//----------------------------------------------------------------------------------------
	//����: ProcessMessage
	//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
	//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
	//����: 
	//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
	//----------------------------------------------------------------------------------------
	void ProcessMessage(const Message& msg);

 	//----------------------------------------------------------------------------------------
	//����: CallDirect
	//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
	//			����������Ҫͨ�����ߡ�
	//����: 
	//		@param	lparam			����1
	//		@param	rparam			����2
	//----------------------------------------------------------------------------------------
	int32 CallDirect(const ServiceValue lServiceValue, param wparam);

 	//----------------------------------------------------------------------------------------
	//����: PaybackExtraInfo
	//����: ĳ��ģ��������Զ���ĸ��ӵ�������Ҫ���������ģ�飬��ô�����Թ���һ��ExtraInfo�ṹ
	//	�������ģ��ʹ����Ϻ󣬸ýṹ���뱻�ͷţ����������ڴ�й¶���ͷű�����ģ��������ɡ�
	//	ĳ��ģ�鶼�����ṩһ��PaybackExtraInfo�ӿڣ��ͷ��Լ����Զ������������
	//����: 
	//		@param	valudId			��Ӧ��pExtraInfo��ֵ���ڲ����ݸ�ֵ���ж�Ӧ���ͷţ���ֵֻ��ģ���Լ����
	//		@param	pExtraInfo	��Ҫ�ͷŵ�ExtraInfo����
	//----------------------------------------------------------------------------------------
	void PaybackExtraInfo(uint32 valudId, void* pExtraInfo);

protected:

	void	OnService_GetFavoriteVector(ServiceValue lServiceValue, param	lParam);
	void	OnService_GetFavoriteData(ServiceValue lServiceValue, param	lParam);


protected:
	
	//	���Ա������е��ղؼ�����
	std::vector<FAVORITELINEDATA>	m_vFavoriteLineData;

	//	���Ա������е���ʷ����
	std::vector<HISTORYLINEDATA>	m_vHistoryLineData;


};

class DataCenterModuleFactory : public ModuleFactoryImpl<DataCenterModule>{};

namespace datacenter
{
	extern DataCenterModule*	g_DataCenterModule;
	extern DataCenterModuleFactory*	g_DataCenterModuleFactory;
}