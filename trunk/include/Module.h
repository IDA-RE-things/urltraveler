#pragma  once

#include "SndaBase.h"
#include "UrlTravelerHeader.h"

interface IModule;
interface IModuleFactory;
interface IModuleManager;

interface IModule
{
	//----------------------------------------------------------------------------------------
	//名称: Load
	//描述: 主程序通过该方法对模块进行加载
	//参数: 
	//		@param	pManager			主模块总线的指针	
	//返回: 
	//		如果加载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Load(IModuleManager* pManager) PURE;

	//----------------------------------------------------------------------------------------
	//名称: Unload
	//描述: 主程序通过该方法对模块进行卸载
	//返回: 
	//		如果卸载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Unload() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetModuleName
	//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
	//返回: 
	//		如果卸载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetModuleName() PURE;

 	//----------------------------------------------------------------------------------------
	//名称: GetModuleId
	//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
	//返回: 
	//		返回该模块的唯一的ID
	//----------------------------------------------------------------------------------------
	virtual uint32 const GetModuleId() PURE;

 	//----------------------------------------------------------------------------------------
	//名称: ProcessEvent
	//描述: 该模块可以处理的所有的点对点事件
	//参数: 
	//		@param	evt			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual void ProcessEvent(const Event& evt) PURE;

 	//----------------------------------------------------------------------------------------
	//名称: ProcessMessage
	//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
	//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
	//参数: 
	//		@param	msg			需要处理的广播消息
	//----------------------------------------------------------------------------------------
	virtual void ProcessMessage(const Message& msg) PURE;

 	//----------------------------------------------------------------------------------------
	//名称: CallDirect
	//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
	//			方法而不需要通过总线。
	//参数: 
	//		@param	lServiceValue		参数1
	//		@param	rparam			参数2
	//----------------------------------------------------------------------------------------
	virtual int32 CallDirect(const ServiceValue lServiceValue, param rparam) PURE;

 	//----------------------------------------------------------------------------------------
	//名称: PaybackExtraInfo
	//描述: 某个模块如果有自定义的复杂的数据需要传给其余的模块，那么它可以构造一个ExtraInfo结构
	//	在其余的模块使用完毕后，该结构必须被释放，否则会造成内存泄露。释放必须由模块自身完成。
	//	某个模块都必须提供一个PaybackExtraInfo接口，释放自己的自定义的数据类型
	//参数: 
	//		@param	valudId			对应的pExtraInfo的值，内部根据该值进行对应的释放，该值只有模块自己理解
	//		@param	pExtraInfo	需要释放的ExtraInfo数据
	//----------------------------------------------------------------------------------------
	virtual void PaybackExtraInfo(uint32 valudId, void* pExtraInfo) PURE;
};


#ifndef HANDLER_CALL
#define HANDLER_CALL
#endif


// Event处理映射函数
#define	DECLEAR_EVENT_MAP()  \
private:   \
	typedef void (HANDLER_CALL IModule::*PEventHandler)(Event* pEvent );     \
	typedef struct _EventHanderMapEntries   \
	{   \
		EventValue		nEventValue;   \
		PEventHandler		pfEventHandler;   \
	} EventHandlerMapEntries;   \
	static EventHandlerMapEntries m_eventMap[]; 

#define	BEGIN_EVENT_MAP( ModuleClass ) \
	ModuleClass##::EventHandlerMapEntries ModuleClass##::m_eventMap[] ={ \

#define	ON_EVENT( event_value, event_handler)  \
	{ event_value,  (PEventHandler)&event_handler },

#define	END_EVENT_MAP() \
	{ 0, (PEventHandler)NULL}};


// 消息处理映射函数
#define	DECLEAR_MESSAGE_MAP()  \
private:   \
	typedef void (HANDLER_CALL IModule::*PMessageHandler)(Message* pMessage );     \
	typedef struct _MessageHanderMapEntries   \
	{   \
		MessageValue		nMessageValue;   \
		PMessageHandler	pfMessageHandler;   \
	} MessageHandlerMapEntries;   \
	\
	static MessageHandlerMapEntries m_messageMap[];

