#include "stdafx.h"
#include "SogouPlugIn.h"
#include <shlwapi.h>
#include "StringHelper.h"
#include "TimeHelper.h"
#include "PathHelper.h"
#include "Decoder.h"
#include "CppSQLite3.h"
#include <algorithm>
#include "CRCHash.h"

using namespace sogouplugin;

#pragma comment(lib, "shlwapi.lib")

namespace sogouplugin
{
	CSogouPlugIn*	g_SogouPlugIn	=	NULL;
}

// �������ʵ��
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
bool compare(FAVORITELINEDATA*& a1,FAVORITELINEDATA*& a2)
{
	return a1->nHashId < a2->nHashId;
}
//���㷨���ʱ�临�Ӷ�ΪO(N),������Ҫ�ϲ���������ʱ�临�Ӷ�ΪO(N ^ 4)
void Merge(PFAVORITELINEDATA pData, int32 nLen, int nParentId)
{
	int nHash = 0;
	vector<PFAVORITELINEDATA> vec;

	//��������ͬ�����Ľڵ�ŵ�vec��
	for (int i = 0; i < nLen; i++)
	{
		if (pData[i].nPid == nParentId)
		{
			vec.push_back(&pData[i]);
		}
	}

	int vSize = vec.size();

	//���ɨ������Ľ��������1��merge����, ��������Ҫ�����ڵ�ָ��ͬһ�������
	if (vSize > 1)
	{
		//��vec��hashid��������, ��Ҫ��������һ�α��������ҳ�������ͬԪ��
		sort(vec.begin(), vec.end(), compare);

		for (int i = 0; i < vSize - 1; i++)
		{
			//ͬһ�����ڵ��������Hash��ͬ�������㣬�����������Ҫ�ϲ�
			if (vec[i]->nHashId == vec[i + 1]->nHashId)
			{
				//������ɾ�����, ���ɾ��
				vec[i]->bDelete = true;
				//�����������и��ڵ�Ϊj�Ľڵ��nPid, ���ϲ�
				for (int m = 0; m < nLen; m++)
				{
					if (pData[m].nPid == vec[i]->nId)
					{
						pData[m].nPid = vec[i + 1]->nId;
					}
				}

				Merge(pData, nLen, vec[i + 1]->nId);
			}
		}
	}
}
//����㷨�����Ǽ��ߣ�����Ҫ���±��
void Rearrange(PFAVORITELINEDATA pData, int nLen)
{
	//�ʱ�临�Ӷ�O(N^2)
	for (int i = 0; i < nLen; i++)
	{
		//����ý���nId���������±�+1,����Ҫ����
		if ((pData[i].nId != i + 1))
		{
			//ɨ�������Ըý��Ϊ�����Ľ�㣬��������Щ����nPid
			for (int j = 0; j < nLen; j++)
			{
				if (pData[j].nPid == pData[i].nId)
				{
					pData[j].nPid = i + 1;
				}
			}

			pData[i].nId = i + 1;
		}
	}
}

CSogouPlugIn::CSogouPlugIn()
{
	m_pMemFavoriteDB = NULL;

	Load();

	vector<FAVORITELINEDATA> vec(245);

	int32 len = 245;

	FILE *pTemp = fopen(StringHelper::UnicodeToANSI(PathHelper::GetModuleDir(NULL) + L"TestTree.bin").c_str(), "rb");
	//FILE *pTemp = fopen("aa.bin", "wb");

	//�ļ��еĽ������
	//      a
	//  b       c     �����������ҳ
	//d   e

	//ExportFavoriteData(&vec[0], len);

	fread(&vec[0], sizeof(FAVORITELINEDATA), 6, pTemp);
	//fwrite(&vec[0], sizeof(FAVORITELINEDATA), 6, pTemp);
	fclose(pTemp);

	ExportFavoriteData(&vec[6], len);

	//Rearrange(&vec[0], len + 6);

	Merge(&vec[0], len + 6, 0);

	ImportFavoriteData(&vec[0], len + 6);
	UnLoad();

}


CSogouPlugIn::~CSogouPlugIn()
{
	
}

BOOL CSogouPlugIn::Load()
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

BOOL CSogouPlugIn::UnLoad()
{
	if (m_pMemFavoriteDB)
	{
		FILE *fOut = fopen("b.db", "wb");
		fwrite(m_pMemFavoriteDB->pMemPointer, 1, m_pMemFavoriteDB->ulMemSize, fOut);
		fclose(fOut);
		free(m_pMemFavoriteDB->pMemPointer);
		free(m_pMemFavoriteDB);
		m_pMemFavoriteDB = NULL;
		return TRUE;
	}

	return FALSE;
}

BOOL CSogouPlugIn::IsWorked(wchar_t *pszBrowserVersion, int32 &nLen)
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
		if (nLen >= dwSize)
		{
			_tcscpy_s(pszBrowserVersion, nLen - 1, szVersion);
			pszBrowserVersion[nLen - 1] = 0;
		}

		return TRUE;
	}
}

int32 CSogouPlugIn::GetPluginVersion()
{
	return 1;
}

const wchar_t* CSogouPlugIn::GetBrowserName()
{
	return L"SogouExplorer";
}

wchar_t* CSogouPlugIn::GetInstallPath()
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
		return _wcsdup(szPath);
	}

	return NULL;
}

