#pragma once

#include "SndaBase.h"
#include "ModuleImpl.h"
#include "MainFrameWnd.h"

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}


class CMainFrameWnd;
class MainFrameModule : public ModuleImpl
{
	DECLEAR_EVENT_MAP(MainFrameModule)
		DECLEAR_MESSAGE_MAP(MainFrameModule)
		DECLEAR_SERVICE_MAP(MainFrameModule)

public:
	MainFrameModule();
	~MainFrameModule();

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
	//		@param	lServiceValue		参数1
	//		@param	rparam			参数2
	//----------------------------------------------------------------------------------------
	int32 CallDirect(const ServiceValue lServiceValue, param rparam);


	// Event处理函数
protected:

	void	OnEvent_OpenMainDlg(Event* pEvent);	// 响应EVENT_VALUE_MAINFRAME_OPEN事件，打开主界面	
	void	OnEvent_CloseMainDlg(Event* pEvent);	// 响应EVENT_VALUE_MAINFRAME_CLOSE事件，关闭主界面
	void	OnEvent_ShowMainDlg(Event* pEvent);		// 响应EVENT_VALUE_MAINFRAME_SHOW事件，显示主界面										
	void	OnEvent_HideMainDlg(Event* pEvent);		// 响应EVENT_VALUE_MAINFRAME_HIDE事件，隐藏主界面	

	void	OnEvent_FavoriteIconArrive(Event* pEvent);		//	网址对应的ICON图标达到
	void	OnEvent_AddUrl(Event*	pEvent);						//	增加一个新的URL
	void	OnEvent_DeleteFavorite(Event* pEvent);			//	删除给定的收藏记录
	void	OnEvent_OpenUrl(Event* pEvent);			//	打开给定的收藏网址
	void	OnEvent_CopyUrl(Event* pEvent);			//	拷贝网址
	void	OnEvent_DeleteFavoriteFold(Event* pEvent);	//	删除收藏夹
	void	OnEvent_SelectTreeListItem(Event* pEvent);	//	选中树结点中的某一个

	// Message处理函数
protected:

	void	OnMessage_CycleTrigged(Message* pMessage);
	void	OnMessage_Show(Message* pMessage);
	void	OnMessage_PreExit(Message* pMessage);	// 程序退出的广播消息，主界面处理该消息，为退出做准备	
	void	OnMessage_FavoriteLoaded(Message* pMessage);		//	收藏夹已经合并结束
	void	OnMessage_PlugInLoaded(Message* pMessage);

protected:

	int32 OnService_GetMainWnd(ServiceValue lServiceValue, param	lParam);

private:
	CMainFrameWnd* m_pMainFrame;
};

class MainFrameModuleFactory : public ModuleFactoryImpl<MainFrameModule>{};

namespace mainframe
{
	extern MainFrameModule*	g_MainFrameModule;
	extern MainFrameModuleFactory*	g_MainFrameModuleFactory;
}