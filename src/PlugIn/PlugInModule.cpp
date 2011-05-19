#include "stdafx.h"
#include "PlugInModule.h"
#include "PlugInDefine.h"
#include "PathHelper.h"
#include "PlugInList.h"
#include "PlugIn.h"
#include <algorithm>

HMODULE	 g_hModule;

using namespace plugin;

namespace plugin
{
	PlugInModule*	g_PlugInModule = NULL;
	PlugInModuleFactory*	g_PlugInModuleFactory = NULL;
}

// �������ʵ��
IModuleFactory*	GetModuleFactory()
{
	if( g_PlugInModuleFactory == NULL)
	{
		g_PlugInModuleFactory = new PlugInModuleFactory();
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
	m_pThreadObj = CreateThreadObject();
	m_nSumFavorite = 0;

	m_vPlugInFactory.clear();
	m_vPlugIns.clear();
}

PlugInModule::~PlugInModule()
{
	if (m_pThreadObj)
	{
		m_pThreadObj->ShutdownThread(0);
		m_pThreadObj->Release();
	}
}


BEGIN_EVENT_MAP(PlugInModule)
	ON_EVENT(EVENT_VALUE_PLUGIN_LOAD_ALL, OnEvent_LoadAllPlugin)
	ON_EVENT(EVENT_VALUE_PLUGIN_COMBINE_FAVORITE, OnEvent_LoadAllFavorite)
	ON_EVENT(EVENT_VALUE_PLUGIN_CHECK_IS_WORKED, OnEvent_CheckPlugInWorked)
END_EVENT_MAP()

//----------------------------------------------------------------------------------------
//����: Unload
//����: ������ͨ���÷�����ģ�����ж��
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL PlugInModule::Unload() 
{
	std::vector<PLUGININFO>::iterator itr = m_vPlugInModuleInfo.begin();

	for( ; itr != m_vPlugInModuleInfo.end(); )
	{
		PLUGININFO* pPlugInfo = &*itr;
		if( pPlugInfo != NULL)
		{
			pPlugInfo->pReleasePlugInFactoryFunc();
		}
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

		HMODULE exportplugin = LoadLibrary(strPlugInPath.c_str());
		ASSERT(exportplugin != NULL);
		if(exportplugin==NULL)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" LoadLibraryʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue ;
		}

		stPlugInInfo.hModule = exportplugin;

		GetPlugInFactoryFunc pGetPlugInFactoryFunc 
			=  reinterpret_cast<GetPlugInFactoryFunc >(GetProcAddress(exportplugin, "GetPlugInFactory"));
		ReleasePlugInFactoryFunc pReleasePlugInFactoryFunc 
			= reinterpret_cast<ReleasePlugInFactoryFunc>(GetProcAddress(exportplugin, "ReleasePlugInFactory")); 

		stPlugInInfo.pGetPlugInFactoryFunc = pGetPlugInFactoryFunc;
		stPlugInInfo.pReleasePlugInFactoryFunc = pReleasePlugInFactoryFunc;

		IPlugInFactory *pPlugInFactory = pGetPlugInFactoryFunc();
		if(pPlugInFactory == NULL)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" �Ҳ���GetPlugInFactory����ʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue ;
		}

		m_vPlugInFactory.push_back(pPlugInFactory);

		// ����Factory��Query���������ҵ�ǰ��Factory��֧�ֵ����е�PlugIn
		uint32 nPlugNum = 0;
		BOOL bRet = pPlugInFactory->QueryPlugInCounter(nPlugNum);
		if( bRet == FALSE)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" �Ҳ���QueryPlugInCounter����ʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue ;
		}

		stPlugInInfo.pvPlugIn.resize(nPlugNum);


		// ���������еĲ��������std::vector<IPlugIn*>��
		bRet = pPlugInFactory->QueryPlugInPoint(nPlugNum, stPlugInInfo.pvPlugIn[0]);
		if( bRet == FALSE)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" �Ҳ���QueryPlugInPoint����ʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue ;
		}

		// �Բ�����м��ز���
		for(int i=0; i<nPlugNum; i++)
		{
			stPlugInInfo.pvPlugIn[i]->Load();
			m_vPlugIns.push_back(stPlugInInfo.pvPlugIn[i]);
		}

		m_vPlugInModuleInfo.push_back(stPlugInInfo);
	}


	// ���ø���ģ���IsWorked��������鵱ǰ����Ƿ���Ҫ����
	m_pModuleManager->PushEvent(
		MakeEvent<MODULE_ID_PLUGIN>()(EVENT_VALUE_PLUGIN_CHECK_IS_WORKED,
		MODULE_ID_PLUGIN));

	m_pThreadObj->CreateThread(static_cast<IThreadEvent *>(this));
}

void PlugInModule::OnEvent_CheckPlugInWorked(Event* pEvent)
{
	std::vector<IPlugIn*>::iterator itrPlugIn = m_vPlugIns.begin();
	for( ; itrPlugIn != m_vPlugIns.end(); )
	{
		// �����ǰ������ܹ���������ɾ��
		if( (*itrPlugIn)->IsWorked() == FALSE)
		{
			itrPlugIn = m_vPlugIns.erase(itrPlugIn);
		}
		else
		{
			itrPlugIn++;
		}
	}
}

bool compare(FAVORITELINEDATA*& a1,FAVORITELINEDATA*& a2)
{
	return a1->nHashId < a2->nHashId;
}

