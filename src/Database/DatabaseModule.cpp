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
	ON_SERVICE(SERVICE_VALUE_DATABASE_FAVICON_LOAD, OnService_GetFavoriteIcon)
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

void DatabaseModule::SQLite3EncodeBlob2Buffer(
	const void * ptrBlobBuffer, 
	CBinaryBuffer &binaryBuffer
	)
{
	PAssertNotNull_Return(ptrBlobBuffer);

	/**���½��������ŵ�CppSQLite3Binary�й���,
	����getBlobField���ص�Ϊ�ַ�����ʽ�ı���������Ϊԭʼ���ݾ�����������ʽ
	����Ϊ����Encoded���ݽ���blob������
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
	if(bResult == false)  //˵�����ݿ���û�����б�
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

		// �Ա����в�ѯ
		string	strSql = "SELECT data FROM fav_icon";
		strSql += " WHERE url='" + StringHelper::UnicodeToANSI(wstrFavoriteUrl) + string("'");
		
		CppSQLite3Query query = m_dbFavorite.execQuery(strSql.c_str());

		// �ҵ���Ӧ��Iconͼ��
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
		// û���ҵ���Ӧ�� ICONͼ��
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

		// ����ǰ��URL��HICON�������ݿ�
		string strSql = "INSERT INTO fav_icon(url, data) values('" + StringHelper::UnicodeToANSI(wstrUrl) + "',";
		strSql += " ? )";

		CppSQLite3Statement stmt = m_dbFavorite.compileStatement(strSql.c_str());

		///�󶨱���sql����е�Blob����
		CppSQLite3Binary blob;
		const unsigned char *ptrBuffer = (const unsigned char *)pE->pIconData;

		///����Blob����Ļ������ͳ���
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
