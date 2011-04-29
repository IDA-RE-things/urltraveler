#include "ModuleManagerImpl.h"
#include "ModuleList.h"

ModuleManagerImpl::ModuleManagerImpl()
{
}

ModuleManagerImpl::~ModuleManagerImpl()
{
}

//----------------------------------------------------------------------------------------
//名称: PushEvent
//描述: Push给定的事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
BOOL ModuleManagerImpl::PushEvent(const Event& evt )
{
	return true;
}

//----------------------------------------------------------------------------------------
//名称: PushMessage
//描述: 需要广播的消息
//参数: 
//		@param	msg			需要处理的事件
//----------------------------------------------------------------------------------------
BOOL ModuleManagerImpl::PushMessage(const Message& msg)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: CallService
//描述: 直接调用另外一个模块的方法
//参数: 
//		@param	msg			需要处理的事件
//----------------------------------------------------------------------------------------
int32 ModuleManagerImpl::CallService(const param lparam ,param rparam )
{
	return 0;
}


static MODULELIST g_stInitList;

void	ModuleManagerImpl::LoadModules()
{
	ModuleList::DllModuleList::iterator itr = g_stInitList.m_stDllModuleList.begin();
	for(; itr != g_stInitList.m_stDllModuleList.end(); ++itr)
	{
			IModule *i_module_=NULL;

			HMODULE exportmodule =LoadLibrary(itr->first.c_str());
			ASSERT(exportmodule != NULL);
			if(exportmodule==NULL)
			{
				std::wstring wstrModuleName = itr->first;
				wstrModuleName.append(L" 加载失败");
				MessageBoxW(NULL, wstrModuleName.c_str(), L"DLL加载失败", MB_OK);
				continue ;
			}

			// 加载模块名与模块IModuleFactory之间的关系
			GetModuleFactoryFunc pGetModuleFactoryFunc 
				=  reinterpret_cast<GetModuleFactoryFunc >(GetProcAddress(exportmodule, "GetModuleFactory"));
			ReleaseModuleFactoryFunc pReleaseModuleFactoryFunc 
				= reinterpret_cast<ReleaseModuleFactoryFunc>(GetProcAddress(exportmodule, "ReleaseModuleFactory")); 
			
			m_mapModuleInterface[itr->first]
				=	ModuleInterface(pGetModuleFactoryFunc,pReleaseModuleFactoryFunc,itr->second.size());

			IModuleFactory *pIModuleFactory = m_mapModuleInterface[itr->first].m_pGetModuleFactoryFunc();
			if(pIModuleFactory==NULL)
			{
				std::wstring wstrModuleName = itr->first;
				wstrModuleName.append(L" 导出GetModuleFactory失败");
				MessageBoxW(NULL, wstrModuleName.c_str(), L"DLL加载失败", MB_OK);
				continue ;
			}

			// 得到模块的IModuleFactory指针，然后获取具体的内部的各个IModule指针
			uint32 nCount = 0;
			BOOL bRet =pIModuleFactory->QueryModuleCounter(nCount);
			if( bRet == FALSE)
			{
				std::wstring wstrModuleName = itr->first;
				wstrModuleName.append(L" QueryModuleCounter失败");
				MessageBoxW(NULL, wstrModuleName.c_str(), L"DLL加载失败", MB_OK);
				continue ;
			}

			m_mapModuleInterface[itr->first].m_pModuleFactory = pIModuleFactory;
			if( nCount <= m_mapModuleInterface[itr->first].m_pModules.size())
			{
				bRet = pIModuleFactory->QueryModulePoint(nCount, m_mapModuleInterface[itr->first].m_pModules[0]);
				if( bRet == FALSE)
				{
					std::wstring wstrModuleName = itr->first;
					wstrModuleName.append(L" QueryModulePoint失败");
					MessageBoxW(NULL, wstrModuleName.c_str(), L"DLL加载失败", MB_OK);
					continue ;
				}

				// 设置模块ID和具体模块IModule指针的关联关系
				for(size_t i=0; i<nCount; i++)
				{
					m_mapModulePoint[itr->second[i]] = m_mapModuleInterface[itr->first].m_pModules[i];
				}
			}
			
			// 逐个加载各个模块
			for(IModulePointMap::iterator it=m_mapModulePoint.begin(); it != m_mapModulePoint.end(); ++it)
			{
				it->second->Load(this);
			}
	}
}

BOOL ModuleManagerImpl::Init()
{
	LoadModules();

	// 在vista和win7下允许低等级进程向高等级进程发送消息
#define MSGFLT_ADD	1
#define MSGFLT_REMOVE	2

	HMODULE hUserModule = LoadLibrary(L"user32.dll");
	if( NULL == hUserModule)
		return FALSE;

	typedef BOOL (WINAPI *PChangeWindowMessageFilter)(UINT, DWORD);
	PChangeWindowMessageFilter pMsgFilter = (PChangeWindowMessageFilter)GetProcAddress(
	hUserModule, "ChangeWindowMessageFilter");

	if( NULL != pMsgFilter)
	{
		BOOL bResult = pMsgFilter(WM_COPYDATA, MSGFLT_ADD);
		ASSERT(bResult == TRUE);
	}

	if( hUserModule != NULL)
		FreeLibrary(hUserModule);

	return TRUE;
}

BOOL	ModuleManagerImpl::Exit()
{
	return TRUE;
}