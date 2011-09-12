#pragma  once

#include "SndaBase.h"
#include "malloc.h"
#include <tchar.h>
#include <shellapi.h>
#include "tchar.h"
#include <vector>
#include "shlwapi.h"
#include "DataCenterDefine.h"
#include "CppSqlite3/sqlite3.h"

using namespace std;
using namespace datacenter;

enum PlugId
{
	PLUG_ID_INVALID=-1,

	PLUG_ID_SOGOU = 0X01,	//	�ѹ�
	PLUG_ID_IE6,		    //	IE6.0,7.0
	PLUG_ID_IE9,			//	IE9.0
	PLUG_ID_MAXTHON2,		//	Maxthon 2.0
	PLUG_ID_MAXTHON3,		//	Maxthon 3.0
	PLUG_ID_360,			//	360se �����
	PLUG_ID_360EXTREME,		//	360���������
	PLUG_ID_FIREFOX,		//	FireFox
	PLUG_IN_CHROME,			//	Chrome
	PLUG_IN_TT,				//	��Ѷ��TT�����
	PLUG_IN_QQ,				//	��Ѷ��QQ�����

	PLUG_ID_END,//���һ�����߽��ж�
};

// ����ϵͳ����ʹ�õ��ĸ��ֳ���
enum 
{
	ID_VALUE_RANGE			= 0XFFF,
};


#define ID_VALUE_BEGIN(X)	(X*0X1000)
#define ID_VALUE_END(X)		((X*0X1000)+ID_VALUE_RANGE)
#define IS_ROOT_ID(id)	(id==0)



enum IDValueRanges
{
	ID_VALUE_IE6_BEGIN=ID_VALUE_BEGIN(PLUG_ID_IE6),
	ID_VALUE_IE6_END=ID_VALUE_END(PLUG_ID_IE6),

	ID_VALUE_IE9_BEGIN=ID_VALUE_BEGIN(PLUG_ID_IE9),
	ID_VALUE_IE9_END=ID_VALUE_END(PLUG_ID_IE9),

	ID_VALUE_SOGOU_BEGIN=ID_VALUE_BEGIN(PLUG_ID_SOGOU),
	ID_VALUE_SOGOU_END=ID_VALUE_END(PLUG_ID_SOGOU),

	ID_VALUE_MAXTHON2_BEGIN=ID_VALUE_BEGIN(PLUG_ID_MAXTHON2),
	ID_VALUE_MAXTHON2_END=ID_VALUE_END(PLUG_ID_MAXTHON2),

	ID_VALUE_MAXTHON3_BEGIN=ID_VALUE_BEGIN(PLUG_ID_MAXTHON3),
	ID_VALUE_MAXTHON3_END=ID_VALUE_END(PLUG_ID_MAXTHON3),

	ID_VALUE_360_BEGIN=ID_VALUE_BEGIN(PLUG_ID_360),
	ID_VALUE_360_END=ID_VALUE_END(PLUG_ID_360),

	ID_VALUE_360EXTREME_BEGIN=ID_VALUE_BEGIN(PLUG_ID_360EXTREME),
	ID_VALUE_360EXTREME_END=ID_VALUE_END(PLUG_ID_360EXTREME),

	ID_VALUE_FIREFOX_BEGIN=ID_VALUE_BEGIN(PLUG_ID_FIREFOX),
	ID_VALUE_FIREFOX_END=ID_VALUE_END(PLUG_ID_FIREFOX),

	ID_VALUE_CHROME_BEGIN=ID_VALUE_BEGIN(PLUG_IN_CHROME),
	ID_VALUE_CHROME_END=ID_VALUE_END(PLUG_IN_CHROME),

	ID_VALUE_TT_BEGIN=ID_VALUE_BEGIN(PLUG_IN_TT),
	ID_VALUE_TT_END=ID_VALUE_END(PLUG_IN_TT),

	ID_VALUE_QQ_BEGIN=ID_VALUE_BEGIN(PLUG_IN_QQ),
	ID_VALUE_QQ_END=ID_VALUE_END(PLUG_IN_QQ),
};

enum EMaxthonItemType
{
	IT_URL = 1,
	IT_FOLDER = 2,
};



typedef struct _winFileMem 
{
	unsigned char *pMemPointer;
	unsigned long  ulMemSize;
}winFileMem;


// ���뵼�������г��õĴ�����
#define ERROR_OK		0

