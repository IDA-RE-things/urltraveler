#include "stdafx.h"
#include "PlugInModule.h"
#include "PlugInDefine.h"
#include "PathHelper.h"
#include "PlugInList.h"
#include "PlugIn.h"
#include <algorithm>
#include <objbase.h>
#include "FavMonitorDefine.h"
#include "XString.h"

HMODULE	 g_hModule;

using namespace plugin;
using namespace favmonitor;

namespace plugin
{
	PlugInModule*	g_PlugInModule = NULL;
	PlugInModuleFactory*	g_PlugInModuleFactory = NULL;
}


EXPORT_GETMODULEFACTORY(PlugInModule)
EXPORT_RELEASEMODULEFACTORY(PlugInModule)


PlugInModule::PlugInModule()
{
	m_eThreadToDoing = NONE;

	m_pExportFavThread = NULL;
	m_pImportFavThread = NULL;

	m_nSumFavorite = 0;

	m_vPlugInFactory.clear();
	m_vPlugIns.clear();
}

PlugInModule::~PlugInModule()
{
	if (m_pExportFavThread)
	{
		m_pExportFavThread->ShutdownThread(0);
		m_pExportFavThread->Release();
	}

	if (m_pImportFavThread)
	{
		m_pImportFavThread->ShutdownThread(0);
		m_pImportFavThread->Release();
	}

}


BEGIN_EVENT_MAP(PlugInModule)
	ON_EVENT(EVENT_VALUE_PLUGIN_LOAD_ALL, OnEvent_LoadAllPlugin)
	ON_EVENT(EVENT_VALUE_PLUGIN_CHECK_IS_WORKED, OnEvent_CheckPlugInWorked)
	ON_EVENT(EVENT_VALUE_PLUGIN_BEGIN_TO_SYNC, OnEvent_BeginToSync)
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

	m_vPlugInFactory.clear();
	m_vPlugIns.clear();
	m_vPlugInModuleInfo.clear();

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

	PlugIn_InExportEndMessage* pMessage = new PlugIn_InExportEndMessage();
	STRNCPY(pMessage->szProcessText, L"���ڼ������е���������");
	GetModuleManager()->PushMessage(*pMessage);
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
		MakeMessage<MODULE_ID_PLUGIN>()(MESSAGE_VALUE_PLUGIN_LOAD_ALL_PLUGIN_FINISHED));

	PlugIn_InExportEndMessage* pMessage = new PlugIn_InExportEndMessage();
	STRNCPY(pMessage->szProcessText, L"���ڼ�����Ƿ�����������");
	GetModuleManager()->PushMessage(*pMessage);

	// �κ�ʱ��ֻ����һ���ֳ���ִ��
	m_ThreadMutex.Lock();
	m_eThreadToDoing = EXPORT;

	if( m_pExportFavThread != NULL)
	{
		m_pExportFavThread->ShutdownThread();
		m_pExportFavThread->Release();
	}

	m_pExportFavThread = CreateThreadObject();
	m_pExportFavThread->CreateThread(static_cast<IThreadEvent *>(this));
	m_ThreadMutex.Unlock();
}

void	PlugInModule::OnEvent_BeginToSync(Event* pEvent)
{
	m_ThreadMutex.Lock();
	m_eThreadToDoing = IMPORT;

	if (m_pImportFavThread)
	{
		m_pImportFavThread->ShutdownThread(0);
		m_pImportFavThread->Release();
	}

	m_pImportFavThread = CreateThreadObject();
	m_pImportFavThread->CreateThread(static_cast<IThreadEvent *>(this));
	m_ThreadMutex.Unlock();
}

void	PlugInModule::OnService_GetAvailablePlugIns(ServiceValue lServiceValue, param	lParam)
{
	PlugIn_GetAvailablePlugInsService* pService = (PlugIn_GetAvailablePlugInsService*)lParam;
	ASSERT(pService != NULL);
	ASSERT(pService->serviceId == SERVICE_VALUE_PLUGIN_GET_AVAILABLE_PLUGIN);

	pService->pvPlugIns = &m_vPlugIns;
}

bool compare_hashid(FAVORITELINEDATA*& a1,FAVORITELINEDATA*& a2)
{
	return a1->nHashId < a2->nHashId;
}

