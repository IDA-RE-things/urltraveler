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


#define CYCLE_TIME		10010				//	��ʱ����ID

	class ModuleManagerImpl : public IModuleManager
	{
	public:
		ModuleManagerImpl();
		virtual ~ModuleManagerImpl();

		//----------------------------------------------------------------------------------------
		//����: PushEvent
		//����: Push�������¼�
		//����: 
		//		@param	evt			��Ҫ������¼�
		//----------------------------------------------------------------------------------------
		virtual BOOL PushEvent(const Event& evt ) ;

		//----------------------------------------------------------------------------------------
		//����: PushAnsycEvent
		//����: ���Դ�����߲���λ���������У�����λ�ڶ������߳��У���ʱ����Ҫ�����߷���
		//		�¼��������ͨ���첽����ģʽ��
		//����: 
		//		@param	evt			��Ҫ������¼�
		//----------------------------------------------------------------------------------------
		virtual BOOL PushAnsycEvent(const Event& evt ) ;

		//----------------------------------------------------------------------------------------
		//����: PushMessage
		//����: ��Ҫ�㲥����Ϣ
		//����: 
		//		@param	msg			��Ҫ������¼�
		//----------------------------------------------------------------------------------------
		virtual BOOL PushMessage(const Message& msg);

		//----------------------------------------------------------------------------------------
		//����: PushAnsycMessage
		//����: ���Դ�����߲���λ���������У�����λ�ڶ������߳��У���ʱ����Ҫ�����߷���
		//		�¼��������ͨ���첽����ģʽ��
		//����: 
		//		@param	msg			��Ҫ������¼�
		//----------------------------------------------------------------------------------------
		virtual BOOL PushAnsycMessage(const Message& msg);

		//----------------------------------------------------------------------------------------
		//����: CallService
		//����: ֱ�ӵ�������һ��ģ��ķ���
		//����: 
		//		@param	msg			��Ҫ������¼�
		//----------------------------------------------------------------------------------------
		virtual int32 CallService(const ServiceValue lServiceValue ,param rparam ) ;

		//----------------------------------------------------------------------------------------
		//����: Init/Destory
		//����: ִ�г�ʼ�������ٹ�����
		//----------------------------------------------------------------------------------------
		BOOL	Init();
		void		Destroy();

		void		Run();

		//----------------------------------------------------------------------------------------
		//����: OnCycleTrigger
		//����: ��ÿ�ζ�ʱ������Ҫ���������
		//����: 
		//		@param	msg			��Ҫ������¼�
		//----------------------------------------------------------------------------------------
		void		OnCycleTrigger();



	protected:

		// ����ÿһ��ģ�飬��ȡ����ģ���IModuleָ�룬Ȼ�󲢱�������
		void		LoadModules();
		void		Exit();

		// ˽�к���
	private:

		// �����������ڲ�����
		void CreatCycleWnd();
		void DestroyCycleWnd();




	private:

		// ���������DLL�е�IModuleFactory�Լ����еĵ�����IModuleָ��
		typedef std::map<std::wstring,MODULEINTERFACE> ModuleInterfaceMap;
		ModuleInterfaceMap m_mapModuleInterface;

		// ģ��ID��ģ��ָ��֮���ӳ���ϵ
		typedef std::map<ModuleId, IModule*> IModulePointMap;
		IModulePointMap m_mapModulePoint;

	private:



		// �����Ƿ��Ѿ���������״̬
		BOOL	m_bRun;

		// ѭ��Buffer���洢���������е�Event��Message
		boost::circular_buffer<BufUnit> m_eventMsgBuf;

		// �ڲ������ش��ھ�������ɼ�
		HWND	m_hInnerWnd;

		// ��ʱ��
		HTIME		m_hTimers;


	};

}

