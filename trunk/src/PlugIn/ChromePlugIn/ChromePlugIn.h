#pragma once
#include "PlugIn.h"
#include "json/json.h"
#include <stack>
#include <map>

#define MAX_URL_LEN  1024

typedef struct ChromeFolderNode
{
	time_t  nAddedTime;
	time_t  nModifiedTime;
	int32   nId;
	wchar_t szName[MAX_PATH];
	wchar_t szType[MAX_PATH];
} CHROMEFOLDERNODE, *PCHROMEFOLDERNODE;

typedef struct ChromeUrlNode
{
	time_t  nAddedTime;
	int32   nId;
	wchar_t szName[MAX_PATH];
	wchar_t szType[MAX_PATH];
	wchar_t szUrl[MAX_URL_LEN];
} CHROMEURLNODE, *PCHROMEURLNODE;

class CChromePlugIn : public PlugInImp
{
public:
	CChromePlugIn();
	~CChromePlugIn();

	//----------------------------------------------------------------------------------------
	//名称: Load 
	//描述: 插件的Load方法，主要用于数据等初始化 
	//返回: 初始化成功返回TRUE，初始化失败返回FALSE。
	//----------------------------------------------------------------------------------------
	virtual BOOL Load();

	//----------------------------------------------------------------------------------------
	//名称: UnLoad 
	//描述: 插件的UnLoad方法，主要用于数据等反初始化 
	//返回: 初始化成功返回TRUE，初始化失败返回FALSE。
	//----------------------------------------------------------------------------------------
	virtual BOOL UnLoad();

	//----------------------------------------------------------------------------------------
	//名称: GetPlugInVersion
	//描述: 获取当前插件的版本号
	//返回: 插件的版本号，通常为一整数。
	//----------------------------------------------------------------------------------------
	virtual int32 GetPlugInVersion() ;

	//----------------------------------------------------------------------------------------
	//名称: GetBrowserName
	//描述: 获取该插件对应的浏览器的名称和版本
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetBrowserName() ;

	//----------------------------------------------------------------------------------------
	//名称: GetInstallPath
	//描述: 获取插件对应的浏览器的安装目录
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetInstallPath() ;

	//----------------------------------------------------------------------------------------
	//名称: GetFavoriteDataPath
	//描述: 获取浏览器收藏夹对应的文件或者文件夹的路径
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetFavoriteDataPath() ;

	//----------------------------------------------------------------------------------------
	//名称: GetHistoryDataPath
	//描述: 获取浏览器收藏夹的历史数据对应的文件或者文件夹
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetHistoryDataPath() ;

	//----------------------------------------------------------------------------------------
	//名称: ExportFavoriteData
	//描述: 导出当前浏览器的所有的收藏夹数据
	//参数: 
	//		@param	pData			导出的收藏夹数据数组
	//		@param	nDataNum		导出的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum);

	//----------------------------------------------------------------------------------------
	//名称: ImportFavoriteData
	//描述: 将当前的记录导入到浏览器中
	//参数: 
	//		@param	pData			需要导入的的收藏夹数据数组
	//		@param	nDataNum		需要导入的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum);

	//----------------------------------------------------------------------------------------
	//名称: ImportFavoriteData
	//描述: 将一条记录导入到浏览器中
	//参数: 
	//		@param	stData			需要导入的的收藏夹记录
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(FAVORITELINEDATA stData);

	//----------------------------------------------------------------------------------------
	//名称: GetFavoriteCount
	//描述: 获取浏览器中收藏网址的条数 
	//返回:
	//      回返收藏网址条数
	//----------------------------------------------------------------------------------------
	virtual int32 GetFavoriteCount();

private:
	BOOL ExportFolder(Json::Value& folder_obj, int32 nPid, PFAVORITELINEDATA pData, int32& nDataNum);
	BOOL ExportUrl(Json::Value& url_obj, int32 nPid, PFAVORITELINEDATA pData, int32& nDataNum);
	BOOL MakeFolderNode(CHROMEFOLDERNODE stFolderNode, Json::Value& folder_obj);
	BOOL MakeUrlNode(CHROMEURLNODE stUrlNode, Json::Value& url_obj);

	BOOL MakeSpecialFolderNode(wchar_t *pszName, int32& nIndex, Json::Value& folder_obj);
	BOOL EnumNode(Json::Value& folder_obj, int32& nCount);

	BOOL MergeNode();

private:
	typedef struct Node
	{
		int32 nId;
		int32 nPid;
		bool bIsFolder;
	} NODE, *PNODE;

	std::stack<NODE> m_stkNodeList;
	std::multimap<int32, Json::Value> m_mapNodeTree;
};