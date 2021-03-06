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

	PLUG_ID_SOGOU = 0X01,	//	搜狗
	PLUG_ID_IE6,		    //	IE6.0,7.0
	PLUG_ID_IE9,			//	IE9.0
	PLUG_ID_MAXTHON2,		//	Maxthon 2.0
	PLUG_ID_MAXTHON3,		//	Maxthon 3.0
	PLUG_ID_360,			//	360se 浏览器
	PLUG_ID_360EXTREME,		//	360极速浏览器
	PLUG_ID_FIREFOX,		//	FireFox
	PLUG_IN_CHROME,			//	Chrome
	PLUG_IN_TT,				//	腾讯的TT浏览器
	PLUG_IN_QQ,				//	腾讯的QQ浏览器

	PLUG_ID_END,//最后一个，边界判断
};

// 定义系统中所使用到的各种常量
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


// 导入导出过程中常用的错误码
#define ERROR_OK		0

//Sqlite3相关错误
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

// 其余的错误码
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
	ERROR_SQLITE_ERROR,		L"SQLite语法错误",
	ERROR_SQLITE_INTERNAL,	L"数据库内部错误",
	ERROR_SQLITE_PERM,		L"访问数据库权限不够",
	ERROR_SQLITE_ABORT,		L"",           /* Callback routine requested an abort */
	ERROR_SQLITE_BUSY,		L"数据库正被使用",            /* The database file is locked */
	ERROR_SQLITE_LOCKED,		L"表格被锁定" ,         /* A table in the database is locked */
	ERROR_SQLITE_NOMEM	,	L"内存分配失败" ,  /* A malloc() failed */
	ERROR_SQLITE_READONLY,	L"数据库只读，无法写入数据",   /* Attempt to write a readonly database */
	ERROR_SQLITE_INTERRUPT,	L"" ,  /* Operation terminated by sqlite3_interrupt()*/
	ERROR_SQLITE_IOERR,		L"数据库 IO失败" ,  /* Some kind of disk I/O error occurred */
	ERROR_SQLITE_CORRUPT	,	L"数据库文件已经损坏",   /* The database disk image is malformed */
	ERROR_SQLITE_NOTFOUND,	L"不支持的数据库操作码"  , /* Unknown opcode in sqlite3_file_control() */
	ERROR_SQLITE_FULL,		L"数据库已满",   /* Insertion failed because database is full */
	ERROR_SQLITE_CANTOPEN,	L"无法打开数据库文件" ,  /* Unable to open the database file */
	ERROR_SQLITE_PROTOCOL,	L"数据库因协议错误被锁定" ,  /* Database lock protocol error */
	ERROR_SQLITE_EMPTY,		L"数据库为空",   /* Database is empty */
	ERROR_SQLITE_SCHEMA,		L"数据库的schema发生变化" ,  /* The database schema changed */
	ERROR_SQLITE_TOOBIG,		L"字符或者二进制数据超过了规定的大小"  , /* String or BLOB exceeds size limit */
	ERROR_SQLITE_CONSTRAINT,	L"" ,  /* Abort due to constraint violation */
	ERROR_SQLITE_MISMATCH,	L"数据类型不匹配",   /* Data type mismatch */
	ERROR_SQLITE_MISUSE,		L"数据库底层Library没有被正确的使用",  /* Library used incorrectly */
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

// 浏览器插件接口，每一个浏览器都必须实现该接口
// 上层应用程序通过该插件了解对应的浏览器的相关信息
interface IPlugIn
{
	//----------------------------------------------------------------------------------------
	//名称: Load 
	//描述: 插件的Load方法，主要用于数据等初始化 
	//返回: 初始化成功返回TRUE，初始化失败返回FALSE。
	//----------------------------------------------------------------------------------------
	virtual BOOL Load() PURE;

	//----------------------------------------------------------------------------------------
	//名称: UnLoad 
	//描述: 插件的UnLoad方法，主要用于数据等反初始化 
	//返回: 初始化成功返回TRUE，初始化失败返回FALSE。
	//----------------------------------------------------------------------------------------
	virtual BOOL UnLoad() PURE;

