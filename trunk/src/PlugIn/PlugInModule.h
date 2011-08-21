#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "PlugIn.h"
#include <vector>
#include "ThreadObject_i.h"
#include "Guard.h"
#include "PlugInDefine.h"
#include "XSync.h"


using namespace std;
using namespace plugin;

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class PlugInModule : public ModuleImpl,
					public IThreadEvent
{
	DECLEAR_EVENT_MAP(PlugInModule)
	DECLEAR_SERVICE_MAP(PlugInModule)

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

public:
	//! �����߳��麯��
	virtual void OnThreadEntry();

	virtual int Run();

	//! �߳��˳��麯��
	virtual void OnThreadExit();

	// Event������
protected:

	void	OnEvent_LoadAllPlugin(Event* pEvent);
	void	OnEvent_CheckPlugInWorked(Event* pEvent);
	void	OnEvent_BeginToSync(Event* pEvent);

protected:

	void	OnService_GetAvailablePlugIns(ServiceValue lServiceValue, param	lParam);

private:

	//	��pFavoriteData���кϲ�
	void	Merge(PFAVORITELINEDATA* ppData, int32 nLen, int nParentId);

	void	DoExportThread();
	void	DoImportThread();

private:

	int                 m_nSumFavorite;

	//	�������еĲ��ģ��ָ��
	std::vector<IPlugInFactory*>	m_vPlugInFactory;

	// �洢����ģ���е�IPlugInָ��
	std::vector<PLUGININFO>		m_vPlugInModuleInfo;

	//	�������еĲ��
	std::vector<IPlugIn*>		m_vPlugIns;	

	enum EThreadDoing
	{
		NONE,
		EXPORT,
		IMPORT,
	};

	// ��Ҫִ����һ���ֳ�
	CXMutex	m_ThreadMutex;
	EThreadDoing	m_eThreadToDoing;

	IThreadObject*      m_pExportFavThread;
	IThreadObject*      m_pImportFavThread;
};

class PlugInModuleFactory : public ModuleFactoryImpl<PlugInModule>{};

namespace plugin
{
	extern PlugInModule*	g_PlugInModule;
	extern PlugInModuleFactory*	g_PlugInModuleFactory;
}
using namespace plugin;