//Sqlite3��ش���
#define ERROR_SQLITE_ERROR			SQLITE_ERROR  /* SQL error or missing database */
#define ERROR_SQLITE_INTERNAL		SQLITE_INTERNAL       /* Internal logic error in SQLite */
#define ERROR_SQLITE_PERM			SQLITE_PERM          /* Access permission denied */
#define ERROR_SQLITE_ABORT			SQLITE_ABORT           /* Callback routine requested an abort */
#define ERROR_SQLITE_BUSY			SQLITE_BUSY            /* The database file is locked */
#define ERROR_SQLITE_LOCKED		SQLITE_LOCKED          /* A table in the database is locked */
#define ERROR_SQLITE_NOMEM		SQLITE_NOMEM   /* A malloc() failed */
#define ERROR_SQLITE_READONLY		SQLITE_READONLY   /* Attempt to write a readonly database */
#define ERROR_SQLITE_INTERRUPT		SQLITE_INTERRUPT   /* Operation terminated by sqlite3_interrupt()*/
#define ERROR_SQLITE_IOERR			SQLITE_IOERR   /* Some kind of disk I/O error occurred */
#define ERROR_SQLITE_CORRUPT		SQLITE_CORRUPT   /* The database disk image is malformed */
#define ERROR_SQLITE_NOTFOUND	SQLITE_NOTFOUND   /* Unknown opcode in sqlite3_file_control() */
#define ERROR_SQLITE_FULL			SQLITE_FULL   /* Insertion failed because database is full */
#define ERROR_SQLITE_CANTOPEN		SQLITE_CANTOPEN   /* Unable to open the database file */
#define ERROR_SQLITE_PROTOCOL		SQLITE_PROTOCOL   /* Database lock protocol error */
#define ERROR_SQLITE_EMPTY			SQLITE_EMPTY   /* Database is empty */
#define ERROR_SQLITE_SCHEMA		SQLITE_SCHEMA   /* The database schema changed */
#define ERROR_SQLITE_TOOBIG		SQLITE_TOOBIG   /* String or BLOB exceeds size limit */
#define ERROR_SQLITE_CONSTRAINT	SQLITE_CONSTRAINT   /* Abort due to constraint violation */
#define ERROR_SQLITE_MISMATCH		SQLITE_MISMATCH   /* Data type mismatch */
#define ERROR_SQLITE_MISUSE		SQLITE_MISUSE  /* Library used incorrectly */
#define ERROR_SQLITE_NOLFS		SQLITE_NOLFS   /* Uses OS features not supported on host */
#define ERROR_SQLITE_AUTH			SQLITE_AUTH   /* Authorization denied */
#define ERROR_SQLITE_FORMAT		SQLITE_FORMAT   /* Auxiliary database format error */
#define ERROR_SQLITE_RANGE		SQLITE_RANGE   /* 2nd parameter to sqlite3_bind out of range */
#define ERROR_SQLITE_NOTADB		SQLITE_NOTADB   /* File opened that is not a database file */
#define ERROR_SQLITE_ROW			SQLITE_ROW  /* sqlite3_step() has another row ready */
#define ERROR_SQLITE_DONE			SQLITE_DONE  /* sqlite3_step() has finished executing */

// ����Ĵ�����
#define ERROR_INVALID_PARAM		ERROR_SQLITE_DONE + 1
#define ERROR_CLEAR_FAVORITE_DATA	ERROR_SQLITE_DONE + 2
#define ERROR_FAVORITE_PATH_NOT_EXIST		ERROR_SQLITE_DONE + 3
#define ERROR_CREATE_FAVORITE_FOLD			ERROR_SQLITE_DONE + 4
#define ERROR_CANNOT_CREATE_DB			ERROR_SQLITE_DONE + 5  /* sqlite3_step() has another row ready */
#define ERROR_INTERNAL_ERROR		ERROR_SQLITE_DONE + 100

typedef struct  STErrorDesc
{
	int nErrorCode;
	wchar_t*	szErrorMsg;

}STErrorDesc, *PSTErrorDesc;