void PlugInModule::Merge(PFAVORITELINEDATA* ppData, int32 nLen, int nParentId)
{
	PlugIn_InExportEndMessage* pMessage = new PlugIn_InExportEndMessage();
	STRNCPY(pMessage->szProcessText, L"���ڶ��ղؼ����ݽ��кϲ�");
	GetModuleManager()->PushMessage(*pMessage);

	int nHash = 0;
	vector<PFAVORITELINEDATA> vec;

	//��������ͬ�����Ľڵ�ŵ�vec��
	for (int i = 0; i < nLen; i++)
	{
		if( ppData[i] == NULL || ppData[i]->szTitle == L"" || ppData[i]->szUrl == L"" || ppData[i]->nId ==0 )
			continue;

		if (ppData[i]->nPid == nParentId )
		{
			vec.push_back(ppData[i]);
		}
	}

	int vSize = vec.size();

	//���ɨ������Ľ��������1��merge����, ��������Ҫ�����ڵ�ָ��ͬһ�������
	if (vSize > 1)
	{
		//��vec��hashid��������, ��Ҫ��������һ�α��������ҳ�������ͬԪ��
		sort(vec.begin(), vec.end(), compare_hashid);

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
					if( ppData[m] == NULL)
						continue;

					if (ppData[m]->nPid == vec[i]->nId)
					{
						ppData[m]->nPid = vec[i + 1]->nId;
					}
				}

				Merge(ppData, nLen, vec[i + 1]->nId);
			}
		}
	}
}

void PlugInModule::OnThreadEntry()
{

}

bool CompareFavoriteData (const FAVORITELINEDATA* i,  const FAVORITELINEDATA* j) 
{ 
	return i->nPid < j->nPid;
}


void	PlugInModule::DoExportThread()
{
	PlugIn_InExportEndMessage* pMessage = new PlugIn_InExportEndMessage();
	STRNCPY(pMessage->szProcessText, L"���ڵ���������������ղؼ�����");
	GetModuleManager()->PushMessage(*pMessage);

	DataCenter_GetFavoriteVectorService favoriteVectorService;
	m_pModuleManager->CallService(SERVICE_VALUE_DATACENTER_GET_FAVORITE_VECTOR, (param)&favoriteVectorService);
	std::vector<FAVORITELINEDATA*>*	pvFavoriteData = favoriteVectorService.pvFavoriteData;

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

		// ���ò����·��������·��
/*
		wchar_t* pFavDataPath = pPlugIn->GetFavoriteDataPath();
		if( pvFavoriteData != NULL)
		{
			FavMonitor_AddMonitorService service;
			STRNCPY(service.szFile, pFavDataPath);
			g_PlugInModule->GetModuleManager()->CallService(service.serviceId, (param)&service);
		}
*/

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
		// ���͹㲥��֪ͨ�ղؼ��Ѿ��ϲ����
		m_pModuleManager->PushMessage(
			MakeMessage<MODULE_ID_PLUGIN>()(MESSAGE_VALUE_PLUGIN_EXPORT_ALL_FINISHED));
		return;
	}

	DataCenter_InitFavoriteDataService initService;
	initService.nNum = m_nSumFavorite;
	GetModuleManager()->CallService(initService.serviceId, (param)&initService);

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

		// ֪ͨ��ʼ�����ղؼ����ݵ���
		PlugIn_ExportBeginMessage* pExportBeginMessage = new PlugIn_ExportBeginMessage();
		pExportBeginMessage->pPlugIn = pPlugIn;
		GetModuleManager()->PushMessage(*pExportBeginMessage);

		BOOL bRet = pPlugIn->ExportFavoriteData(&(*pvFavoriteData)[nCurrentOffset], panCount[i]);
		if( bRet == FALSE)
		{
			PlugIn_ExportEndMessage* pExportEndMessage = new PlugIn_ExportEndMessage();
			pExportEndMessage->bSuccess = FALSE;
			pExportEndMessage->pPlugIn = pPlugIn;
			pExportEndMessage->nFavoriteNum = panCount[i];
			GetModuleManager()->PushMessage(*pExportEndMessage);

			continue;
		}

		nCurrentOffset += panCount[i];

		PlugIn_ExportEndMessage* pExportEndMessage = new PlugIn_ExportEndMessage();
		pExportEndMessage->pPlugIn = pPlugIn;
		pExportEndMessage->nFavoriteNum = panCount[i];
		GetModuleManager()->PushMessage(*pExportEndMessage);
	}

	Merge(&(*pvFavoriteData)[0], m_nSumFavorite, 0);

	std::vector<FAVORITELINEDATA*>::iterator itr = pvFavoriteData->begin();
	for( ; itr !=  pvFavoriteData->end();)
	{
		if(*itr == NULL)
			continue;

		if( (*itr)->bDelete == true || String((*itr)->szTitle) == L"")
			itr =  pvFavoriteData->erase(itr);
		else
			itr++;
	}
	m_nSumFavorite =  pvFavoriteData->size();

	if( m_nSumFavorite == 0)
	{
		// ���ϲ�������ݵ��뵽�����������
		if (panCount)
		{
			delete []panCount;
			panCount = NULL;
		}

		// ���͹㲥��֪ͨ�ղؼ��Ѿ��ϲ����
		m_pModuleManager->PushMessage(
			MakeMessage<MODULE_ID_PLUGIN>()(MESSAGE_VALUE_PLUGIN_EXPORT_ALL_FINISHED));

		return;
	}

	// ���й�ȱ�������
	sort(pvFavoriteData->begin(), pvFavoriteData->end(), CompareFavoriteData);

	DataCenter_ReArrangeFavoriteService service;
	GetModuleManager()->CallService(service.serviceId, (param)&service);
	
	sort(pvFavoriteData->begin(), pvFavoriteData->end(), CompareFavoriteData);

	// ���ϲ�������ݵ��뵽�����������
	if (panCount)
	{
		delete []panCount;
		panCount = NULL;
	}

	// ���͹㲥��֪ͨ�ղؼ��Ѿ��ϲ����
	m_pModuleManager->PushMessage(
		MakeMessage<MODULE_ID_PLUGIN>()(MESSAGE_VALUE_PLUGIN_EXPORT_ALL_FINISHED));
}