void PlugInModule::Merge(PFAVORITELINEDATA pData, int32 nLen, int nParentId)
{
	int nHash = 0;
	vector<PFAVORITELINEDATA> vec;

	//��������ͬ�����Ľڵ�ŵ�vec��
	for (int i = 0; i < nLen; i++)
	{
		if (pData[i].nPid == nParentId)
		{
			vec.push_back(&pData[i]);
		}
	}

	int vSize = vec.size();

	//���ɨ������Ľ��������1��merge����, ��������Ҫ�����ڵ�ָ��ͬһ�������
	if (vSize > 1)
	{
		//��vec��hashid��������, ��Ҫ��������һ�α��������ҳ�������ͬԪ��
		sort(vec.begin(), vec.end(), compare);

		for (int i = 0; i < vSize - 1; i++)
		{
			//ͬһ�����ڵ��������Hash��ͬ�������㣬�����������Ҫ�ϲ�
			if (vec[i]->nHashId == vec[i + 1]->nHashId)
			{
				//������ɾ�����, ���ɾ��
				vec[i]->bDelete = true;
				//�����������и��ڵ�Ϊj�Ľڵ��nPid, ���ϲ�
				for (int m = 0; m < nLen; m++)
				{
					if (pData[m].nPid == vec[i]->nId)
					{
						pData[m].nPid = vec[i + 1]->nId;
					}
				}

				Merge(pData, nLen, vec[i + 1]->nId);
			}
		}
	}
}

// ֪ͨ���غϲ����е��ղؼ�����
void	PlugInModule::OnEvent_LoadAllFavorite(Event* pEvent)
{
		int nNumOfPlugIns = m_vPlugIns.size();
		int *panOffset = new int[nNumOfPlugIns + 1];
	
		m_vFavoriateLineData.clear();
	
		ZeroMemory(panOffset, sizeof(int) * (nNumOfPlugIns + 1));
		m_nSumFavorite = 0;
	
		for (int i = 0; i < nNumOfPlugIns; i++)
		{
			IPlugIn*	pPlugIn = m_vPlugIns.at(i);
			if( pPlugIn == NULL)
				continue;
	
			int nFavoriteCount = pPlugIn->GetFavoriteCount();
			if( nFavoriteCount == 0)
				continue;

			panOffset[i + 1] = nFavoriteCount;
			m_nSumFavorite += nFavoriteCount;
		}
	
		m_vFavoriateLineData.resize(m_nSumFavorite);
	
		// �����е���������ݽ��кϲ�
		for(int i=0; i < nNumOfPlugIns; i++)
		{
			IPlugIn*	pPlugIn = m_vPlugIns.at(i);
			if( pPlugIn == NULL)
				continue;
	
	
			int nFavoriteCount = pPlugIn->GetFavoriteCount();
			if( nFavoriteCount == 0)
				continue;
	
			pPlugIn->ExportFavoriteData(&m_vFavoriateLineData[panOffset[i]], panOffset[i + 1]);
		}
	
	
		Merge(&m_vFavoriateLineData[0], m_nSumFavorite, 0);
		
		// ���ϲ�������ݵ��뵽�����������
		if (panOffset)
		{
			delete []panOffset;
			panOffset = NULL;
		}
	
}

void PlugInModule::OnThreadEntry()
{

}

int PlugInModule::Run()
{

	int nNumOfPlugIns = m_vPlugIns.size();
	int *panOffset = new int[nNumOfPlugIns + 1];

	m_vFavoriateLineData.clear();

	ZeroMemory(panOffset, sizeof(int) * (nNumOfPlugIns + 1));
	m_nSumFavorite = 0;

	for (int i = 0; i < nNumOfPlugIns; i++)
	{
		IPlugIn*	pPlugIn = m_vPlugIns.at(i);
		if( pPlugIn == NULL)
			continue;

		int nFavoriteCount = pPlugIn->GetFavoriteCount();
		if( nFavoriteCount == 0)
			continue;

		panOffset[i + 1] = nFavoriteCount;
		m_nSumFavorite += nFavoriteCount;
	}

	m_vFavoriateLineData.resize(m_nSumFavorite);

	// �����е���������ݽ��кϲ�
	for(int i=0; i < nNumOfPlugIns; i++)
	{
		IPlugIn*	pPlugIn = m_vPlugIns.at(i);
		if( pPlugIn == NULL)
			continue;


		int nFavoriteCount = pPlugIn->GetFavoriteCount();
		if( nFavoriteCount == 0)
			continue;

		pPlugIn->ExportFavoriteData(&m_vFavoriateLineData[panOffset[i]], panOffset[i + 1]);
	}


	Merge(&m_vFavoriateLineData[0], m_nSumFavorite, 0);
	
	// ���ϲ�������ݵ��뵽�����������
	if (panOffset)
	{
		delete []panOffset;
		panOffset = NULL;
	}
	
	for( int i=0; i<nNumOfPlugIns; i++)
	{
		IPlugIn* pPlugIn = m_vPlugIns.at(i);
		if( pPlugIn == NULL)
			continue;

		pPlugIn->ImportFavoriteData(&m_vFavoriateLineData[0], m_nSumFavorite);
	}

	//ʹ�߳�ֱ���˵�����0��������Ҫ�Լ�ȥShutdown
	return 0;
}

void PlugInModule::OnThreadExit()
{
}
