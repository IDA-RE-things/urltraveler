#pragma once

#include "SndaBase.h"
#include "Module.h"
#include <string>
#include <vector>
#include <map>

using namespace std;


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
	//����: PushMessage
	//����: ��Ҫ�㲥����Ϣ
	//����: 
	//		@param	msg			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	virtual BOOL PushMessage(const Message& msg);

	//----------------------------------------------------------------------------------------
	//����: CallService
	//����: ֱ�ӵ�������һ��ģ��ķ���
	//����: 
	//		@param	msg			��Ҫ������¼�
	//----------------------------------------------------------------------------------------
	virtual int32 CallService(const param lparam ,param rparam ) ;

protected:

	// ����ÿһ��ģ�飬��ȡ����ģ���IModuleָ�룬Ȼ�󲢱�������
	BOOL	LoadModules();

private:

	// ���������DLL�е�IModuleFactory�Լ����еĵ�����IModuleָ��
	typedef std::map<std::wstring,MODULEINTERFACE> ModuleInterfaceMap;
	ModuleInterfaceMap m_mapModuleInterface;

	// ģ��ID��ģ��ָ��֮���ӳ���ϵ
	typedef std::map<ModuleId, IModule*> IModulePointMap;
	IModulePointMap m_mapModulePoint;

};