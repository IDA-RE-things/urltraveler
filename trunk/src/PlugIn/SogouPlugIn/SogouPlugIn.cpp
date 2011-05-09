#include "stdafx.h"
#include "SogouPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "PathHelper.h"
#include "Decoder.h"
#include "CppSQLite3.h"

using namespace sogouplugin;

#pragma comment(lib, "shlwapi.lib")

namespace sogouplugin
{
	CSogouPlugIn*	g_SogouPlugIn	=	NULL;
}

// 导出借口实现
IPlugIn*	GetPlugIn()
{
	if( g_SogouPlugIn == NULL)
	{
		g_SogouPlugIn = new CSogouPlugIn();
		ASSERT( g_SogouPlugIn != NULL);
	}

	return g_SogouPlugIn;
}

void	ReleasePlugIn( IPlugIn* p)
{
	ASSERT( g_SogouPlugIn == p);
	if( g_SogouPlugIn  != NULL)
	{
		delete g_SogouPlugIn;
		g_SogouPlugIn = NULL;
	}
}

CSogouPlugIn::CSogouPlugIn()
{
	std::string strDecodeContent;

	typedef struct _winFileMem 
	{
		unsigned char *pMemPointer;
		unsigned long  ulMemSize;
	}winFileMem;

	if (decode(StringHelper::UnicodeToANSI(GetFavoriteDataPath()), strDecodeContent) == 0)
	{
		CppSQLite3DB  m_SqliteDatabase;

		winFileMem *pstFileMem = (winFileMem *)malloc(sizeof(winFileMem));

		pstFileMem->ulMemSize = strDecodeContent.length();
		pstFileMem->pMemPointer = (unsigned char *)malloc(strDecodeContent.length());
		memcpy(pstFileMem->pMemPointer, strDecodeContent.c_str(), pstFileMem->ulMemSize);

		m_SqliteDatabase.openmem((char *)pstFileMem, "");

		//CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from dbInfo");

		//int dbVer = Query.getIntField("value");

		for (int i = 0; i < 100; i++)
		{
			char szInsert[MAX_PATH] = {0};

			sprintf(szInsert, "insert into dbInfo(id, value, reserved) values('hello world%d', '4', 0)", i);
			m_SqliteDatabase.execDML(szInsert);
		}

		m_SqliteDatabase.close();

		FILE *fOut = fopen("b.db", "wb");

		fwrite(pstFileMem->pMemPointer, 1, pstFileMem->ulMemSize, fOut);

		fclose(fOut);
	}
}

CSogouPlugIn::~CSogouPlugIn()
{

}

BOOL CSogouPlugIn::IsWorked()
{
	return GetPluginVersion() != 0;
}

int32 CSogouPlugIn::GetPluginVersion()
{
	wchar_t szVersion[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szVersion); 
	int32   nVersion = 0;

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\SogouExplorer",
		L"DisplayVersion", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szVersion, 
		&dwSize))
	{
		std::string strVersion = StringHelper::UnicodeToANSI(szVersion);
		std::vector<std::string> vecResutl = StringHelper::Split(strVersion, '.');

		if (vecResutl.size() < 4)
		{
			return 0;
		}

		BYTE nMajaor = atol(vecResutl[0].c_str());
		BYTE nMinor  = atol(vecResutl[1].c_str());
		WORD nBuilderNum = atol(vecResutl[3].c_str());

		nVersion = MAKELONG(MAKEWORD(nMajaor, nMinor), nBuilderNum);

		return nVersion ;
	}

	return 0;
}

//----------------------------------------------------------------------------------------
//名称: GetBrowserName
//描述: 获取该插件对应的浏览器的名称和版本
//----------------------------------------------------------------------------------------
void CSogouPlugIn::GetSupportBrowserVersion(wchar_t** ppBrowserVersion, int& nVersionNumber)
{
}

const wchar_t* CSogouPlugIn::GetBrowserName()
{
	return L"SogouExplorer";
}

//----------------------------------------------------------------------------------------
//名称: GetBrowserIcon
//描述: 获取该插件对应的浏览器的图标
//----------------------------------------------------------------------------------------
HICON CSogouPlugIn::GetBrowserIcon() 
{
	return NULL;
}

const wchar_t* CSogouPlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\SogouExplorer",
		L"DisplayIcon", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		if (PathRemoveFileSpec(szPath))
		{
			MessageBox(NULL, szPath, szPath, NULL);
			return szPath;
		}
	}

	return NULL;
}

const wchar_t* CSogouPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\SogouExplorer\\Favorite2.dat";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _tcsdup(strPath.c_str());
}

const wchar_t* CSogouPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\SogouExplorer\\HistoryUrl.db";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _tcsdup(strPath.c_str());
}

BOOL CSogouPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	return TRUE;
}

BOOL CSogouPlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	return TRUE;
}

BOOL CSogouPlugIn::ImportFavoriteData( FAVORITELINEDATA stData )
{
	return TRUE;
}
