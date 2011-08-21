#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "PlugIn.h"
#include <vector>
#include "ThreadObject_i.h"
#include "Guard.h"
#include "PlugInDefine.h"
#include "XSync.h"


using namespace std;
using namespace plugin;

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class PlugInModule : public ModuleImpl,
					public IThreadEvent
{
	DECLEAR_EVENT_MAP(PlugInModule)
	DECLEAR_SERVICE_MAP(PlugInModule)

public:
	PlugInModule();
	~PlugInModule();

	//----------------------------------------------------------------------------------------
	//名称: Unload
	//描述: 主程序通过该方法对模块进行卸载
	//返回: 
	//		如果卸载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Unload();

	//----------------------------------------------------------------------------------------
	//名称: GetModuleName
	//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
	//返回: 
	//		如果卸载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	const wchar_t* GetModuleName();

 	//----------------------------------------------------------------------------------------
	//名称: GetModuleId
	//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
	//返回: 
	//		返回该模块的唯一的ID
	//----------------------------------------------------------------------------------------
	uint32 const GetModuleId();

 	//----------------------------------------------------------------------------------------
	//名称: ProcessEvent
	//描述: 该模块可以处理的所有的点对点事件
	//参数: 
	//		@param	evt			需要处理的事件
	//----------------------------------------------------------------------------------------
	void ProcessEvent(const Event& evt);

 	//----------------------------------------------------------------------------------------
	//名称: ProcessMessage
	//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
	//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
	//参数: 
	//		@param	msg			需要处理的广播消息
	//----------------------------------------------------------------------------------------
	void ProcessMessage(const Message& msg);

 	//----------------------------------------------------------------------------------------
	//名称: CallDirect
	//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
	//			方法而不需要通过总线。
	//参数: 
	//		@param	lparam			参数1
	//		@param	rparam			参数2
	//----------------------------------------------------------------------------------------
	int32 CallDirect(const ServiceValue lServiceValue, param wparam);

 	//----------------------------------------------------------------------------------------
	//名称: PaybackExtraInfo
	//描述: 某个模块如果有自定义的复杂的数据需要传给其余的模块，那么它可以构造一个ExtraInfo结构
	//	在其余的模块使用完毕后，该结构必须被释放，否则会造成内存泄露。释放必须由模块自身完成。
	//	某个模块都必须提供一个PaybackExtraInfo接口，释放自己的自定义的数据类型
	//参数: 
	//		@param	valudId			对应的pExtraInfo的值，内部根据该值进行对应的释放，该值只有模块自己理解
	//		@param	pExtraInfo	需要释放的ExtraInfo数据
	//----------------------------------------------------------------------------------------
	void PaybackExtraInfo(uint32 valudId, void* pExtraInfo);

public:
	//! 进入线程虚函数
	virtual void OnThreadEntry();

	virtual int Run();

	//! 线程退出虚函数
	virtual void OnThreadExit();

	// Event处理函数
protected:

	void	OnEvent_LoadAllPlugin(Event* pEvent);
	void	OnEvent_CheckPlugInWorked(Event* pEvent);
	void	OnEvent_BeginToSync(Event* pEvent);

protected:

	void	OnService_GetAvailablePlugIns(ServiceValue lServiceValue, param	lParam);

private:

	//	将pFavoriteData进行合并
	void	Merge(PFAVORITELINEDATA* ppData, int32 nLen, int nParentId);

	void	DoExportThread();
	void	DoImportThread();

private:

	int                 m_nSumFavorite;

	//	保存所有的插件模块指针
	std::vector<IPlugInFactory*>	m_vPlugInFactory;

	// 存储各个模块中的IPlugIn指针
	std::vector<PLUGININFO>		m_vPlugInModuleInfo;

	//	保存所有的插件
	std::vector<IPlugIn*>		m_vPlugIns;	

	enum EThreadDoing
	{
		NONE,
		EXPORT,
		IMPORT,
	};

	// 需要执行哪一个现成
	CXMutex	m_ThreadMutex;
	EThreadDoing	m_eThreadToDoing;

	IThreadObject*      m_pExportFavThread;
	IThreadObject*      m_pImportFavThread;
};

class PlugInModuleFactory : public ModuleFactoryImpl<PlugInModule>{};

namespace plugin
{
	extern PlugInModule*	g_PlugInModule;
	extern PlugInModuleFactory*	g_PlugInModuleFactory;
}
using namespace plugin;