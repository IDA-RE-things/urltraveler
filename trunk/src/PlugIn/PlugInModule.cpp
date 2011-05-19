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

// 导出借口实现
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
//名称: Unload
//描述: 主程序通过该方法对模块进行卸载
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
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
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* PlugInModule::GetModuleName() 
{
	return L"PlugInModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const PlugInModule::GetModuleId()
{
	return MODULE_ID_PLUGIN;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void PlugInModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//名称: ProcessMessage
//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
//参数: 
//		@param	msg			需要处理的广播消息
//----------------------------------------------------------------------------------------
void PlugInModule::ProcessMessage(const Message& msg) 
{
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lparam			参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 PlugInModule::CallDirect(const param lparam, param wparam) 
{
	return -1;
}

//----------------------------------------------------------------------------------------
//名称: PaybackExtraInfo
//描述: 某个模块如果有自定义的复杂的数据需要传给其余的模块，那么它可以构造一个ExtraInfo结构
//	在其余的模块使用完毕后，该结构必须被释放，否则会造成内存泄露。释放必须由模块自身完成。
//	某个模块都必须提供一个PaybackExtraInfo接口，释放自己的自定义的数据类型
//参数: 
//		@param	valudId			对应的pExtraInfo的值，内部根据该值进行对应的释放，该值只有模块自己理解
//		@param	pExtraInfo	需要释放的ExtraInfo数据
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
			wstrModuleName.append(L" LoadLibrary失败");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn加载失败", MB_OK);
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
			wstrModuleName.append(L" 找不到GetPlugInFactory函数失败");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn加载失败", MB_OK);
			continue ;
		}

		m_vPlugInFactory.push_back(pPlugInFactory);

		// 调用Factory的Query方法，查找当前的Factory中支持的所有的PlugIn
		uint32 nPlugNum = 0;
		BOOL bRet = pPlugInFactory->QueryPlugInCounter(nPlugNum);
		if( bRet == FALSE)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" 找不到QueryPlugInCounter函数失败");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn加载失败", MB_OK);
			continue ;
		}

		stPlugInInfo.pvPlugIn.resize(nPlugNum);


		// 导出的所有的插件保存在std::vector<IPlugIn*>中
		bRet = pPlugInFactory->QueryPlugInPoint(nPlugNum, stPlugInInfo.pvPlugIn[0]);
		if( bRet == FALSE)
		{
			std::wstring wstrModuleName = strPlugInPath;
			wstrModuleName.append(L" 找不到QueryPlugInPoint函数失败");
			MessageBoxW(NULL, wstrModuleName.c_str(), L"PlugIn加载失败", MB_OK);
			continue ;
		}

		// 对插件进行加载操作
		for(int i=0; i<nPlugNum; i++)
		{
			stPlugInInfo.pvPlugIn[i]->Load();
			m_vPlugIns.push_back(stPlugInInfo.pvPlugIn[i]);
		}

		m_vPlugInModuleInfo.push_back(stPlugInInfo);
	}


	// 调用各个模块的IsWorked方法，检查当前插件是否需要启用
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
		// 如果当前插件不能工作，则将其删除
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

	//把所有相同父结点的节点放到vec中
	for (int i = 0; i < nLen; i++)
	{
		if (pData[i].nPid == nParentId)
		{
			vec.push_back(&pData[i]);
		}
	}

	int vSize = vec.size();

	//如果扫描出来的结点数少于1，merge结束, 即至少需要两个节点指到同一个父结点
	if (vSize > 1)
	{
		//对vec按hashid进行排序, 主要方便下面一次遍历就能找出所有相同元素
		sort(vec.begin(), vec.end(), compare);

		for (int i = 0; i < vSize - 1; i++)
		{
			//同一个父节点下如果有Hash相同的两个点，则该两个点需要合并
			if (vec[i]->nHashId == vec[i + 1]->nHashId)
			{
				//置上懒删除标记, 向后删除
				vec[i]->bDelete = true;
				//重新修正所有父节点为j的节点的nPid, 即合并
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

// 通知加载合并所有的收藏夹数据
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
	
		// 对所有的浏览器数据进行合并
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
		
		// 将合并后的数据导入到各个浏览器中
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

	// 对所有的浏览器数据进行合并
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
	
	// 将合并后的数据导入到各个浏览器中
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

	//使线程直接退掉返回0，否则需要自己去Shutdown
	return 0;
}

void PlugInModule::OnThreadExit()
{
}
