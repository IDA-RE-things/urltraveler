#include "stdafx.h"
#include "PlugInModule.h"
#include "PlugInDefine.h"
#include "PathHelper.h"
#include "PlugInList.h"
#include "PlugIn.h"
#include <algorithm>
#include <objbase.h>

HMODULE	 g_hModule;

using namespace plugin;

namespace plugin
{
	PlugInModule*	g_PlugInModule = NULL;
	PlugInModuleFactory*	g_PlugInModuleFactory = NULL;
}


EXPORT_GETMODULEFACTORY(PlugInModule)
EXPORT_RELEASEMODULEFACTORY(PlugInModule)


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
	ON_EVENT(EVENT_VALUE_PLUGIN_CHECK_IS_WORKED, OnEvent_CheckPlugInWorked)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(PlugInModule)
	ON_SERVICE(SERVICE_VALUE_PLUGIN_GET_AVAILABLE_PLUGIN, OnService_GetAvailablePlugIns)
END_SERVICE_MAP()
//----------------------------------------------------------------------------------------
//����: Unload
//����: ������ͨ���÷�����ģ�����ж��
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL PlugInModule::Unload() 
{
	std::vector<IPlugIn*>::iterator itrPlugIn = m_vPlugIns.begin();
	std::vector<IPlugIn*>::const_iterator itrConPlugIn = m_vPlugIns.end();

	for( ; itrPlugIn != itrConPlugIn; ++itrPlugIn)
	{
		(*itrPlugIn)->UnLoad();
	}

	std::vector<PLUGININFO>::iterator itr = m_vPlugInModuleInfo.begin();

	for( ; itr != m_vPlugInModuleInfo.end(); itr++)
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
int32 PlugInModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
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

		if( pGetPlugInFactoryFunc == NULL)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" DLL���Ҳ���GetPlugInFactory����ʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue;
		}

		if(pReleasePlugInFactoryFunc == NULL)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" DLL���Ҳ���ReleasePlugInFactory����ʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue;
		}

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
			wstrModuleName.append(L" QueryPlugInCounter��������ʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue ;
		}

		stPlugInInfo.pvPlugIn.resize(nPlugNum);

		// ���������еĲ��������std::vector<IPlugIn*>��
		bRet = pPlugInFactory->QueryPlugInPoint(nPlugNum, stPlugInInfo.pvPlugIn[0]);
		if( bRet == FALSE)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" QueryPlugInPoint��������ʧ��");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn����ʧ��", MB_OK);
			continue ;
		}

		// �Բ�����м��ز���
		for(int i=0; i<nPlugNum; i++)
		{
			if (stPlugInInfo.pvPlugIn[i]->Load())
			{
				m_vPlugIns.push_back(stPlugInInfo.pvPlugIn[i]);
			}
		}

		m_vPlugInModuleInfo.push_back(stPlugInInfo);
	}

	// ���ø���ģ���IsWorked��������鵱ǰ����Ƿ���Ҫ����
	m_pModuleManager->PushEvent(
		MakeEvent<MODULE_ID_PLUGIN>()(EVENT_VALUE_PLUGIN_CHECK_IS_WORKED,
		MODULE_ID_PLUGIN));
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

	// ���͹㲥��Ϣ��֪ͨ���еĲ���Ѿ����ز�������
	m_pModuleManager->PushMessage(
		MakeMessage<MODULE_ID_PLUGIN>()(MESSAGE_VALUE_PLUGIN_LOAD_ALL_FINISHED));

	m_pThreadObj->CreateThread(static_cast<IThreadEvent *>(this));
}

void	PlugInModule::OnService_GetAvailablePlugIns(ServiceValue lServiceValue, param	lParam)
{
	PlugIn_GetAvailablePlugInsService* pService = (PlugIn_GetAvailablePlugInsService*)lParam;
	ASSERT(pService != NULL);
	ASSERT(pService->serviceId == SERVICE_VALUE_PLUGIN_GET_AVAILABLE_PLUGIN);

	pService->pvPlugIns = &m_vPlugIns;
}

bool compare(FAVORITELINEDATA*& a1,FAVORITELINEDATA*& a2)
{
	return a1->nHashId < a2->nHashId;
}