static STErrorDesc stErrorDesc[] = 
{
	ERROR_SQLITE_ERROR,		L"SQLite�﷨����",
	ERROR_SQLITE_INTERNAL,	L"���ݿ��ڲ�����",
	ERROR_SQLITE_PERM,		L"�������ݿ�Ȩ�޲���",
	ERROR_SQLITE_ABORT,		L"",           /* Callback routine requested an abort */
	ERROR_SQLITE_BUSY,		L"���ݿ�����ʹ��",            /* The database file is locked */
	ERROR_SQLITE_LOCKED,		L"�������" ,         /* A table in the database is locked */
	ERROR_SQLITE_NOMEM	,	L"�ڴ����ʧ��" ,  /* A malloc() failed */
	ERROR_SQLITE_READONLY,	L"���ݿ�ֻ�����޷�д������",   /* Attempt to write a readonly database */
	ERROR_SQLITE_INTERRUPT,	L"" ,  /* Operation terminated by sqlite3_interrupt()*/
	ERROR_SQLITE_IOERR,		L"���ݿ� IOʧ��" ,  /* Some kind of disk I/O error occurred */
	ERROR_SQLITE_CORRUPT	,	L"���ݿ��ļ��Ѿ���",   /* The database disk image is malformed */
	ERROR_SQLITE_NOTFOUND,	L"��֧�ֵ����ݿ������"  , /* Unknown opcode in sqlite3_file_control() */
	ERROR_SQLITE_FULL,		L"���ݿ�����",   /* Insertion failed because database is full */
	ERROR_SQLITE_CANTOPEN,	L"�޷������ݿ��ļ�" ,  /* Unable to open the database file */
	ERROR_SQLITE_PROTOCOL,	L"���ݿ���Э���������" ,  /* Database lock protocol error */
	ERROR_SQLITE_EMPTY,		L"���ݿ�Ϊ��",   /* Database is empty */
	ERROR_SQLITE_SCHEMA,		L"���ݿ��schema�����仯" ,  /* The database schema changed */
	ERROR_SQLITE_TOOBIG,		L"�ַ����߶��������ݳ����˹涨�Ĵ�С"  , /* String or BLOB exceeds size limit */
	ERROR_SQLITE_CONSTRAINT,	L"" ,  /* Abort due to constraint violation */
	ERROR_SQLITE_MISMATCH,	L"�������Ͳ�ƥ��",   /* Data type mismatch */
	ERROR_SQLITE_MISUSE,		L"���ݿ�ײ�Libraryû�б���ȷ��ʹ��",  /* Library used incorrectly */
	ERROR_SQLITE_NOLFS,		L"" ,  /* Uses OS features not supported on host */ 
	ERROR_SQLITE_AUTH,		L"",   /* Authorization denied */
	ERROR_SQLITE_FORMAT,		L"" ,  /* Auxiliary database format error */
	ERROR_SQLITE_RANGE,		L"" ,  /* 2nd parameter to sqlite3_bind out of range */
	ERROR_SQLITE_NOTADB	,	L"",   /* File opened that is not a database file */
	ERROR_SQLITE_ROW,		L"",  /* sqlite3_step() has another row ready */
	ERROR_SQLITE_DONE,		L"",  /* sqlite3_step() has finished executing */
};

inline const wchar_t*	GetErrorMsg(int nErrorCode)
{
	for( int i=0; i< sizeof(stErrorDesc)/sizeof(STErrorDesc); i++)
	{
		if( stErrorDesc[i].nErrorCode == nErrorCode)
			return stErrorDesc[i].szErrorMsg;
	}

	return L"";
}

// ���������ӿڣ�ÿһ�������������ʵ�ָýӿ�
// �ϲ�Ӧ�ó���ͨ���ò���˽��Ӧ��������������Ϣ
interface IPlugIn
{
	//----------------------------------------------------------------------------------------
	//����: Load 
	//����: �����Load��������Ҫ�������ݵȳ�ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL Load() PURE;

	//----------------------------------------------------------------------------------------
	//����: UnLoad 
	//����: �����UnLoad��������Ҫ�������ݵȷ���ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL UnLoad() PURE;

