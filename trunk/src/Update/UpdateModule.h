#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "PlugIn.h"
#include <vector>
#include "UpdateDefine.h"
#include "UpdateWnd.h"

using namespace std;

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

// 需要更新的文件信息
typedef struct UpdateFileInfo
{
	wstring	strFileName;		//	文件名
	wstring	strMd5;		
	wstring	strDownloadUrl;
	wstring	strTempSavePath;	//	下载之后的临时存放目录

}UPDATEFILEINFO, *PUPDATEFILEINFO;

class UpdateModule : public ModuleImpl
{
	DECLEAR_SERVICE_MAP(UpdateModule)
	DECLEAR_EVENT_MAP(UpdateModule)
	DECLEAR_MESSAGE_MAP(UpdateModule)

public:
	UpdateModule();
	~UpdateModule();

	//----------------------------------------------------------------------------------------
	//名称: Load
	//描述: 主程序通过该方法对模块进行加载
	//参数: 
	//		@param	pManager			主模块总线的指针	
	//返回: 
	//		如果加载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Load(IModuleManager* pManager) ;

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

	void	OnEvent_CheckUpdateInfo(Event* pEvent);
	void	OnEvent_UpdateInfoArrive(Event* pEvent);
	void	OnEvent_UpdateFileDownloaded(Event* pEvent);

protected:
	void OnMessage_CycleTrigged(Message* pMessage);

private:

	BOOL	IsHaveUpdatePackage();
	void		ProcessUpdateConfig();


protected:
	wstring	m_strUpdatePath;	//	下载的文件的保存路径
	wstring	m_strUpdateXml;	//	更新的xml文件内容
	wstring	m_strUpdateFileName;	//	更新的文件名称
	int		m_nDownloadSeqNo;
	CUpdateWnd*	m_pUpdateWnd;		
};

class UpdateModuleFactory : public ModuleFactoryImpl<UpdateModule>{};

namespace update
{
	extern UpdateModule*	g_UpdateModule;
	extern UpdateModuleFactory*	g_UpdateModuleFactory;
}