#include "stdafx.h"
#include "Maxthon2PlugIn.h"
#include <string>
#include "Decoder.h"
#include "StringHelper.h"
#include <shlwapi.h>
#include "PathHelper.h"
#include "CppSQLite3.h"
#include "CRCHash.h"
#include "MD5Checksum.h"

#include "MaxthonPlugInFactory.h"

#pragma comment(lib, "shlwapi.lib")


Maxthon2PlugIn::Maxthon2PlugIn()
{

}

Maxthon2PlugIn::~Maxthon2PlugIn()
{

}

BOOL Maxthon2PlugIn::Load()
{
	std::string strDecodeContent;

	if (decode(StringHelper::UnicodeToANSI(GetFavoriteDataPath()), strDecodeContent) == 0)
	{
		m_pMemFavoriteDB = (winFileMem *)malloc(sizeof(winFileMem));

		m_pMemFavoriteDB->ulMemSize = strDecodeContent.length();
		m_pMemFavoriteDB->pMemPointer = (unsigned char *)malloc(strDecodeContent.length());
		memcpy(m_pMemFavoriteDB->pMemPointer, strDecodeContent.c_str(), m_pMemFavoriteDB->ulMemSize);

		return TRUE;
	}

	return FALSE;

}

BOOL Maxthon2PlugIn::UnLoad()
{
	if (m_pMemFavoriteDB)
	{
		FILE *fOut = fopen("maxthon_b.db", "wb");
		fwrite(m_pMemFavoriteDB->pMemPointer, 1, m_pMemFavoriteDB->ulMemSize, fOut);
		fclose(fOut);
		free(m_pMemFavoriteDB->pMemPointer);
		free(m_pMemFavoriteDB);
		m_pMemFavoriteDB = NULL;
		return TRUE;
	}

	return FALSE;

}

BOOL Maxthon2PlugIn::IsWorked()
{
	return FALSE;
}

int32 Maxthon2PlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* Maxthon2PlugIn::GetBrowserName()
{
	return L"傲游2";
}

wchar_t* Maxthon2PlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Maxthon2",
		L"InstallLocation", 
		SRRF_RT_REG_SZ, 
		NULL, 
		szPath, 
		&dwSize))
	{
		if (PathRemoveFileSpec(szPath))
		{
			wcscat(szPath, L"\\Maxthon.exe");
			return _wcsdup(szPath);
		}
	}

	return NULL;
}

wchar_t* Maxthon2PlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Maxthon3\\Users\\guest\\Favorite\\Favorite.dat";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _wcsdup(strPath.c_str());
}

wchar_t* Maxthon2PlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\Maxthon3\\Users\\guest\\History\\History.dat";

	//需要复制一份,不然strPath被析构时,返回野指针,由调用者进行释放,否则会造成内存泄漏

	return _wcsdup(strPath.c_str());
}