	//----------------------------------------------------------------------------------------
	//����: IsWorked
	//����: ���ò���Ƿ��ܹ��������������û�а�װ��PlugIn��Ӧ�����������ò������Ҫ
	//	����ֻ��Ҫ����FALSE���ɡ����������ᱻ���á�
	//����:
	//      @param pszBrowserVersion[out]  �����ǰ֧�ֵ�������汾��
	//		@param nLen[in��out]           pszBrowserVersion��buffer����, ���ʵ��ʹ�ó���
	//����: TRUE��ʾ���Թ�����FALSE��ʾ���ܹ�����
	//----------------------------------------------------------------------------------------
	virtual BOOL IsWorked() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetPlugInVersion
	//����: ��ȡ��ǰ����İ汾��
	//����: ����İ汾�ţ�ͨ��Ϊһ������
	//----------------------------------------------------------------------------------------
	virtual int32 GetPlugInVersion() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetBrowserName
	//����: ��ȡ�ò����Ӧ������������ƺͰ汾
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetBrowserName() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetBrowserIcon
	//����: ��ȡ�ò����Ӧ���������ͼ��
	//----------------------------------------------------------------------------------------
	virtual HICON GetBrowserIcon() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetInstallPath
	//����: ��ȡ�����Ӧ��������İ�װĿ¼
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetInstallPath() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteDataPath
	//����: ��ȡ������ղؼж�Ӧ���ļ������ļ��е�·��
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetFavoriteDataPath() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetHistoryDataPath
	//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetHistoryDataPath() PURE;

	//----------------------------------------------------------------------------------------
	//����: ExportFavoriteData
	//����: ������ǰ����������е��ղؼ�����
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//����: ImportFavoriteData
	//����: ����ǰ�ļ�¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//����: ExportHistoryData
	//����: ������ʷ�����¼
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportHistoryData(PHISTORYLINEDATA pData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//����: ImportHistoryData
	//����: ����ʷ��¼��¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------

	virtual BOOL ImportHistoryData(PHISTORYLINEDATA pData, int32 nDataNum) PURE;
	//----------------------------------------------------------------------------------------
	//����: GetHistoryCount
	//����: ����ʷ��¼��¼���뵽�������
	//����: 
	//      ������ʷ��¼����
	//----------------------------------------------------------------------------------------
	virtual int32 GetHistoryCount() PURE;

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteCount
	//����: ��ȡ��������ղ���ַ������ 
	//����:
	//      �ط��ղ���ַ����
	//----------------------------------------------------------------------------------------
	virtual int32 GetFavoriteCount() PURE;
};

interface IPlugInFactory;

typedef IPlugInFactory *  (*GetPlugInFactoryFunc)();
typedef void  (*ReleasePlugInFactoryFunc)();

typedef struct _PlugInInfo
{
	wchar_t			wszPlugInName[MAX_PATH];
	HMODULE			hModule;
	std::vector<IPlugIn*>	pvPlugIn;		//	���������

	GetPlugInFactoryFunc	pGetPlugInFactoryFunc;
	ReleasePlugInFactoryFunc	pReleasePlugInFactoryFunc;

}PLUGININFO, *PPLUGININFO;

class PlugInImp : public IPlugIn
{
public:

	//----------------------------------------------------------------------------------------
	//����: Load 
	//����: �����Load��������Ҫ�������ݵȳ�ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL Load()
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: UnLoad 
	//����: �����UnLoad��������Ҫ�������ݵȷ���ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL UnLoad()
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: IsWorked
	//����: ���ò���Ƿ��ܹ��������������û�а�װ��PlugIn��Ӧ�����������ò������Ҫ
	//	����ֻ��Ҫ����FALSE���ɡ����������ᱻ���á�
	//����:
	//      @param pszBrowserVersion[out]  �����ǰ֧�ֵ�������汾��
	//		@param nLen[in��out]           pszBrowserVersion��buffer����, ���ʵ��ʹ�ó���
	//����: TRUE��ʾ���Թ�����FALSE��ʾ���ܹ�����
	//----------------------------------------------------------------------------------------
	virtual BOOL IsWorked()
	{
		wchar_t* pszInstallPath = GetInstallPath();

		if( pszInstallPath == NULL)
			return FALSE;

		// ����ļ��Ƿ����
		if( PathFileExists(pszInstallPath) == FALSE)
		{
			free(pszInstallPath);
			return FALSE;
		}

		free(pszInstallPath);
		return TRUE;
	}

	//----------------------------------------------------------------------------------------
	//����: GetPlugInVersion
	//����: ��ȡ��ǰ����İ汾��
	//����: ����İ汾�ţ�ͨ��Ϊһ������
	//----------------------------------------------------------------------------------------
	virtual int32 GetPlugInVersion()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//����: GetBrowserName
	//����: ��ȡ�ò����Ӧ������������ƺͰ汾
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetBrowserName()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//����: GetBrowserIcon
	//����: ��ȡ�ò����Ӧ���������ͼ��
	//----------------------------------------------------------------------------------------
	virtual HICON GetBrowserIcon()
	{
		wchar_t *pszBrowserPath = GetInstallPath();
		HICON    hIcon = NULL;

		if (pszBrowserPath != NULL && _wcsicmp(pszBrowserPath, _T("")) == 0)
		{
			free(pszBrowserPath);
			return NULL;
		}

		hIcon = ExtractIcon(GetModuleHandle(NULL), GetInstallPath(), 0);
		free(pszBrowserPath);

		return hIcon;
	}