wchar_t* CSogouPlugIn::GetFavoriteDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\SogouExplorer\\Favorite2.dat";

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©

	return _wcsdup(strPath.c_str());
}

wchar_t* CSogouPlugIn::GetHistoryDataPath()
{
	std::wstring strPath = PathHelper::GetAppDataDir() + L"\\SogouExplorer\\HistoryUrl.db";

	//��Ҫ����һ��,��ȻstrPath������ʱ,����Ұָ��,�ɵ����߽����ͷ�,���������ڴ�й©

	return _wcsdup(strPath.c_str());
}

BOOL CSogouPlugIn::ExportFavoriteData( PFAVORITELINEDATA pData, int32& nDataNum )
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
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select * from FavorTable");
		int i = 0;

		while(!Query.eof() && i < nDataNum)
		{
			 memset(&pData[i], 0, sizeof(FAVORITELINEDATA));
			 pData[i].nId = Query.getIntField("id", 0) + ID_VALUE_SOGOU_BEGIN;
		     pData[i].nPid = Query.getIntField("pid", 0);

			 if (pData[i].nPid != 0)
			 {
				 pData[i].nPid += ID_VALUE_SOGOU_BEGIN;
			 }

			 pData[i].bFolder = Query.getIntField("folder", 0);
			 _tcscpy_s(pData[i].szTitle, MAX_PATH - 1, StringHelper::Utf8ToUnicode(Query.getStringField("title", 0)).c_str());
			 pData[i].szTitle[MAX_PATH - 1] = 0;
			 _tcscpy_s(pData[i].szUrl, 1023, StringHelper::Utf8ToUnicode(Query.getStringField("url", 0)).c_str());
			 pData[i].szUrl[1023] = 0;
			 pData[i].nOrder = Query.getIntField("sequenceNO", 0);

			 TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(Query.getStringField("addTime", "2011-05-11 21:00:00")).c_str()), pData[i].nAddTimes);
			 TimeHelper::SysTime2Time(TimeHelper::GetTimeFromStr2(StringHelper::Utf8ToUnicode(Query.getStringField("lastmodify", "2011-05-11 21:00:00")).c_str()), pData[i].nLastModifyTime);
			 ojbCrcHash.GetHash((BYTE *)pData[i].szTitle, wcslen(pData[i].szTitle) * sizeof(wchar_t), (BYTE *)&pData[i].nHashId, sizeof(uint32));
			 pData[i].nCatId = Query.getIntField("category", 0);

			 pData[i].bDelete = false;

			 Query.nextRow();
			 i++;
		}

		nDataNum = i;

		return TRUE;
	}

	return FALSE;
}

BOOL CSogouPlugIn::ImportFavoriteData( PFAVORITELINEDATA pData, int32 nDataNum )
{
	if (pData == NULL || nDataNum == 0)
	{
		return FALSE;
	}

	if (m_pMemFavoriteDB != NULL)
	{
#define MAX_BUFFER_LEN	4096

		CppSQLite3DB  m_SqliteDatabase;
		wchar_t szInsert[MAX_BUFFER_LEN] = {0};
		wchar_t szDelete[MAX_BUFFER_LEN] = {0};

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		int i = 0;

		m_SqliteDatabase.execDML(StringHelper::UnicodeToUtf8(L"delete from favorTable").c_str());

		for (int i = 0; i < nDataNum; i++)
		{
			if (pData[i].bDelete == true)
			{
				continue;
			}

			ReplaceSingleQuoteToDoubleQuote(pData[i].szTitle);
			ReplaceSingleQuoteToDoubleQuote(pData[i].szUrl);

			swprintf_s(szInsert, MAX_BUFFER_LEN-1, L"insert into favorTable"
				L"(id,pid,folder,title,url,sequenceNo,addtime,lastmodify,hashid,category,reserved)"
				L"values(%d,%d,%d,'%s','%s',%d,'%s',"
				L"'%s',%u,%d,0)", 
				pData[i].nId,
				pData[i].nPid,
				pData[i].bFolder,
				pData[i].szTitle,
				pData[i].szUrl,
				pData[i].nOrder,
				L"2011-05-11 12:00:00", 
				L"2011-05-11 12:00:00",
				pData[i].nHashId,
				pData[i].nCatId);
			m_SqliteDatabase.execDML(StringHelper::UnicodeToUtf8(szInsert).c_str());
		}

		SaveDatabase();

		return TRUE;
	}

	return FALSE;
}

int32 CSogouPlugIn::GetFavoriteCount()
{
	int32 nCount = 0;
	
	if (m_pMemFavoriteDB != NULL)
	{
		CppSQLite3DB  m_SqliteDatabase;

		m_SqliteDatabase.openmem(m_pMemFavoriteDB, "");
		CppSQLite3Query Query = m_SqliteDatabase.execQuery("select count(*) as Total from FavorTable");

		nCount = Query.getIntField("Total");
	}

	return nCount;
}

BOOL CSogouPlugIn::SaveDatabase()
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

void CSogouPlugIn::ReplaceSingleQuoteToDoubleQuote(wchar_t *pszOri)
{
	int32 nLen = _tcslen(pszOri);

	if (pszOri == NULL || nLen == 0)
	{
		return;
	}

	for (int i = 0; i < nLen; i++)
	{
		if (pszOri[i] == '\'')
		{
			pszOri[i] = '\"';
		}
	}

}



