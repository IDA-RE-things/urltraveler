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
				wstrModuleName.append(L" ����ʧ��");
				MessageBoxW(NULL, wstrModuleName.c_str(), L"DLL����ʧ��", MB_OK);
				continue ;
			}

			// ����ģ������ģ��IModuleFactory֮��Ĺ�ϵ
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
				wstrModuleName.append(L" ����GetModuleFactoryʧ��");
				MessageBoxW(NULL, wstrModuleName.c_str(), L"DLL����ʧ��", MB_OK);
				continue ;
			}

			// �õ�ģ���IModuleFactoryָ�룬Ȼ���ȡ������ڲ��ĸ���IModuleָ��
			uint32 nCount = 0;
			BOOL bRet =pIModuleFactory->QueryModuleCounter(nCount);
			if( bRet == FALSE)
			{
				std::wstring wstrModuleName = itr->first;
				wstrModuleName.append(L" QueryModuleCounterʧ��");
				MessageBoxW(NULL, wstrModuleName.c_str(), L"DLL����ʧ��", MB_OK);
				continue ;
			}

			m_mapModuleInterface[itr->first].m_pModuleFactory = pIModuleFactory;
			if( nCount <= m_mapModuleInterface[itr->first].m_pModules.size())
			{
				bRet = pIModuleFactory->QueryModulePoint(nCount, m_mapModuleInterface[itr->first].m_pModules[0]);
				if( bRet == FALSE)
				{
					std::wstring wstrModuleName = itr->first;
					wstrModuleName.append(L" QueryModulePointʧ��");
					MessageBoxW(NULL, wstrModuleName.c_str(), L"DLL����ʧ��", MB_OK);
					continue ;
				}

				// ����ģ��ID�;���ģ��IModuleָ��Ĺ�����ϵ
				for(size_t i=0; i<nCount; i++)
				{
					m_mapModulePoint[itr->second[i]] = m_mapModuleInterface[itr->first].m_pModules[i];
				}
			}
			
			// ������ظ���ģ��
			for(IModulePointMap::iterator it=m_mapModulePoint.begin(); it != m_mapModulePoint.end(); ++it)
			{
				it->second->Load(this);
			}
	}
}

BOOL ModuleManagerImpl::Init()
{
	LoadModules();

	// ��vista��win7������͵ȼ�������ߵȼ����̷�����Ϣ
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