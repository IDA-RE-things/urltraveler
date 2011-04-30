#include "ModuleManagerImpl.h"
#include "ModuleList.h"
#include <sstream>
#include "time.h"

#define CYCLE_TIMER		10010
#define CYCLE_TIMER_LENGTH	100

using namespace core;

#define 	EXIST_MODULE(Y,X) ((Y=m_mapModulePoint.find((ModuleId)(X))) != m_mapModulePoint.end())


ModuleManagerImpl::ModuleManagerImpl()
:m_eventMsgBuf(10240),
m_bRun(FALSE)
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
	ASSERT(evt.srcMId != MODULE_ID_INVALID && evt.desMId != MODULE_ID_INVALID);
	if( m_eventMsgBuf.size() > 10000)
	{
		ASSERT("ModuleManagerImpl::PushEvent: circle buffer is full, please wait a while");
		return FALSE;
	}

	if(IsEventValue(evt.eventValue))
	{
		m_eventMsgBuf.push_back(evt);
		return TRUE;
	}

	ASSERT("ModuleManagerImpl::PushEvent: evt is not a event");
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: PushMessage
//����: ��Ҫ�㲥����Ϣ
//����: 
//		@param	msg			��Ҫ������¼�
//----------------------------------------------------------------------------------------
BOOL ModuleManagerImpl::PushMessage(const Message& msg)
{
	ASSERT(msg.srcMId != MODULE_ID_INVALID);
	if( m_eventMsgBuf.size() > 10000)
	{
		ASSERT("ModuleManagerImpl::PushMessage: circle buffer is full, please wait a while");
		return FALSE;
	}

	if(IsMessageValue(msg.messageValue))
	{
		m_eventMsgBuf.push_back(msg);
		return TRUE;
	}

	ASSERT("ModuleManagerImpl::PushMessage: msg is not a message");
	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: CallService
//����: ֱ�ӵ�������һ��ģ��ķ���
//����: 
//		@param	msg			��Ҫ������¼�
//----------------------------------------------------------------------------------------
int32 ModuleManagerImpl::CallService(const ServiceValue lServiceValue ,param rparam )
{
	int nRet = -1;
	IModulePointMap::iterator itr;

	if(IsServiceValue(lServiceValue))
	{
		if(EXIST_MODULE(itr,MODULIE_ID_FROM_SERVICE_VALUE(lServiceValue)) == FALSE)
		{
			ASSERT(0);
			return -1;
		}
		return itr->second->CallDirect(lServiceValue,rparam);
	}

	//  CallServiceҲ֧��ͨ��Event�ķ�ʽ����
	if(IsEventValue(lServiceValue))
	{
		if(EXIST_MODULE(itr,MODULIE_ID_FROM_EVENT_VALUE(lServiceValue)) == FALSE)
		{
			ASSERT(0);
			return -1;
		}
		return itr->second->CallDirect(lServiceValue,rparam);
	}

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
	}

	// ������ظ���ģ��
	for(IModulePointMap::iterator it=m_mapModulePoint.begin(); it != m_mapModulePoint.end(); ++it)
	{
		it->second->Load(this);
	}
}

static LRESULT CALLBACK CycleProc(HWND inWindow, UINT inMsg, WPARAM inParam, LPARAM inOtherParam)
{
	ModuleManagerImpl * pModuleManagerImpl = (ModuleManagerImpl*)GetWindowLong(inWindow, GWL_USERDATA);
	if(pModuleManagerImpl)
	{
		if(inMsg==WM_TIMER)
		{
			pModuleManagerImpl->OnCycleTrigger();
			return 1 ;
		}

		return DefWindowProc(inWindow, inMsg, inParam, inOtherParam);
	}

	if(inMsg == WM_NCCREATE)
		return 1;

	return DefWindowProcW(inWindow, inMsg, inParam, inOtherParam);
}

static uint32 s_iWndAtom = (unsigned)time( NULL );

