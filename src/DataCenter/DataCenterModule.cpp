#include "stdafx.h"
#include "DataCenterModule.h"

using namespace datacenter;

namespace datacenter
{
	DataCenterModule*	g_DataCenterModule = NULL;
	CDataCenterModuleFactory*	g_DataCenterModuleFactory = NULL;
}

// 导出借口实现
IModuleFactory*	GetModuleFactory()
{
	if( g_DataCenterModuleFactory == NULL)
	{
		g_DataCenterModuleFactory = new CDataCenterModuleFactory();
		g_DataCenterModuleFactory->QueryModulePoint(1, (IModule*&)g_DataCenterModule);
		
		ASSERT( g_DataCenterModule != NULL);
	}

	return g_DataCenterModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_DataCenterModuleFactory == p);
	if( g_DataCenterModuleFactory  != NULL)
	{
		delete g_DataCenterModuleFactory;
		g_DataCenterModuleFactory = NULL;
	}
}

DataCenterModule::DataCenterModule()
{

}

DataCenterModule::~DataCenterModule()
{

}

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* DataCenterModule::GetModuleName() 
{
	return L"DataCenterModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const DataCenterModule::GetModuleId()
{
	return MODULE_ID_DATACENTER;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void DataCenterModule::ProcessEvent(const Event& evt)
{
}

//----------------------------------------------------------------------------------------
//名称: ProcessMessage
//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
//参数: 
//		@param	msg			需要处理的广播消息
//----------------------------------------------------------------------------------------
void DataCenterModule::ProcessMessage(const Message& msg) 
{
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lparam			参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 DataCenterModule::CallDirect(const param lparam, param wparam) 
{
	return -1;
}

//----------------------------------------------------------------------------------------
//名称: PaybackExtraInfo
//描述: 某个模块如果有自定义的复杂的数据需要传给其余的模块，那么它可以构造一个ExtraInfo结构
//	在其余的模块使用完毕后，该结构必须被释放，否则会造成内存泄露。释放必须由模块自身完成。
//	某个模块都必须提供一个PaybackExtraInfo接口，释放自己的自定义的数据类型
//参数: 
//		@param	valudId			对应的pExtraInfo的值，内部根据该值进行对应的释放，该值只有模块自己理解
//		@param	pExtraInfo	需要释放的ExtraInfo数据
//----------------------------------------------------------------------------------------
void DataCenterModule::PaybackExtraInfo(uint32 valudId, void* pExtraInfo)
{
	return;
}