void	PlugInModule::DoImportThread()
{
	GetModuleManager()->PushMessage(MakeMessage<MODULE_ID_PLUGIN>()(MESSAGE_VALUE_PLUGIN_IMPORT_PRE_BEGIN));
	
	DataCenter_GetFavoriteVectorService favoriteVectorService;
	m_pModuleManager->CallService(SERVICE_VALUE_DATACENTER_GET_FAVORITE_VECTOR, (param)&favoriteVectorService);
	std::vector<FAVORITELINEDATA*>*	pvFavoriteData = favoriteVectorService.pvFavoriteData;
	if( pvFavoriteData == NULL)
		return;

	if( pvFavoriteData->size() == 0)
		return;

	// �Ƚ�������
	DataCenter_ReArrangeFavoriteService service;
	GetModuleManager()->CallService(service.serviceId, (param)&service);

	for( int i=0; i<m_vPlugIns.size(); i++)
	{
		IPlugIn* pPlugIn = m_vPlugIns.at(i);
		if( pPlugIn == NULL)
			continue;

		// ֪ͨ��ʼ�����ղؼ����ݵ���
		PlugIn_ImportBeginMessage* pImportBeginMessage = new PlugIn_ImportBeginMessage();
		pImportBeginMessage->pPlugIn = pPlugIn;
		GetModuleManager()->PushMessage(*pImportBeginMessage);

		int nImportFavNum = pvFavoriteData->size();
		int nRet = pPlugIn->ImportFavoriteData(&(*pvFavoriteData)[0], nImportFavNum);

		PlugIn_ImportEndMessage* pImportEndMessage = new PlugIn_ImportEndMessage();
		pImportEndMessage->pPlugIn = pPlugIn;
		pImportEndMessage->nFavoriteNum = nImportFavNum;
		pImportEndMessage->nErrorCode = nRet;
		GetModuleManager()->PushMessage(*pImportEndMessage);
	}

	GetModuleManager()->PushMessage(
		MakeMessage<MODULE_ID_PLUGIN>()(MESSAGE_VALUE_PLUGIN_IMPORT_ALL_FINISHED));
}

int PlugInModule::Run()
{
	if( m_eThreadToDoing == EXPORT)
	{
		DoExportThread();
		m_eThreadToDoing = NONE;
		return 0;
	}

	if( m_eThreadToDoing == IMPORT)
	{
		DoImportThread();
		m_eThreadToDoing = NONE;
		return 0;
	}

	return 0;
}

void PlugInModule::OnThreadExit()
{
}