// �����������ڲ�����
void ModuleManagerImpl::CreatCycleWnd()
{
	std::wstringstream os;
	os<<L"CycleWnd"<<s_iWndAtom;
	std::wstring className=os.str();

	WNDCLASSEX theWndClass;
	theWndClass.cbSize = sizeof(theWndClass);
	theWndClass.style = 0;
	theWndClass.lpfnWndProc = &CycleProc;
	theWndClass.cbClsExtra = 0;
	theWndClass.cbWndExtra = 0;
	theWndClass.hInstance = NULL;
	theWndClass.hIcon = NULL;
	theWndClass.hCursor = NULL;
	theWndClass.hbrBackground = NULL;
	theWndClass.lpszMenuName = NULL;
	theWndClass.lpszClassName = className.c_str();
	theWndClass.hIconSm = NULL;
	ATOM theWndAtom = ::RegisterClassEx(&theWndClass);
	ASSERT(theWndAtom != NULL);

	m_hInnerWnd = ::CreateWindow( className.c_str(), className.c_str(), 
		WS_POPUP, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, NULL, NULL);
	ASSERT(m_hInnerWnd != NULL);

	SetWindowLongW(m_hInnerWnd, GWL_USERDATA, (LONG)this);
}

void ModuleManagerImpl::DestroyCycleWnd()
{
	::DestroyWindow(m_hInnerWnd);

	std::wstringstream os;
	os<<L"CycleWnd"<<s_iWndAtom;
	std::wstring className=os.str();
	UnregisterClass(className.c_str(), NULL );
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

BOOL ModuleManagerImpl::Exit()
{
	return TRUE;
}

void	ModuleManagerImpl::Run()
{
	// �����ڲ�����
	CreatCycleWnd();

	// ������ʱ��
	m_hTimers = ::SetTimer(m_hInnerWnd, CYCLE_TIMER,  CYCLE_TIMER_LENGTH, NULL);


	m_bRun = TRUE;

	OnCycleTrigger();

	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0) 
		&& ( m_bRun != FALSE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)
		&& m_eventMsgBuf.size() > 0 )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// ������ʱ����ֹ��ʱ����ͬʱ���ٴ���
	KillTimer(NULL,m_hTimers);
	DestroyCycleWnd();
}

void ModuleManagerImpl::OnCycleTrigger()
{
	PushMessage(MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_SYS_CYCLE_TRIGGER));

	while(m_eventMsgBuf.size() > 0)
	{
		IModulePointMap::iterator itr;

		Event & eventRef=*reinterpret_cast<Event*>(m_eventMsgBuf[0].szBuffer);
		Message & messageRef=*reinterpret_cast<Message*>(m_eventMsgBuf[0].szBuffer);

		// �����Event�����ҵ�Ŀ��ģ��
		if(IsEventValue(eventRef.eventValue))
		{
			Event  evt = eventRef;

			m_eventMsgBuf.pop_front();
			if(evt.eventValue==EVENT_VALUE_CORE_MAIN_LOOP_EXIT)
			{
				Exit();
				continue;
			}

			if(EXIST_MODULE(itr,evt.desMId))
			{
				itr->second->ProcessEvent(evt);

				// ExtraInfo���ݱ�����Դģ������ͷ�
				if(evt.m_pstExtraInfo)
				{
					if(EXIST_MODULE(itr,evt.srcMId))
						itr->second->PaybackExtraInfo(evt.eventValue,&evt);
					else
						ASSERT(L"�Ҳ���Դģ�飬�޷��ͷ�ExtraInfo");
				}
			}
			else
			{
				ASSERT(L"�Ҳ���Ŀ��ģ��");
			}
		}

		// �������Ϣ�����������ģ���ProcessMessage
		else if(IsMessageValue(messageRef.messageValue))
		{
			Message  message= messageRef;
			m_eventMsgBuf.pop_front();

			for(IModulePointMap::iterator it = m_mapModulePoint.begin(); it != m_mapModulePoint.end();++itr)
			{
				(*it).second->ProcessMessage(message);
			}

			if(message.m_pstExtraInfo)
			{
				if(EXIST_MODULE(itr,message.srcMId))
					itr->second->PaybackExtraInfo(message.messageValue,&message);
				else
					ASSERT(L"�Ҳ���Դģ�飬�޷��ͷ�ExtraInfo");
			}
		}	

	}

}