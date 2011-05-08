#include "stdafx.h"
#include "ModuleManagerImpl.h"
#include "ModuleList.h"
#include <sstream>
#include "time.h"
#include "MainFrameDefine.h"
#include "ole2.h"

#define CYCLE_TIMER		10010
#define CYCLE_TIMER_LENGTH	100

using namespace core;

#define 	EXIST_MODULE(Y,X) ((Y=m_mapModulePoint.find((ModuleId)(X))) != m_mapModulePoint.end())


ModuleManagerImpl::ModuleManagerImpl()
:m_eventMsgBuf(10240)
{
	m_bRun = FALSE;
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
//名称: PushAnsycEvent
//描述: 如果源发送者不是位于主进程中，而是位于独立的线程中，此时它需要向总线发送
//		事件，则必须通过异步发送模式。
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
BOOL ModuleManagerImpl::PushAnsycEvent(const Event& evt )
{
	CLockMgr<CCSWrapper> guard(m_Lock, TRUE);
	if(IsEventValue(evt.eventValue))
	{
		m_vectorOut.push_back(evt);
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

//----------------------------------------------------------------------------------------
//名称: PushMessage
//描述: 需要广播的消息
//参数: 
//		@param	msg			需要处理的事件
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
//名称: PushAnsycMessage
//描述: 如果源发送者不是位于主进程中，而是位于独立的线程中，此时它需要向总线发送
//		事件，则必须通过异步发送模式。
//参数: 
//		@param	msg			需要处理的事件
//----------------------------------------------------------------------------------------
BOOL ModuleManagerImpl::PushAnsycMessage(const Message& msg)
{
	CLockMgr<CCSWrapper> guard(m_Lock, TRUE);
	if(	IsMessageValue(msg.messageValue))
	{
		m_vectorOut.push_back(msg);
		return TRUE;
	}

	ASSERT(0);
	return FALSE;
}

//----------------------------------------------------------------------------------------
//名称: CallService
//描述: 直接调用另外一个模块的方法
//参数: 
//		@param	msg			需要处理的事件
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

	//  CallService也支持通过Event的方式传递
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
	}

	// 逐个加载各个模块
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

// 创建和销毁内部窗口
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
	OleInitialize(NULL);

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

void ModuleManagerImpl::Destroy()
{
	// 释放IModule指针数组
	for(IModulePointMap::iterator it = m_mapModulePoint.begin(); it!=m_mapModulePoint.end();++it)
	{
		it->second->Unload();
	}

	m_mapModulePoint.clear();

	for(ModuleInterfaceMap::iterator it=m_mapModuleInterface.begin();it!=m_mapModuleInterface.end();++it)
	{
		it->second.m_pModuleFactory->ReleaseModulePoint(it->second.m_pModules.size(),it->second.m_pModules[0]);
		it->second.m_pReleaseModuleFactoryFunc(it->second.m_pModuleFactory);
	}
	m_mapModuleInterface.clear();

	OleUninitialize();
}

void ModuleManagerImpl::Exit()
{
	m_bRun = FALSE;
}

void	ModuleManagerImpl::Run()
{
	// 创建内部窗口
	CreatCycleWnd();

	// 启动定时器
	m_hTimers = ::SetTimer(m_hInnerWnd, CYCLE_TIMER,  CYCLE_TIMER_LENGTH, NULL);

	m_bRun = TRUE;
	PushMessage(MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_BEGIN_SHOW));

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

	// 结束的时候终止定时器，同时销毁窗口
	KillTimer(NULL,m_hTimers);
	DestroyCycleWnd();
}

void ModuleManagerImpl::OnCycleTrigger()
{
	PushMessage(MakeMessage<MODULE_ID_CORE>()(MESSAGE_VALUE_CORE_CYCLE_TRIGGER));

	// 将异步Push的Event和Message放入事件循环Buffer中
	ASSERT(m_vectorIn.size() == 0);
	if(m_vectorOut.size()!=0)
	{
		CLockMgr<CCSWrapper> guard(m_Lock, TRUE);
		std::swap(m_vectorOut, m_vectorIn);
	}
	for(BufUnitVec::iterator it=m_vectorIn.begin();it!=m_vectorIn.end();++it)
		m_eventMsgBuf.push_back(*it);
	m_vectorIn.clear();

	while(m_eventMsgBuf.size() > 0)
	{
		IModulePointMap::iterator itr;

		Event & eventRef=*reinterpret_cast<Event*>(m_eventMsgBuf[0].szBuffer);
		Message & messageRef=*reinterpret_cast<Message*>(m_eventMsgBuf[0].szBuffer);

		// 如果是Event，则找到目标模块
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

				// ExtraInfo内容必须由源模块进行释放
				if(evt.m_pstExtraInfo)
				{
					if(EXIST_MODULE(itr,evt.srcMId))
						itr->second->PaybackExtraInfo(evt.eventValue,&evt);
					else
						ASSERT(L"找不到源模块，无法释放ExtraInfo");
				}
			}
			else
			{
				ASSERT(L"找不到目标模块");
			}
		}

		// 如果是消息，则调用所有模块的ProcessMessage
		else if(IsMessageValue(messageRef.messageValue))
		{
			Message  message= messageRef;
			m_eventMsgBuf.pop_front();


			for(IModulePointMap::iterator module_it = m_mapModulePoint.begin(); 
				module_it != m_mapModulePoint.end();++module_it)
			{
				(*module_it).second->ProcessMessage(message);
			}

			if(message.m_pstExtraInfo)
			{
				if(EXIST_MODULE(itr,message.srcMId))
					itr->second->PaybackExtraInfo(message.messageValue,&message);
				else
					ASSERT(L"找不到源模块，无法释放ExtraInfo");
			}
		}	

		/** 内存占用优化 
		下面为清理内存时最少保留的内存和其它几个级别的内存数值。
		*/
#define MINI_MEM_SIZEq	8*1024*1024
#define MEDIUM_MEM_SIZE 12*1024*1024
		{
			static unsigned int iFactor = 0;
			if(iFactor<10*60)  //前一分钟，10s调用一次
			{
				if(0==(iFactor%(10*10)))
				{
					::SetProcessWorkingSetSize(::GetCurrentProcess(), MEDIUM_MEM_SIZE, MEDIUM_MEM_SIZE);
				}
			}
			else
			{
				if (0==(iFactor%(10*60*5))) //100ms 5分钟
				{
					::SetProcessWorkingSetSize(::GetCurrentProcess(), MEDIUM_MEM_SIZE, MEDIUM_MEM_SIZE);
				}
			}

			++iFactor;
		}

	}

}