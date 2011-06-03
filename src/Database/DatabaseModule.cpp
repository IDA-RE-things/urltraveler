#include "stdafx.h"
#include "DatabaseModule.h"
#include "Database.h"
#include "PathHelper.h"
#include "DatabaseDefine.h"

HMODULE	g_hModule = NULL;


using namespace database;

namespace database
{
	DatabaseModule*	g_DatabaseModule = NULL;
	DatabaseModuleFactory*	g_DatabaseModuleFactory = NULL;
}

// �������ʵ��
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
END_SERVICE_MAP()

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* DatabaseModule::GetModuleName() 
{
	return L"DatabaseModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const DatabaseModule::GetModuleId()
{
	return MODULE_ID_DATABASE;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void DatabaseModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//����: ProcessMessage
//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
//����: 
//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
//----------------------------------------------------------------------------------------
void DatabaseModule::ProcessMessage(const Message& msg) 
{
}

//----------------------------------------------------------------------------------------
//����: CallDirect
//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
//			����������Ҫͨ�����ߡ�
//����: 
//		@param	lparam			����1
//		@param	rparam			����2
//----------------------------------------------------------------------------------------
int32 DatabaseModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}


CppSQLite3DB& DatabaseModule::OpenFavoriteDB()
{
	if (m_dbFavorite.IsOpen())
	{
		return m_dbFavorite;
	}
	else
	{
		std::wstring strFileName = PathHelper::GetAppDataDir();
		strFileName += L"\\urltraveler\\";
		PathHelper::CreateMultipleDirectory(strFileName);

		strFileName += L"\\fav.db";
		m_dbFavorite.open(strFileName.c_str(),"");
		return m_dbFavorite;
	}
}

bool DatabaseModule::PrepareFavoriteTable()
{
	bool bResult;

	bResult = m_dbFavorite.tableExists(FAVORITE_TABLE);
	if(bResult == false)  //˵�����ݿ���û�����б�
	{
		m_dbFavorite.execDML(FAVORITE_CREATE_TABLE);
	}

	return bResult;
}


void DatabaseModule::OnEvent_SaveFavoriteIcon(Event* evt)
{
	bool bResult = false;

	try
	{
		CppSQLite3DB &db = OpenFavoriteDB();
		if(!db.IsOpen())
			return;

		bResult = PrepareFavoriteTable() ;
		if (bResult != true)
			return;

		Database_FavIconSaveEvent* pE = (Database_FavIconSaveEvent*)evt->m_pstExtraInfo;
		wstring wstrUrl = pE->szFavoriteUrl;

		// ����ǰ��URL��HICON�������ݿ�

	}
	catch (CppSQLite3Exception& e)
	{
		CppSQLite3DB &db = OpenFavoriteDB();
		db.close();
		bResult =  false;
	}
	catch(...)
	{
		bResult =  false;
	}
}
