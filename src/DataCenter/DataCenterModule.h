#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "PlugIn.h"
#include <vector>
#include <map>

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

protected:

	void	OnEvent_FavoriteIconArrive(Event* pEvent);		//	��ַ��Ӧ��ICONͼ��ﵽ


protected:

	void	OnService_GetFavoriteVector(ServiceValue lServiceValue, param	lParam);
	void	OnService_GetFavoriteData(ServiceValue lServiceValue, param	lParam);
	void	OnService_GetFavoriteIcon(ServiceValue lServiceValue, param lParam);


protected:
	
	//	���Ա������е��ղؼ�����
	std::vector<FAVORITELINEDATA>	m_vFavoriteLineData;

	//	���Ա������е���ʷ����
	std::vector<HISTORYLINEDATA>	m_vHistoryLineData;

	//	����Domain���Ӧ���ղؼ�ͼ��֮��Ĺ�ϵ
	std::map<wstring, HICON>	m_mapDomain2Icon;
	HICON                       m_hDefaultIcon;
};

class DataCenterModuleFactory : public ModuleFactoryImpl<DataCenterModule>{};

namespace datacenter
{
	extern DataCenterModule*	g_DataCenterModule;
	extern DataCenterModuleFactory*	g_DataCenterModuleFactory;
}