	//----------------------------------------------------------------------------------------
	//����: GetInstallPath
	//����: ��ȡ�����Ӧ��������İ�װĿ¼
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetInstallPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteDataPath
	//����: ��ȡ������ղؼж�Ӧ���ļ������ļ��е�·��
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetFavoriteDataPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//����: GetHistoryDataPath
	//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetHistoryDataPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//����: ExportFavoriteData
	//����: ������ǰ����������е��ղؼ�����
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: ImportFavoriteData
	//����: ����ǰ�ļ�¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual int ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
	{
		return ERROR_OK;
	}

	//----------------------------------------------------------------------------------------
	//����: ExportHistoryData
	//����: ������ʷ�����¼
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportHistoryData(PHISTORYLINEDATA pData, int32& nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: ImportHistoryData
	//����: ����ʷ��¼��¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportHistoryData(PHISTORYLINEDATA pData, int32 nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//����: GetHistoryCount
	//����: ����ʷ��¼��¼���뵽�������
	//����: 
	//      ������ʷ��¼����
	//----------------------------------------------------------------------------------------
	virtual int32 GetHistoryCount()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteCount
	//����: ��ȡ��������ղ���ַ������ 
	//����:
	//      �ط��ղ���ַ����
	//----------------------------------------------------------------------------------------
	virtual int32 GetFavoriteCount()
	{
		return 0;
	}

	void ReplaceSingleQuoteToDoubleQuote(wchar_t *pszOri)
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
};

//----------------------------------------------------------------------------------------
//����: IPlugInFactory
//����: һ��DLL�п��԰������IPlugIn��ͨ��IPlugInFactory�����߿���֪����ǰDll�е�
//	���е�ģ�顣����IPlugInFactory������Ҫ��ԭ����Ϊ�˱���Project���ࡣ
//----------------------------------------------------------------------------------------
interface IPlugInFactory
{
	//----------------------------------------------------------------------------------------
	//����: QueryIPlugInCounter
	//����: ���ص�ǰDll��֧�ֵ�ģ�����Ŀ
	//����: 
	//		@param	nCount			���ص�ģ����Ŀ������1��2��
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryPlugInCounter(uint32&  nCount) PURE;

	//----------------------------------------------------------------------------------------
	//����: QueryIPlugInPoint
	//����: ���ص�ǰDll��֧�ֵ�ģ�����Ŀ
	//����: 
	//		@param	nCount			������������buf��ά�ȡ������2����������buf��ά����2
	//		@param	ppPlugIn		ָ��һ��IPlugIn����Ŀ�ʼλ�ã�buf��ʵ�����ṩ����䡣
	//								�ⲿͨ�������ָ��
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryPlugInPoint(uint32 nCount, IPlugIn *&  ppPlugIn) PURE;

	//----------------------------------------------------------------------------------------
	//����: ReleaseIPlugInPoint
	//����: �ͷŵ�ǰDLL�е����е�IPlugIn
	//����: 
	//		@param	nCount			������������buf��ά�ȡ������2����������buf��ά����2
	//		@param	ppPlugIn		ָ��һ��IPlugIn����Ŀ�ʼλ�ã�buf��������ʵ�������
	//----------------------------------------------------------------------------------------
	virtual void ReleasePlugInPoint(uint32 nCount, IPlugIn* pPlugIn ) PURE;
};

// 
template <typename T>
class PlugInFactoryImpl :public  IPlugInFactory
{
public:
	PlugInFactoryImpl(){}
	~PlugInFactoryImpl()
	{
	}

	virtual BOOL QueryPlugInCounter(uint32 & counter)
	{
		counter=1;
		return TRUE;
	}

	virtual BOOL QueryPlugInPoint(uint32 counter,IPlugIn*& pPlugIn)
	{
		PAssert_ReturnWithValue(1 == counter, FALSE);

		IPlugIn** ptrPlugIn = &pPlugIn;
		ptrPlugIn[0] = (IPlugIn *)&m_stPlugIn;

		return TRUE;
	}

	virtual void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
	{
	}

protected:

	T  m_stPlugIn;
};
