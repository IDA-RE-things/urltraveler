#pragma once

#include "SndaBase.h"
#include "Module.h"
#include "ModuleImpl.h"
#include "PlugIn.h"
#include <vector>
#include <CppSQLite3/CppSQLite3.h>
#include "DatabaseDefine.h"
#include "Database.h"

using namespace std;

extern "C" 
{
	DLLEXPORT IModuleFactory*	GetModuleFactory();
	DLLEXPORT void	ReleaseModuleFactory( IModuleFactory*);
}

class DatabaseModule : public ModuleImpl
{
	DECLEAR_SERVICE_MAP(DatabaseModule)
	DECLEAR_EVENT_MAP(DatabaseModule)

public:
	DatabaseModule();
	~DatabaseModule();

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

	void OnEvent_SaveFavoriteIcon(Event* evt);

protected:

	int32 OnService_GetFavoriteIcon(ServiceValue lService, param wparam);


protected:

	void	OpenFavoriteDB();	
	bool	PrepareFavoriteTable();
	HICON	GetFavoriteIcon( wstring wstrFavoriteUrl);

private:

	/** 将Sqlite3的encode的Blob指针转换成CBinaryBuffer结构
	@return void.
	*/
	void SQLite3EncodeBlob2Buffer(
		const void * ptrBlobBuffer, 
		CBinaryBuffer &binaryBuffer
		);

protected:

	CppSQLite3DB	m_dbFavorite;			//	收藏夹相关的数据库内容
};

class DatabaseModuleFactory : public ModuleFactoryImpl<DatabaseModule>{};

namespace database
{
	extern DatabaseModule*	g_DatabaseModule;
	extern DatabaseModuleFactory*	g_DatabaseModuleFactory;
}