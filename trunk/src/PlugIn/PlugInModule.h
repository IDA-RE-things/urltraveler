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

class FavoriateNode;

class PlugInModule : public ModuleImpl
{
	DECLEAR_EVENT_MAP(PlugInModule)

public:
	PlugInModule();
	~PlugInModule();

	//----------------------------------------------------------------------------------------
	//����: Unload
	//����: ������ͨ���÷�����ģ�����ж��
	//����: 
	//		���ж�سɹ�������TRUE�����򷵻�FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Unload();

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
	int32 CallDirect(const param lparam, param wparam);

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

	// Event������
protected:

	void	OnEvent_LoadAllPlugin(Event* pEvent);
	void	OnEvent_LoadAllFavorite(Event* pEvent);

private:

	// �洢����ģ���е�IPlugInָ��
	std::vector<PLUGININFO>		m_vPlugInModuleInfo;

	FavoriateNode*		m_pRootFavoriateNode;			//	�ղؼ����ĸ����	
};

class CPlugInModuleFactory : public ModuleFactoryImpl<PlugInModule>{};

namespace plugin
{
	extern PlugInModule*	g_PlugInModule;
	extern CPlugInModuleFactory*	g_PlugInModuleFactory;
}
using namespace plugin;