	//----------------------------------------------------------------------------------------
	//名称: IsWorked
	//描述: 检查该插件是否能够正常工作。如果没有安装该PlugIn对应的浏览器，则该插件不需要
	//	处理，只需要返回FALSE即可。否则它将会被调用。
	//参数:
	//      @param pszBrowserVersion[out]  输出当前支持的浏览器版本号
	//		@param nLen[in、out]           pszBrowserVersion的buffer长度, 输出实际使用长度
	//返回: TRUE表示可以工作，FALSE表示不能工作。
	//----------------------------------------------------------------------------------------
	virtual BOOL IsWorked() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetPlugInVersion
	//描述: 获取当前插件的版本号
	//返回: 插件的版本号，通常为一整数。
	//----------------------------------------------------------------------------------------
	virtual int32 GetPlugInVersion() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetBrowserName
	//描述: 获取该插件对应的浏览器的名称和版本
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetBrowserName() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetBrowserIcon
	//描述: 获取该插件对应的浏览器的图标
	//----------------------------------------------------------------------------------------
	virtual HICON GetBrowserIcon() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetInstallPath
	//描述: 获取插件对应的浏览器的安装目录
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetInstallPath() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetFavoriteDataPath
	//描述: 获取浏览器收藏夹对应的文件或者文件夹的路径
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetFavoriteDataPath() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetHistoryDataPath
	//描述: 获取浏览器收藏夹的历史数据对应的文件或者文件夹
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetHistoryDataPath() PURE;

	//----------------------------------------------------------------------------------------
	//名称: ExportFavoriteData
	//描述: 导出当前浏览器的所有的收藏夹数据
	//参数: 
	//		@param	pData			导出的收藏夹数据数组
	//		@param	nDataNum		导出的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//名称: ImportFavoriteData
	//描述: 将当前的记录导入到浏览器中
	//参数: 
	//		@param	pData			需要导入的的收藏夹数据数组
	//		@param	nDataNum		需要导入的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//名称: ExportHistoryData
	//描述: 导出历史浏览记录
	//参数: 
	//		@param	pData			导出的收藏夹数据数组
	//		@param	nDataNum		导出的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportHistoryData(PHISTORYLINEDATA pData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//名称: ImportHistoryData
	//描述: 将历史记录记录导入到浏览器中
	//参数: 
	//		@param	pData			需要导入的的收藏夹数据数组
	//		@param	nDataNum		需要导入的收藏夹条目的条数
	//----------------------------------------------------------------------------------------

	virtual BOOL ImportHistoryData(PHISTORYLINEDATA pData, int32 nDataNum) PURE;
	//----------------------------------------------------------------------------------------
	//名称: GetHistoryCount
	//描述: 将历史记录记录导入到浏览器中
	//返回: 
	//      返回历史记录条数
	//----------------------------------------------------------------------------------------
	virtual int32 GetHistoryCount() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetFavoriteCount
	//描述: 获取浏览器中收藏网址的条数 
	//返回:
	//      回返收藏网址条数
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
	std::vector<IPlugIn*>	pvPlugIn;		//	插件的数组

	GetPlugInFactoryFunc	pGetPlugInFactoryFunc;
	ReleasePlugInFactoryFunc	pReleasePlugInFactoryFunc;

}PLUGININFO, *PPLUGININFO;

class PlugInImp : public IPlugIn
{
public:

	//----------------------------------------------------------------------------------------
	//名称: Load 
	//描述: 插件的Load方法，主要用于数据等初始化 
	//返回: 初始化成功返回TRUE，初始化失败返回FALSE。
	//----------------------------------------------------------------------------------------
	virtual BOOL Load()
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//名称: UnLoad 
	//描述: 插件的UnLoad方法，主要用于数据等反初始化 
	//返回: 初始化成功返回TRUE，初始化失败返回FALSE。
	//----------------------------------------------------------------------------------------
	virtual BOOL UnLoad()
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//名称: IsWorked
	//描述: 检查该插件是否能够正常工作。如果没有安装该PlugIn对应的浏览器，则该插件不需要
	//	处理，只需要返回FALSE即可。否则它将会被调用。
	//参数:
	//      @param pszBrowserVersion[out]  输出当前支持的浏览器版本号
	//		@param nLen[in、out]           pszBrowserVersion的buffer长度, 输出实际使用长度
	//返回: TRUE表示可以工作，FALSE表示不能工作。
	//----------------------------------------------------------------------------------------
	virtual BOOL IsWorked()
	{
		wchar_t* pszInstallPath = GetInstallPath();

		if( pszInstallPath == NULL)
			return FALSE;

		// 检查文件是否存在
		if( PathFileExists(pszInstallPath) == FALSE)
		{
			free(pszInstallPath);
			return FALSE;
		}

		free(pszInstallPath);
		return TRUE;
	}

