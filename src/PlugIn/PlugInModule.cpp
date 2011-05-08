#include "stdafx.h"
#include "PlugInModule.h"
#include "PlugInDefine.h"
#include "PathHelper.h"
#include "PlugInList.h"

HMODULE	 g_hModule;

using namespace plugin;

namespace plugin
{
	PlugInModule*	g_PlugInModule = NULL;
	CPlugInModuleFactory*	g_PlugInModuleFactory = NULL;
}

// �������ʵ��
IModuleFactory*	GetModuleFactory()
{
	if( g_PlugInModuleFactory == NULL)
	{
		g_PlugInModuleFactory = new CPlugInModuleFactory();
		g_PlugInModuleFactory->QueryModulePoint(1, (IModule*&)g_PlugInModule);
		
		ASSERT( g_PlugInModule != NULL);
	}

	return g_PlugInModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_PlugInModuleFactory == p);
	if( g_PlugInModuleFactory  != NULL)
	{
		delete g_PlugInModuleFactory;
		g_PlugInModuleFactory = NULL;
	}
}

PlugInModule::PlugInModule()
{

}

PlugInModule::~PlugInModule()
{

}


BEGIN_EVENT_MAP(PlugInModule)
	ON_EVENT(EVENT_VALUE_PLUGIN_LOAD_ALL, OnEvent_LoadAllPlugin)
END_EVENT_MAP()

//----------------------------------------------------------------------------------------
//����: Unload
//����: ������ͨ���÷�����ģ�����ж��
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL PlugInModule::Unload() 
{
	for(int i=0; i<m_vPlugInModuleInfo.size(); i++)
	{
		PPLUGININFO pPlugInInfo = &m_vPlugInModuleInfo.at(i);
		pPlugInInfo->pReleasePlugInFunc(pPlugInInfo->pPlugIn);
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* PlugInModule::GetModuleName() 
{
	return L"PlugInModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const PlugInModule::GetModuleId()
{
	return MODULE_ID_PLUGIN;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void PlugInModule::ProcessEvent(const Event& evt)
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
void PlugInModule::ProcessMessage(const Message& msg) 
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
int32 PlugInModule::CallDirect(const param lparam, param wparam) 
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
void PlugInModule::PaybackExtraInfo(uint32 valudId, void* pExtraInfo)
{
	return;
}

static PLUGINLIST g_stPlugInInitList;
void	PlugInModule::OnEvent_LoadAllPlugin(Event* pEvent)
{
	std::wstring strPath = PathHelper::GetModuleDir(g_hModule);
	strPath += L"\\PlugIn\\";

	PlugInList::DllPlugInList::iterator itr = g_stPlugInInitList.m_stDllPlugInList.begin();
	for(; itr != g_stPlugInInitList.m_stDllPlugInList.end(); ++itr)
	{
		wstring strPlugInPath = strPath + *itr;

		PLUGININFO stPlugInInfo;
		STRNCPY(stPlugInInfo.wszPlugInName, strPlugInPath.c_str());

		HMODULE exportplugin =LoadLibrary(strPlugInPath.c_str());
		ASSERT(exportplugin != NULL);
		if(exportplugin==NULL)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" LoadLibraryʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue ;
		}

		stPlugInInfo.hModule = exportplugin;

		GetPlugInFunc pGetPlugInFunc 
			=  reinterpret_cast<GetPlugInFunc >(GetProcAddress(exportplugin, "GetPlugIn"));
		ReleasePlugInFunc pReleasePlugInFunc 
			= reinterpret_cast<ReleasePlugInFunc>(GetProcAddress(exportplugin, "ReleasePlugIn")); 

		stPlugInInfo.pGetPlugInFunc = pGetPlugInFunc;
		stPlugInInfo.pReleasePlugInFunc = pReleasePlugInFunc;

		IPlugIn *pPlugIn = pGetPlugInFunc();
		if(pPlugIn == NULL)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" �Ҳ���GetPlugIn����ʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue ;
		}

		stPlugInInfo.pPlugIn = pPlugIn;
		m_vPlugInModuleInfo.push_back(stPlugInInfo);
	}
}