BOOL Maxthon2PlugIn::ExportFavoriteData( PFAVORITELINEDATA* ppData, int32& nDataNum )
{
	CCRCHash ojbCrcHash;

	if (ppData == NULL || *ppData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from MyFavNodes where parent_id <> ''");
		int i = 0;
		unsigned char szParentNode[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		while(!Query.eof() && i < nDataNum)
		{
			const unsigned char *pTemp = NULL;
			int nBlobLen = 0;
			pTemp = Query.getBlobField("parent_id", nBlobLen);

			if (memcmp(pTemp, szParentNode, 16) == 0)
			{
				ppData[i]->nPid = 0;
			}
			else
			{
				ojbCrcHash.GetHash((BYTE *)pTemp, nBlobLen, (BYTE *)&ppData[i]->nPid, sizeof(uint32));
			}

			pTemp = Query.getBlobField("id", nBlobLen);

			ojbCrcHash.GetHash((BYTE *)pTemp, nBlobLen, (BYTE *)&ppData[i]->nId, sizeof(uint32));

			pTemp = Query.getBlobField("title", nBlobLen);

			if (nBlobLen != 0)
			{
				memcpy(ppData[i]->szTitle, pTemp, nBlobLen);

				ojbCrcHash.GetHash((BYTE *)ppData[i]->szTitle, wcslen(ppData[i]->szTitle) * sizeof(wchar_t), 
					(BYTE *)&ppData[i]->nHashId, sizeof(uint32));
			}
			else
			{
				ppData[i]->szTitle[0] = 0;
			}

			pTemp = Query.getBlobField("url", nBlobLen);

			if (nBlobLen != 0)
			{
				memcpy(ppData[i]->szUrl, pTemp, nBlobLen);
			}
			else
			{
				ppData[i]->szUrl[0] = 0;
			}

			ppData[i]->bFolder = Query.getIntField("type", 1) == IT_FOLDER ? true : false;

			ppData[i]->nAddTimes = Query.getIntField("add_date", 0);
			ppData[i]->nClickTimes = Query.getIntField("visit_count", 0);
			ppData[i]->nLastModifyTime = Query.getIntField("last_modified", 0);
			ppData[i]->bDelete = false;
			ppData[i]->nCatId = 0;
			ppData[i]->nOrder = Query.getIntField("norder", 0);

			Query.nextRow();
			i++;
		}

		nDataNum = i;

		for (int i = 0; i < nDataNum; i++)
		{
			//如果该结点的nId不是数组下标+1,则需要修正
			if ((ppData[i]->nId != i + 1 + ID_VALUE_MAXTHON2_BEGIN))
			{
				//扫描所有以该结点为父结点的结点，并修正这些结点的nPid
				for (int j = 0; j < nDataNum; j++)
				{
					if (ppData[j]->nPid == ppData[i]->nId)
					{
						ppData[j]->nPid = i + 1 + ID_VALUE_MAXTHON2_BEGIN;
					}
				}

				ppData[i]->nId = i + 1 + ID_VALUE_MAXTHON2_BEGIN;
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL Maxthon2PlugIn::ImportFavoriteData( PFAVORITELINEDATA* ppData, int32 nDataNum )
{
	if (ppData == NULL || *ppData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	#define MAX_BUFFER_LEN	4096

	CppSQLite3DB  m_SqliteDatabase;
	wchar_t szInsert[MAX_BUFFER_LEN] = {0};
	wchar_t szDelete[MAX_BUFFER_LEN] = {0};

	if (m_pMemFavoriteDB != NULL)
	{
		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");

		int i = 0;

		m_SqliteDatabase.execDML(StringHelper::UnicodeToUtf8(L"delete from MyFavNodes where parent_id <> ''").c_str());

		m_SqliteDatabase.execDML("begin transaction");

		for (int i = 0; i < nDataNum; i++)
		{
			if (ppData[i]->bDelete == true)
			{
				continue;
			}

			swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into MyFavNodes"
				L"(id,parent_id,type,title,url,most_fav,visit_count,norder,add_date,shortcut)"
				L" values(?,?,%d,?,?,0,0,%d,%d,0)",
				ppData[i]->bFolder == true ? IT_FOLDER : IT_URL,
				ppData[i]->nOrder,
				(int32)ppData[i]->nAddTimes);

			CppSQLite3Statement sqliteStatement = m_SqliteDatabase.compileStatement(StringHelper::UnicodeToANSI(szInsert).c_str());

			std::string strTemp1 = StringHelper::StringToHex(StringHelper::ANSIToUnicode(CMD5Checksum::GetMD5((BYTE *)&ppData[i]->nId, sizeof(int32))));
			sqliteStatement.bind(1, (unsigned char *)strTemp1.c_str(), strTemp1.length());

			if (ppData[i]->nPid == 0)
			{
				unsigned char szParentNode[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

				sqliteStatement.bind(2, szParentNode, 16);
			}
			else
			{
				strTemp1 = StringHelper::StringToHex(StringHelper::ANSIToUnicode(CMD5Checksum::GetMD5((BYTE *)&ppData[i]->nPid, 
					sizeof(int32))).c_str());

				sqliteStatement.bind(2, (unsigned char *)strTemp1.c_str(), strTemp1.length());
			}

			sqliteStatement.bind(3, (unsigned char *)ppData[i]->szTitle, (wcslen(ppData[i]->szTitle) + 1) * sizeof(wchar_t));
			sqliteStatement.bind(4, (unsigned char *)ppData[i]->szUrl, (wcslen(ppData[i]->szUrl) + 1) * sizeof(wchar_t));

			sqliteStatement.execDML();
			sqliteStatement.reset();
		}
		m_SqliteDatabase.execDML("commit transaction");

		SaveDatabase();
		return TRUE;
	}

	return FALSE;
}

int32 Maxthon2PlugIn::GetFavoriteCount()
{
	int32 nCount = 0;

	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select count(*) as Total from MyFavNodes where parent_id <> ''");

		nCount = Query.getIntField("Total");
	}

	return nCount;

}

BOOL Maxthon2PlugIn::SaveDatabase()
{
	std::string strEncode;
	wchar_t *pszFavoriteDataPath = GetFavoriteDataPath();
	int nRet = 0;

	if (m_pMemFavoriteDB == NULL)
	{
		return FALSE;
	}

	strEncode.assign((char *)m_pMemFavoriteDB->pMemPointer, m_pMemFavoriteDB->ulMemSize);

	nRet = encode(strEncode, StringHelper::UnicodeToANSI(pszFavoriteDataPath));

	free(pszFavoriteDataPath);

	return nRet == 0;
}
