#include "stdafx.h"
#include "DatabaseModule.h"
#include "Database.h"
#include "PathHelper.h"
#include "DatabaseDefine.h"
#include "StringHelper.h"
#include <string>
#include "ImageHelper.h"

HMODULE	g_hModule = NULL;

using namespace std;
using namespace database;

namespace database
{
	DatabaseModule*	g_DatabaseModule = NULL;
	DatabaseModuleFactory*	g_DatabaseModuleFactory = NULL;
}

// 导出借口实现
IModuleFactory*	GetModuleFactory()
{
	if( g_DatabaseModuleFactory == NULL)
	{
		g_DatabaseModuleFactory = new DatabaseModuleFactory();
		g_DatabaseModuleFactory->QueryModulePoint(1, (IModule*&)g_DatabaseModule);
		
		ASSERT( g_DatabaseModule != NULL);
	}

	return g_DatabaseModuleFactory;
}

void	ReleaseModuleFactory( IModuleFactory* p)
{
	ASSERT( g_DatabaseModuleFactory == p);
	if( g_DatabaseModuleFactory  != NULL)
	{
		delete g_DatabaseModuleFactory;
		g_DatabaseModuleFactory = NULL;
	}
}

DatabaseModule::DatabaseModule()
{

}

DatabaseModule::~DatabaseModule()
{

}

BEGIN_EVENT_MAP(DatabaseModule)
	ON_EVENT(EVENT_VALUE_DATABASE_FAVICON_SAVE, OnEvent_SaveFavoriteIcon)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(DatabaseModule)
	ON_SERVICE(SERVICE_VALUE_DATABASE_FAVICON_LOAD, OnService_GetFavoriteIcon)
END_SERVICE_MAP()

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* DatabaseModule::GetModuleName() 
{
	return L"DatabaseModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const DatabaseModule::GetModuleId()
{
	return MODULE_ID_DATABASE;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void DatabaseModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//名称: ProcessMessage
//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
//参数: 
//		@param	msg			需要处理的广播消息
//----------------------------------------------------------------------------------------
void DatabaseModule::ProcessMessage(const Message& msg) 
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
int32 DatabaseModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void DatabaseModule::SQLite3EncodeBlob2Buffer(
	const void * ptrBlobBuffer, 
	CBinaryBuffer &binaryBuffer
	)
{
	PAssertNotNull_Return(ptrBlobBuffer);

	/**重新将缓冲区放到CppSQLite3Binary中管理,
	由于getBlobField返回的为字符串形式的编码后的数据为原始数据经过编码后的形式
	所以为设置Encoded数据进入blob对象中
	*/
	CppSQLite3Binary blob;
	blob.setEncoded((const unsigned char*)ptrBlobBuffer);

	UINT uiMaxBufferLen = sizeof(binaryBuffer.m_binaryBuffer);
	UINT uiBinaryBufferLen = blob.getBinaryLength();

	if (uiBinaryBufferLen <= uiMaxBufferLen)
	{
		binaryBuffer.m_uiBufferLen = uiBinaryBufferLen;
	}
	else
	{
		binaryBuffer.m_uiBufferLen = uiMaxBufferLen;
	}
	memcpy(binaryBuffer.m_binaryBuffer, blob.getBinary(), binaryBuffer.m_uiBufferLen);
}


void DatabaseModule::OpenFavoriteDB()
{
	if (m_dbFavorite.IsOpen())
	{
		return;
	}
	else
	{
		std::wstring strFileName = PathHelper::GetAppDataDir();
		strFileName += L"\\urltraveler\\";
		PathHelper::CreateMultipleDirectory(strFileName);

		strFileName += L"\\fav.db";
		m_dbFavorite.open(strFileName.c_str(),"");
	}
}

bool DatabaseModule::PrepareFavoriteTable()
{
	bool bResult;

	bResult = m_dbFavorite.tableExists(FAVORITE_TABLE);
	if(bResult == false)  //说明数据库中没有组列表
	{
		m_dbFavorite.execDML(FAVORITE_CREATE_TABLE);
	}

	return bResult;
}

HICON	DatabaseModule::GetFavoriteIcon( wstring wstrFavoriteUrl)
{
	bool bResult = false;
	try
	{
		OpenFavoriteDB();
		if(!m_dbFavorite.IsOpen())
			return NULL;

		bResult = PrepareFavoriteTable() ;
		if (bResult != true)
			return NULL;

		// 对表格进行查询
		string	strSql = "SELECT data FROM fav_icon";
		strSql += " WHERE url='" + StringHelper::UnicodeToANSI(wstrFavoriteUrl) + string("'");
		
		CppSQLite3Query query = m_dbFavorite.execQuery(strSql.c_str());

		// 找到对应的Icon图标
		if (!query.eof())
		{
			int nDataLen = 0;
			const unsigned char* pData = query.getBlobField("data", nDataLen);
			if( nDataLen == 0)
			{
				query.finalize();
				return NULL;
			}

			CBinaryBuffer binaryBuffer;
			SQLite3EncodeBlob2Buffer(pData, binaryBuffer);

			HICON hIcon = ImageHelper::CreateIconFromBuffer((LPBYTE) binaryBuffer.m_binaryBuffer, 
				binaryBuffer.m_uiBufferLen, 16);
			query.finalize();
			return hIcon;
		}
		// 没有找到对应的 ICON图标
		query.finalize();
		return NULL;
	}
	catch (CppSQLite3Exception& e)
	{
		m_dbFavorite.close();
		return NULL;
	}
	catch(...)
	{
		return NULL;
	}
}

void DatabaseModule::OnEvent_SaveFavoriteIcon(Event* evt)
{
	bool bResult = false;
	try
	{
		OpenFavoriteDB();
		if(!m_dbFavorite.IsOpen())
			return;

		bResult = PrepareFavoriteTable() ;
		if (bResult != true)
			return;

		Database_FavIconSaveEvent* pE = (Database_FavIconSaveEvent*)evt->m_pstExtraInfo;
		wstring wstrUrl = pE->szFavoriteUrl;

		// 将当前的URL和HICON插入数据库
		string strSql = "INSERT INTO fav_icon(url, data) values('" + StringHelper::UnicodeToANSI(wstrUrl) + "',";
		strSql += " ? )";

		CppSQLite3Statement stmt = m_dbFavorite.compileStatement(strSql.c_str());

		///绑定编译sql语句中的Blob参数
		CppSQLite3Binary blob;
		const unsigned char *ptrBuffer = (const unsigned char *)pE->pIconData;

		///设置Blob对象的缓冲区和长度
 		blob.setBinary(ptrBuffer, pE->nIconDataLen);
		stmt.bind(1, (const char *)blob.getEncoded());
		stmt.execDML();
		stmt.reset();
	}
	catch (CppSQLite3Exception& e)
	{
		m_dbFavorite.close();
	}
	catch(...)
	{
	}
}

int32 DatabaseModule:: OnService_GetFavoriteIcon(ServiceValue lService, param lparam)
{
	Database_GetFavoriteIconService* pGetFavoriteIconService = (Database_GetFavoriteIconService*)lparam;
	ASSERT( pGetFavoriteIconService != NULL);

	wstring wstrUrl = pGetFavoriteIconService->szFavoriteUrl;
	HICON hIcon = GetFavoriteIcon(wstrUrl);
	pGetFavoriteIconService->hcon = hIcon;

	return -1;
}