#define	BEGIN_MESSAGE_MAP( ModuleClass ) \
	ModuleClass##::MessageHandlerMapEntries ModuleClass##::m_messageMap[] ={ \

#define	ON_MESSAGE( message_value, message_handler)  \
	{ message_value,  (PMessageHandler)&message_handler },

#define	END_MESSAGE_MAP() \
	{ 0, (PMessageHandler)NULL}};



// 直接调用处理映射函数
#define	DECLEAR_SERVICE_MAP()  \
private:   \
	typedef void (HANDLER_CALL IModule::*PServiceHandler)( ServiceValue value, param wparam );    \
	typedef struct _ServiceHanderMapEntries    \
	{    \
		ServiceValue		nServiceValue;		    \
		PServiceHandler	pfServiceHandler;	\
	} ServiceHandlerMapEntries;    \
	static ServiceHandlerMapEntries m_serviceMap[];

#define	BEGIN_SERVICE_MAP( ModuleClass ) \
	ModuleClass##::ServiceHandlerMapEntries ModuleClass##::m_serviceMap[] ={ \

#define	ON_SERVICE( service_value, service_handler)  \
	{ service_value, (PServiceHandler)&service_handler },

#define	END_SERVICE_MAP() \
	{ 0, (PServiceHandler)NULL}};


#define DECLARE_HANDLER_MAP()  \
	DECLEAR_EVENT_MAP()  \
	DECLEAR_MESSAGE_MAP()  \
	DECLEAR_SERVICE_MAP()

//----------------------------------------------------------------------------------------
//名称: IModuleFactory
//描述: 一个DLL中可以包含多个IModule，通过IModuleFactory，总线可以知晓当前Dll中的
//	所有的模块。引入IModuleFactory的最主要的原因是为了避免Project过多。
//----------------------------------------------------------------------------------------
interface IModuleFactory
{
	//----------------------------------------------------------------------------------------
	//名称: QueryIModuleCounter
	//描述: 返回当前Dll中支持的模块的数目
	//参数: 
	//		@param	nCount			返回的模块数目，比如1，2等
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryModuleCounter(uint32&  nCount) PURE;

 	//----------------------------------------------------------------------------------------
	//名称: QueryIModulePoint
	//描述: 返回当前Dll中支持的模块的数目
	//参数: 
	//		@param	nCount			参数表明后面buf的维度。如果是2，表明后面buf的维度是2
	//		@param	ppModule		指向一个IModule数组的开始位置，buf的内容由实现者填充
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryModulePoint(uint32 nCount, IModule *&  ppModule) PURE;

	//----------------------------------------------------------------------------------------
	//名称: ReleaseIModulePoint
	//描述: 释放当前DLL中的所有的IModule
	//参数: 
	//		@param	nCount			参数表明后面buf的维度。如果是2，表明后面buf的维度是2
	//		@param	ppModule		指向一个IModule数组的开始位置，buf的内容由实现者填充
	//----------------------------------------------------------------------------------------
	virtual void ReleaseModulePoint(uint32 nCount, IModule* pModule ) PURE;
};


 //----------------------------------------------------------------------------------------
//名称: IModuleManager
//描述: 模块总线结构，用以对模块进行管理以及各个模块之间进行交互。
//----------------------------------------------------------------------------------------
interface IModuleManager
{
	//----------------------------------------------------------------------------------------
	//名称: PushEvent
	//描述: Push给定的事件
	//参数: 
	//		@param	evt			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual BOOL PushEvent(const Event& evt ) PURE;

	//----------------------------------------------------------------------------------------
	//名称: PushMessage
	//描述: 需要广播的消息
	//参数: 
	//		@param	msg			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual BOOL PushMessage(const Message& msg) PURE;

	//----------------------------------------------------------------------------------------
	//名称: CallService
	//描述: 直接调用另外一个模块的方法
	//参数: 
	//		@param	msg			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual int32 CallService(const ServiceValue lServiceValue ,param rparam ) PURE;
};