void PlugInModule::SortByBreadth(PFAVORITELINEDATA pFavoriteLineData, int nNum)
{
	// ���ȶ�pFavoriteLineData��������
	FAVORITELINEDATA*	pSortLineData = new FAVORITELINEDATA[nNum];
	memset(pSortLineData, 0x0, sizeof(FAVORITELINEDATA) * nNum);

	FAVORITELINEDATA*	pSortLineDataPos = pSortLineData;

	// ��һ�ҵ����ʵ����ݣ������뵽pSortLineData��ȥ
	int nParentId	=	0;
	for( int i=0; i<nNum; i++)
	{
		// ��δ����������в���ParentΪnParentId������
		for(int j=0; j<nNum; j++)
		{
			if( pFavoriteLineData[j].nPid	==	nParentId)
			{
				memcpy(pSortLineDataPos, &pFavoriteLineData[j], sizeof(FAVORITELINEDATA));
				pSortLineDataPos++;
			}
		}

		nParentId	=	pSortLineData[i].nId;
	}

	// ���������ݿ���
	memcpy(pFavoriteLineData, pSortLineData, nNum*sizeof( FAVORITELINEDATA));
	delete[] pSortLineData;
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

void PlugInModule::ReArrange(PFAVORITELINEDATA pData, int nLen)
{
	//�ʱ�临�Ӷ�O(N^2)
	for (int i = 0; i < nLen; i++)
	{
		//����ý���nId���������±�+1,����Ҫ����
		if ((pData[i].nId != i + 1))
		{
			//ɨ�������Ըý��Ϊ�����Ľ�㣬��������Щ����nPid
			for (int j = 0; j < nLen; j++)
			{
				if (pData[j].nPid == pData[i].nId)
				{
					pData[j].nPid = i + 1;
				}
			}

			pData[i].nId = i + 1;
		}
	}
}

void PlugInModule::OnThreadEntry()
{

}

int PlugInModule::Run()
{
	DataCenter_GetFavoriteVectorService favoriteVectorService;
	m_pModuleManager->CallService(SERVICE_VALUE_DATACENTER_GET_FAVORITE_VECTOR, (param)&favoriteVectorService);
	std::vector<FAVORITELINEDATA>*	pvFavoriteData = favoriteVectorService.pvFavoriteData;

	CoInitialize(NULL);

	int nNumOfPlugIns = m_vPlugIns.size();
	int *panCount = new int[nNumOfPlugIns];

	pvFavoriteData->clear();

	ZeroMemory(panCount, sizeof(int) * nNumOfPlugIns);
	m_nSumFavorite = 0;

	for (int i = 0; i < nNumOfPlugIns; i++)
	{
		IPlugIn*	pPlugIn = m_vPlugIns.at(i);
		if( pPlugIn == NULL)
			continue;

		int nFavoriteCount = pPlugIn->GetFavoriteCount();
		panCount[i] = nFavoriteCount;

		if( nFavoriteCount == 0)
		{
			continue;
		}

		m_nSumFavorite += nFavoriteCount;
	}

	if (m_nSumFavorite == 0)
	{
		return 0;
	}

	pvFavoriteData->resize(m_nSumFavorite);

	int nCurrentOffset = 0;

	// �����е���������ݽ��кϲ�
	for(int i = 0; i < nNumOfPlugIns; i++)
	{
		IPlugIn*	pPlugIn = m_vPlugIns.at(i);
		if( pPlugIn == NULL)
			continue;

		int nFavoriteCount = panCount[i];
		if( nFavoriteCount == 0)
			continue;

		pPlugIn->ExportFavoriteData(&(*pvFavoriteData)[nCurrentOffset], panCount[i]);
		nCurrentOffset += panCount[i];
	}

	Merge(&(*pvFavoriteData)[0], m_nSumFavorite, 0);

	std::vector<FAVORITELINEDATA>::iterator itr = pvFavoriteData->begin();
	for( ; itr !=  pvFavoriteData->end();)
	{
		if( (*itr).bDelete == true)
			itr =  pvFavoriteData->erase(itr);
		else
			itr++;
	}
	m_nSumFavorite =  pvFavoriteData->size();

	// ���й�ȱ�������
	SortByBreadth(&(*pvFavoriteData)[0], m_nSumFavorite);

	ReArrange(&(*pvFavoriteData)[0], m_nSumFavorite);
	
	// ���ϲ�������ݵ��뵽�����������
	if (panCount)
	{
		delete []panCount;
		panCount = NULL;
	}

	// ���͹㲥��֪ͨ�ղؼ��Ѿ��ϲ����
	m_pModuleManager->PushMessage(
		MakeMessage<MODULE_ID_PLUGIN>()(MESSAGE_VALUE_PLUGIN_LOAD_FAVORITE_DATA_FINISHED));

	/*
	for( int i=0; i<nNumOfPlugIns; i++)
	{
		IPlugIn* pPlugIn = m_vPlugIns.at(i);
		if( pPlugIn == NULL)
			continue;

		pPlugIn->ImportFavoriteData(&m_vFavoriateLineData[0], m_nSumFavorite);
	}
	*/

	//ʹ�߳�ֱ���˵�����0��������Ҫ�Լ�ȥShutdown
	return 0;
}

void PlugInModule::OnThreadExit()
{
}

