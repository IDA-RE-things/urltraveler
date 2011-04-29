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

BOOL	ModuleManagerImpl::LoadModules()
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
				MessageBoxW(NULL, L"DLL加载失败", wstrModuleName.c_str(), MB_OK);
				continue ;
			}



	}

	return TRUE;

}
