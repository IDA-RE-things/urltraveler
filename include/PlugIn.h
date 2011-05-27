#pragma  once

#include "SndaBase.h"
#include "malloc.h"
#include <tchar.h>
#include <shellapi.h>
#include "tchar.h"
#include <vector>
#include "shlwapi.h"

using namespace std;

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

// 定义收藏夹数据结构

// 收藏夹中每一个收藏记录的数据
typedef struct FavoriteLineData
{
	int32       nId;                //  ID
	wchar_t	    szTitle[MAX_PATH];	//	描述文本，或者为分类名称，或者为URL的title
	int32		nCatId;				//  分类ID
	int32		nPid;		        //	父类的ID
	bool        bFolder;            //  是否是目录结点。如果是true，表示是目录结点，否则是子结点
	wchar_t	    szUrl[1024];		//	收藏的URL
	time_t		nAddTimes;		    //	增加的时间
	time_t		nLastModifyTime;	//	最近的更改时间
	int32		nClickTimes;		//	点击次数
	uint32		nHashId;			//  Hash
	int32		nOrder;				//	当前结点在当前层次中的顺序
	bool        bDelete;

	FavoriteLineData()
	{
		nId	=	0;
		nCatId	=	0;
		nPid	=	0;
		bFolder	=	false;
		nAddTimes	=	0;
		nLastModifyTime	=	0;
		nClickTimes	=	0;
		nHashId	=	0;
		nOrder	=	0;
		bDelete =   false;
	}

	bool operator < (const FavoriteLineData& x)const
	{
		if (nPid < x.nPid)
		{
			return true;
		}

		return false;
	}

} FAVORITELINEDATA, *PFAVORITELINEDATA; 

typedef struct HistoryLineData
{
	wchar_t	    szTitle;			//	描述文本，或者为分类名称，或者为URL的title
	int32		nCatId;				//	分类ID
	int32		nParentCatId;		//	父类的ID，如果nParentCatId=0表示当前ID是目录结点
	wchar_t	    szUrl;				//	收藏的URL
	int32		nAddTimes;			//	增加的时间
	int32		nLastModifyTime;	//	最近的更改时间
	int32		nClickTimes;		//	点击次数
	int32		nOrder;				//	当前结点在当前层次中的顺序

} HISTORYLINEDATA, *PHISTORYLINEDATA;


typedef struct _winFileMem 
{
	unsigned char *pMemPointer;
	unsigned long  ulMemSize;
}winFileMem;

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
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum) PURE;

	//----------------------------------------------------------------------------------------
	//名称: ImportFavoriteData
	//描述: 将当前的记录导入到浏览器中
	//参数: 
	//		@param	pData			需要导入的的收藏夹数据数组
	//		@param	nDataNum		需要导入的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum) PURE;

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

class IPlugInFactory;

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

		if (_wcsicmp(pszBrowserPath, _T("")) == 0)
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
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum)
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
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum)
	{
		return FALSE;
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
		PAssert_ReturnWithValue(1 == counter, false);

		IPlugIn** ptrPlugIn = &pPlugIn;
		ptrPlugIn[0] = (IPlugIn *)&m_stPlugIn;

		return FALSE;
	}

	virtual void ReleasePlugInPoint(uint32 counter,IPlugIn* pPlugIn)
	{
	}

protected:

	T  m_stPlugIn;
};
