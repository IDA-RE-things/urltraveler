#include "stdafx.h"
#include "PlugInModule.h"
#include "PlugInDefine.h"
#include "PathHelper.h"
#include "PlugInList.h"
#include "PlugIn.h"
#include "FavoriateTree.h"
#include <algorithm>

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
	m_pFavoriateTree	=	NULL;
}

PlugInModule::~PlugInModule()
{

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

		// �Բ�����м��ز���
		pPlugIn->Load();

		stPlugInInfo.pPlugIn = pPlugIn;
		m_vPlugInModuleInfo.push_back(stPlugInInfo);
	}


	// ���ø���ģ���IsWorked��������鵱ǰ����Ƿ���Ҫ����
	m_pModuleManager->PushEvent(
		MakeEvent<MODULE_ID_PLUGIN>()(EVENT_VALUE_PLUGIN_CHECK_IS_WORKED,
		MODULE_ID_PLUGIN));


	// OnEvent_LoadAllFavorite(NULL);
}

void PlugInModule::OnEvent_CheckPlugInWorked(Event* pEvent)
{
	std::vector<PLUGININFO>::iterator itr = m_vPlugInModuleInfo.begin();

	for( ; itr != m_vPlugInModuleInfo.end(); )
	{
		PLUGININFO* pPlugInfo = &*itr;
		if( pPlugInfo != NULL && pPlugInfo->pPlugIn != NULL)
		{
			// �����ǰ������ܹ���������ɾ��
			if( pPlugInfo->pPlugIn->IsWorked() == FALSE)
			{
				itr = m_vPlugInModuleInfo.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}

void PlugInModule::SortFavoriateData(PFAVORITELINEDATA pFavoriteLineData, int nNum)
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

bool compare(FAVORITELINEDATA*& a1,FAVORITELINEDATA*& a2)
{
	return a1->nHashId < a2->nHashId;
}


//����㷨�����Ǽ��ߣ�����Ҫ���±��
void Rearrange(PFAVORITELINEDATA pData, int nLen)
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
	m_pFavoriateTree	=	new FavoriateTree();
	int nNumOfPlugIns = m_vPlugInModuleInfo.size();
	int *panOffset = new int[nNumOfPlugIns + 1];
	int nSum = 0;

	m_vFavoriateLineData.clear();

	ZeroMemory(panOffset, sizeof(int) * (nNumOfPlugIns + 1));

	for (int i = 0; i < nNumOfPlugIns; i++)
	{
		PPLUGININFO	pLogInfo = &m_vPlugInModuleInfo.at(i);

		if( pLogInfo == NULL)
			continue;

		if( pLogInfo->pPlugIn == NULL)
			continue;

		int nFavoriteCount = pLogInfo->pPlugIn->GetFavoriteCount();

		if( nFavoriteCount == 0)
			continue;

		panOffset[i + 1] = nFavoriteCount;

		nSum += nFavoriteCount;

	}

	m_vFavoriateLineData.resize(nSum);

	// �����е���������ݽ��кϲ�
	for(int i=0; i < nNumOfPlugIns; i++)
	{
		PPLUGININFO	pLogInfo = &m_vPlugInModuleInfo.at(i);
		if( pLogInfo == NULL)
			continue;

		if( pLogInfo->pPlugIn == NULL)
			continue;


		int nFavoriteCount = pLogInfo->pPlugIn->GetFavoriteCount();
		if( nFavoriteCount == 0)
			continue;

		pLogInfo->pPlugIn->ExportFavoriteData(&m_vFavoriateLineData[panOffset[i]], panOffset[i + 1]);
	}

	DWORD dwBegin = GetTickCount();

	Merge(&m_vFavoriateLineData[0], nSum, 0);

	wchar_t szInfo[102];

	_stprintf_s(szInfo, 102, L"Time Elapse:%d", GetTickCount() - dwBegin);

	MessageBox(NULL, szInfo, L"OK", MB_OK);


	// ���ϲ�������ݵ��뵽�����������
	for( int i=0; i<nNumOfPlugIns; i++)
	{
		PPLUGININFO	pLogInfo = &m_vPlugInModuleInfo.at(i);
		if( pLogInfo == NULL)
			continue;

		if( pLogInfo->pPlugIn == NULL)
			continue;

		pLogInfo->pPlugIn->ImportFavoriteData(&m_vFavoriateLineData[0], nSum);
	}

	if (panOffset)
	{
		delete []panOffset;
		panOffset = NULL;
	}

}