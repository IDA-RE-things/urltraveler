#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "PlugIn.h"
#include <vector>
#include <map>
#include <list>

using namespace std;


extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class DataCenterModule : public ModuleImpl
{
	DECLEAR_SERVICE_MAP(DataCenterModule)
	DECLEAR_EVENT_MAP(DataCenterModule)

public:
	DataCenterModule();
	~DataCenterModule();

	//----------------------------------------------------------------------------------------
	//名称: Load
	//描述: 主程序通过该方法对模块进行加载
	//参数: 
	//		@param	pManager			主模块总线的指针	
	//返回: 
	//		如果加载成功，返回TRUE，否则返回FALSE
	//----------------------------------------------------------------------------------------
	virtual BOOL Load(IModuleManager* pManager);
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
	virtual const wchar_t* GetModuleName();

 	//----------------------------------------------------------------------------------------
	//名称: GetModuleId
	//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
	//返回: 
	//		返回该模块的唯一的ID
	//----------------------------------------------------------------------------------------
	virtual uint32 const GetModuleId();

 	//----------------------------------------------------------------------------------------
	//名称: ProcessEvent
	//描述: 该模块可以处理的所有的点对点事件
	//参数: 
	//		@param	evt			需要处理的事件
	//----------------------------------------------------------------------------------------
	virtual void ProcessEvent(const Event& evt);

 	//----------------------------------------------------------------------------------------
	//名称: ProcessMessage
	//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
	//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
	//参数: 
	//		@param	msg			需要处理的广播消息
	//----------------------------------------------------------------------------------------
	virtual void ProcessMessage(const Message& msg);

 	//----------------------------------------------------------------------------------------
	//名称: CallDirect
	//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
	//			方法而不需要通过总线。
	//参数: 
	//		@param	lparam			参数1
	//		@param	rparam			参数2
	//----------------------------------------------------------------------------------------
	virtual int32 CallDirect(const ServiceValue lServiceValue, param wparam);

protected:

	void	OnEvent_FavoriteIconArrive(Event* pEvent);		//	网址对应的ICON图标达到
	void	OnEvent_AddFavorite(Event* pEvent);			//	增加一个收藏记录
	void	OnEvent_DeleteFavorite(Event* pEvent);			//	删除特定的URL
	void	OnEvent_AddFavoriteFolder(Event* pEvent);		//	删除指定的收藏夹
	void	OnEvent_DeleteFavoriteFolder(Event* pEvent);		//	删除指定的收藏夹
	void	OnEvent_SetAutoUpdate(Event* pEvent);			//	设置自动更新不提示

protected:

	void	OnService_GetFavoriteVector(ServiceValue lServiceValue, param	lParam);
	void	OnService_GetFavoriteData(ServiceValue lServiceValue, param	lParam);
	void	OnService_GetFavoriteIcon(ServiceValue lServiceValue, param lParam);
	void	OnService_CheckExistSubFolder(ServiceValue	lServiceValue, param	 lParam);
	void	OnService_GetSubFolderId(ServiceValue lServiceValue, param lParam);
	void	OnService_GetAutoUpdate(ServiceValue lServiceValue, param lParam);
	void	OnService_ReArrangeFavorite(ServiceValue lServiceValue, param lParam);
	void	OnService_GetFavoriteNumAtFold(ServiceValue lServiceValue, param lParam);
	void	OnService_InitFavoriteData(ServiceValue lServiceValue, param lParam);
	void	OnService_ClearData(ServiceValue lServiceValue, param lParam);

private:

	// 删除指定收藏夹
	void	DeleteFavoriteFold(int nId);
	void	GetSubFolderId(std::vector<int>* pvId, int nId);

protected:
	
	//	用以保存所有的收藏夹数据
	std::vector<FAVORITELINEDATA*>	m_vFavoriteLineData;

	//	用以保存所有的历史数据
	std::vector<HISTORYLINEDATA>	m_vHistoryLineData;

	//	保存Domain与对应的收藏夹图标之间的关系
	std::map<wstring, HICON>	m_mapDomain2Icon;
	HICON                       m_hDefaultIcon;

	// 保存一些Key Value设置
	//	是否自动更新
	BOOL	m_bAutoUpdate;		
};

class DataCenterModuleFactory : public ModuleFactoryImpl<DataCenterModule>{};

namespace datacenter
{
	extern DataCenterModule*	g_DataCenterModule;
	extern DataCenterModuleFactory*	g_DataCenterModuleFactory;
}