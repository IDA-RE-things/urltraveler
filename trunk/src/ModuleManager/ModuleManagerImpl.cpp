#include "ModuleManagerImpl.h"
#include "ModuleList.h"

ModuleManagerImpl::ModuleManagerImpl()
{
}

ModuleManagerImpl::~ModuleManagerImpl()
{
}

//----------------------------------------------------------------------------------------
//����: PushEvent
//����: Push�������¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
BOOL ModuleManagerImpl::PushEvent(const Event& evt )
{
	return true;
}

//----------------------------------------------------------------------------------------
//����: PushMessage
//����: ��Ҫ�㲥����Ϣ
//����: 
//		@param	msg			��Ҫ������¼�
//----------------------------------------------------------------------------------------
BOOL ModuleManagerImpl::PushMessage(const Message& msg)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: CallService
//����: ֱ�ӵ�������һ��ģ��ķ���
//����: 
//		@param	msg			��Ҫ������¼�
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
				wstrModuleName.append(L" ����ʧ��");
				MessageBoxW(NULL, L"DLL����ʧ��", wstrModuleName.c_str(), MB_OK);
				continue ;
			}



	}

	return TRUE;

}
