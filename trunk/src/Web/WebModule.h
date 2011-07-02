#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include <vector>
#include "WebDefine.h"
#include <map>

using namespace std;
using namespace web;


extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class CRequestManager;
class WebModule : public ModuleImpl
{
	DECLEAR_SERVICE_MAP(WebModule)
	DECLEAR_EVENT_MAP(WebModule)
	DECLEAR_MESSAGE_MAP(WebModule)

public:
	WebModule();
	~WebModule();

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

protected:

	void OnEvent_GetFavoriteIcon(Event* pEvent);
	void OnEvent_OpenUrlTraveler(Event* pEvent);
	void OnEvent_CloseUrlTraveler(Event* pEvent);
	void OnEvent_LoginInUrlTraveler(Event* pEvent);
	void OnEvent_LoginOutUrlTraveler(Event* pEvent);
	void OnEvent_ReportUserInfo(Event* pEvent);
	void	OnEvent_CheckUpdateConfig(Event* pEvent);
	void	OnEvent_DownloadUpdateFile(Event* pEvent);

protected:

	int32 OnService_GetFavoriteIcon(ServiceValue lService, param wparam);

protected:
	void OnMessage_CycleTrigged(Message* pMessage);

private:

	void	ProcessGetResponse();
	void	ProcessSendUrl( );

public:

	PROXYDATA			m_ProxyData;

	CRequestManager*	m_pRequestManager;				// 对HTTP请求的管理，负责向后台的Web服务器发送请求，

	Cookie	m_unCookie;
	std::map<Cookie, Event*> m_mapCookie2Event;				// 支持Event事件

	std::map<uint32, uint32>	m_mapSeqNo2ModuleId;	//	请求序列号与模块ID的对应关系
};

class WebModuleFactory : public ModuleFactoryImpl<WebModule>{};

namespace web
{
	extern WebModule*	g_WebModule;
	extern WebModuleFactory*	g_WebModuleFactory;
}