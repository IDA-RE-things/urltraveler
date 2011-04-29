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
	//名称: PushEvent
	//描述: Push给定的事件
	//参数: 
	//		@param	evt			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual BOOL PushEvent(const Event& evt ) ;

	//----------------------------------------------------------------------------------------
	//名称: PushMessage
	//描述: 需要广播的消息
	//参数: 
	//		@param	msg			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual BOOL PushMessage(const Message& msg);

	//----------------------------------------------------------------------------------------
	//名称: CallService
	//描述: 直接调用另外一个模块的方法
	//参数: 
	//		@param	msg			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual int32 CallService(const param lparam ,param rparam ) ;

protected:

	// 加载每一个模块，获取到该模块的IModule指针，然后并保存起来
	BOOL	LoadModules();

private:

	// 保存给定的DLL中的IModuleFactory以及所有的导出的IModule指针
	typedef std::map<std::wstring,MODULEINTERFACE> ModuleInterfaceMap;
	ModuleInterfaceMap m_mapModuleInterface;

	// 模块ID与模块指针之间的映射关系
	typedef std::map<ModuleId, IModule*> IModulePointMap;
	IModulePointMap m_mapModulePoint;

};