	//----------------------------------------------------------------------------------------
	//名称: GetPlugInVersion
	//描述: 获取当前插件的版本号
	//返回: 插件的版本号，通常为一整数。
	//----------------------------------------------------------------------------------------
	virtual int32 GetPlugInVersion()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//名称: GetBrowserName
	//描述: 获取该插件对应的浏览器的名称和版本
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetBrowserName()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//名称: GetBrowserIcon
	//描述: 获取该插件对应的浏览器的图标
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
	//名称: GetInstallPath
	//描述: 获取插件对应的浏览器的安装目录
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetInstallPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//名称: GetFavoriteDataPath
	//描述: 获取浏览器收藏夹对应的文件或者文件夹的路径
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetFavoriteDataPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//名称: GetHistoryDataPath
	//描述: 获取浏览器收藏夹的历史数据对应的文件或者文件夹
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetHistoryDataPath()
	{
		return L"";
	}

	//----------------------------------------------------------------------------------------
	//名称: ExportFavoriteData
	//描述: 导出当前浏览器的所有的收藏夹数据
	//参数: 
	//		@param	pData			导出的收藏夹数据数组
	//		@param	nDataNum		导出的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//名称: ImportFavoriteData
	//描述: 将当前的记录导入到浏览器中
	//参数: 
	//		@param	pData			需要导入的的收藏夹数据数组
	//		@param	nDataNum		需要导入的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual int ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum)
	{
		return ERROR_OK;
	}

	//----------------------------------------------------------------------------------------
	//名称: ExportHistoryData
	//描述: 导出历史浏览记录
	//参数: 
	//		@param	pData			导出的收藏夹数据数组
	//		@param	nDataNum		导出的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportHistoryData(PHISTORYLINEDATA pData, int32& nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//名称: ImportHistoryData
	//描述: 将历史记录记录导入到浏览器中
	//参数: 
	//		@param	pData			需要导入的的收藏夹数据数组
	//		@param	nDataNum		需要导入的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportHistoryData(PHISTORYLINEDATA pData, int32 nDataNum)
	{
		return FALSE;
	}

	//----------------------------------------------------------------------------------------
	//名称: GetHistoryCount
	//描述: 将历史记录记录导入到浏览器中
	//返回: 
	//      返回历史记录条数
	//----------------------------------------------------------------------------------------
	virtual int32 GetHistoryCount()
	{
		return 0;
	}

	//----------------------------------------------------------------------------------------
	//名称: GetFavoriteCount
	//描述: 获取浏览器中收藏网址的条数 
	//返回:
	//      回返收藏网址条数
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
//名称: IPlugInFactory
//描述: 一个DLL中可以包含多个IPlugIn，通过IPlugInFactory，总线可以知晓当前Dll中的
//	所有的模块。引入IPlugInFactory的最主要的原因是为了避免Project过多。
//----------------------------------------------------------------------------------------
interface IPlugInFactory
{
	//----------------------------------------------------------------------------------------
	//名称: QueryIPlugInCounter
	//描述: 返回当前Dll中支持的模块的数目
	//参数: 
	//		@param	nCount			返回的模块数目，比如1，2等
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryPlugInCounter(uint32&  nCount) PURE;

	//----------------------------------------------------------------------------------------
	//名称: QueryIPlugInPoint
	//描述: 返回当前Dll中支持的模块的数目
	//参数: 
	//		@param	nCount			参数表明后面buf的维度。如果是2，表明后面buf的维度是2
	//		@param	ppPlugIn		指向一个IPlugIn数组的开始位置，buf由实现者提供被填充。
	//								外部通常传入空指针
	//----------------------------------------------------------------------------------------
	virtual BOOL QueryPlugInPoint(uint32 nCount, IPlugIn *&  ppPlugIn) PURE;

	//----------------------------------------------------------------------------------------
	//名称: ReleaseIPlugInPoint
	//描述: 释放当前DLL中的所有的IPlugIn
	//参数: 
	//		@param	nCount			参数表明后面buf的维度。如果是2，表明后面buf的维度是2
	//		@param	ppPlugIn		指向一个IPlugIn数组的开始位置，buf的内容由实现者填充
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
