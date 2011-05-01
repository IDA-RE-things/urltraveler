#pragma once

#include "SndaBase.h"
#include "Module.h"
#include <string>
#include <vector>
#include <map>

#include <boost/circular_buffer.hpp>
#include <boost/bimap.hpp>


using namespace std;

namespace core
{

	enum
	{
		MAXER = (sizeof(Event) >sizeof(Message)  ? sizeof(Event) : sizeof(Message))
	};

	struct BufUnit
	{
		BufUnit(){}
		BufUnit(const Event& evt)
		{
			Event& e = *reinterpret_cast<Event*>(szBuffer);
			e = evt;
		}

		BufUnit(Message const & msg)
		{
			Message& m =*reinterpret_cast<Message*>(szBuffer);
			m = msg;
		}

		char szBuffer[MAXER];
	};




	typedef IModuleFactory *  (*GetModuleFactoryFunc)();
	typedef void  (*ReleaseModuleFactoryFunc)(IModuleFactory*);

	struct ModuleInterface
	{
		ModuleInterface()
			:m_pGetModuleFactoryFunc(NULL),
			m_pReleaseModuleFactoryFunc(NULL)
		{}

		ModuleInterface(GetModuleFactoryFunc  p1,
			ReleaseModuleFactoryFunc p2,std::size_t size)
			:m_pGetModuleFactoryFunc(p1),
			m_pReleaseModuleFactoryFunc(p2),
			m_pModules(size),
			m_pModuleFactory(NULL)
		{}

		GetModuleFactoryFunc m_pGetModuleFactoryFunc;
		ReleaseModuleFactoryFunc m_pReleaseModuleFactoryFunc;

		IModuleFactory * m_pModuleFactory;
		std::vector<IModule*> m_pModules;
	};
	typedef ModuleInterface MODULEINTERFACE;
	typedef MODULEINTERFACE* PMODULEINTERFACE;


#define CYCLE_TIME		10010				//	定时器的ID

	class ModuleManagerImpl : public IModuleManager
	{
	public:
		ModuleManagerImpl();
		virtual ~ModuleManagerImpl();

		//----------------------------------------------------------------------------------------
		//名称: PushEvent
		//描述: Push给定的事件
		//参数: 
		//		@param	evt			需要处理的事件
		//----------------------------------------------------------------------------------------
		virtual BOOL PushEvent(const Event& evt ) ;

		//----------------------------------------------------------------------------------------
		//名称: PushAnsycEvent
		//描述: 如果源发送者不是位于主进程中，而是位于独立的线程中，此时它需要向总线发送
		//		事件，则必须通过异步发送模式。
		//参数: 
		//		@param	evt			需要处理的事件
		//----------------------------------------------------------------------------------------
		virtual BOOL PushAnsycEvent(const Event& evt ) ;

		//----------------------------------------------------------------------------------------
		//名称: PushMessage
		//描述: 需要广播的消息
		//参数: 
		//		@param	msg			需要处理的事件
		//----------------------------------------------------------------------------------------
		virtual BOOL PushMessage(const Message& msg);

		//----------------------------------------------------------------------------------------
		//名称: PushAnsycMessage
		//描述: 如果源发送者不是位于主进程中，而是位于独立的线程中，此时它需要向总线发送
		//		事件，则必须通过异步发送模式。
		//参数: 
		//		@param	msg			需要处理的事件
		//----------------------------------------------------------------------------------------
		virtual BOOL PushAnsycMessage(const Message& msg);

		//----------------------------------------------------------------------------------------
		//名称: CallService
		//描述: 直接调用另外一个模块的方法
		//参数: 
		//		@param	msg			需要处理的事件
		//----------------------------------------------------------------------------------------
		virtual int32 CallService(const ServiceValue lServiceValue ,param rparam ) ;

		//----------------------------------------------------------------------------------------
		//名称: Init/Destory
		//描述: 执行初始化和销毁工工作
		//----------------------------------------------------------------------------------------
		BOOL	Init();
		void		Destroy();

		void		Run();

		//----------------------------------------------------------------------------------------
		//名称: OnCycleTrigger
		//描述: 在每次定时器中需要处理的事情
		//参数: 
		//		@param	msg			需要处理的事件
		//----------------------------------------------------------------------------------------
		void		OnCycleTrigger();



	protected:

		// 加载每一个模块，获取到该模块的IModule指针，然后并保存起来
		void		LoadModules();
		void		Exit();

		// 私有函数
	private:

		// 创建和销毁内部窗口
		void CreatCycleWnd();
		void DestroyCycleWnd();




	private:

		// 保存给定的DLL中的IModuleFactory以及所有的导出的IModule指针
		typedef std::map<std::wstring,MODULEINTERFACE> ModuleInterfaceMap;
		ModuleInterfaceMap m_mapModuleInterface;

		// 模块ID与模块指针之间的映射关系
		typedef std::map<ModuleId, IModule*> IModulePointMap;
		IModulePointMap m_mapModulePoint;

	private:



		// 总线是否已经处于运行状态
		BOOL	m_bRun;

		// 循环Buffer，存储总线上所有的Event和Message
		boost::circular_buffer<BufUnit> m_eventMsgBuf;

		// 内部的隐藏窗口句柄，不可见
		HWND	m_hInnerWnd;

		// 定时器
		HTIME		m_hTimers;


	};

}

