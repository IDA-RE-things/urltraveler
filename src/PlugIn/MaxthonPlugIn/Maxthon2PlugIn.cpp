#include "stdafx.h"
#include "Maxthon2PlugIn.h"
#include <string>
#include "Decoder.h"
#include "StringHelper.h"
#include <shlwapi.h>
#include "PathHelper.h"
#include "CppSQLite3.h"
#include "CRCHash.h"

#include "MaxthonPlugInFactory.h"

#pragma comment(lib, "shlwapi.lib")


Maxthon2PlugIn::Maxthon2PlugIn()
{
	Load();
	int32 nCount = GetFavoriteCount();
	vector<FAVORITELINEDATA> vecFav(100);
	int nNum = 100;
	ExportFavoriteData(&vecFav[0], nNum);
	UnLoad();

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
	return TRUE;
}

int32 Maxthon2PlugIn::GetPlugInVersion()
{
	return 1;
}

const wchar_t* Maxthon2PlugIn::GetBrowserName()
{
	return L"Maxthon";
}

wchar_t* Maxthon2PlugIn::GetInstallPath()
{
	wchar_t szPath[MAX_PATH] = {0};
	DWORD   dwSize = sizeof(szPath); 

	if (ERROR_SUCCESS == SHRegGetValue(HKEY_LOCAL_MACHINE, 
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Maxthon3",
		L"DisplayIcon", 
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

BOOL Maxthon2PlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
{
	CCRCHash ojbCrcHash;

	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from MyFavNodes");
		int i = 0;
		unsigned char szParentNode[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		while(!Query.eof() && i < nDataNum)
		{
			const unsigned char *pTemp = NULL;
			int nBlobLen = 0;
			pTemp = Query.getBlobField("parent_id", nBlobLen);

			if (nBlobLen == 0)
			{
				Query.nextRow();
				continue;
			}

			if (memcmp(pTemp, szParentNode, 16) == 0)
			{
				pData[i].nPid = 0;
			}
			else
			{
				ojbCrcHash.GetHash((BYTE *)pTemp, nBlobLen, (BYTE *)&pData[i].nPid, sizeof(uint32));
			}

			pTemp = Query.getBlobField("id", nBlobLen);

			ojbCrcHash.GetHash((BYTE *)pTemp, nBlobLen, (BYTE *)&pData[i].nId, sizeof(uint32));

			pTemp = Query.getBlobField("title", nBlobLen);

			if (nBlobLen != 0)
			{
				memcpy(pData[i].szTitle, pTemp, nBlobLen);

				ojbCrcHash.GetHash((BYTE *)pData[i].szTitle, wcslen(pData[i].szTitle) * sizeof(wchar_t), (BYTE *)&pData[i].nHashId, sizeof(uint32));
			}
			else
			{
				pData[i].szTitle[0] = 0;
			}

			pTemp = Query.getBlobField("url", nBlobLen);

			if (nBlobLen != 0)
			{
				memcpy(pData[i].szUrl, pTemp, nBlobLen);
			}
			else
			{
				pData[i].szUrl[0] = 0;
			}

			pData[i].bFolder = Query.getIntField("type", 1) == IT_FOLDER ? true : false;

			pData[i].nAddTimes = Query.getIntField("add_date", 0);
			pData[i].nClickTimes = Query.getIntField("visit_count", 0);
			pData[i].nLastModifyTime = Query.getIntField("last_modified", 0);
			pData[i].bDelete = false;
			pData[i].nCatId = 0;
			pData[i].nOrder = Query.getIntField("norder", 0);

			Query.nextRow();
			i++;
		}

		nDataNum = i;

		for (int i = 0; i < nDataNum; i++)
		{
			//如果该结点的nId不是数组下标+1,则需要修正
			if ((pData[i].nId != i + 1 + ID_VALUE_MAXTHON2_BEGIN))
			{
				//扫描所有以该结点为父结点的结点，并修正这些结点的nPid
				for (int j = 0; j < nDataNum; j++)
				{
					if (pData[j].nPid == pData[i].nId)
					{
						pData[j].nPid = i + 1 + ID_VALUE_MAXTHON2_BEGIN;
					}
				}

				pData[i].nId = i + 1 + ID_VALUE_MAXTHON2_BEGIN;
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL Maxthon2PlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	return TRUE;
}

int32 Maxthon2PlugIn::GetFavoriteCount()
{
	int32 nCount = 0;

	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select count(*) as Total from MyFavNodes");

		nCount = Query.getIntField("Total");
	}

	return nCount;

}
