#pragma once
#include "PlugIn.h"


class CppSQLite3DB;
class FireFox3PlugIn : public PlugInImp
{
public:
	FireFox3PlugIn();
	~FireFox3PlugIn();

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
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum);

	//----------------------------------------------------------------------------------------
	//名称: ImportFavoriteData
	//描述: 将当前的记录导入到浏览器中
	//参数: 
	//		@param	pData			需要导入的的收藏夹数据数组
	//		@param	nDataNum		需要导入的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual int ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum);

	//----------------------------------------------------------------------------------------
	//名称: GetFavoriteCount
	//描述: 获取浏览器中收藏网址的条数 
	//返回:
	//      回返收藏网址条数
	//----------------------------------------------------------------------------------------
	virtual int32 GetFavoriteCount();

protected:
	
	// 因为Firefox的目录比较特殊，它位于C:\Users\zhangzq\AppData\Roaming\Mozilla\Firefox\Profiles\*.default
	// 目录下，*为一随即数，因此必须先计算出该目录
	wchar_t* GetAppBasePath();

	// 获取父结点为nParentId的所有结点的数目
	int	GetFavoriteCount(int nParentId);

	// 导出所有父结点为nParentId的结点的收藏数据
	// nCurrentIndex为导出的收藏数据
	BOOL ExportFavoriteData(int nParentId, PFAVORITELINEDATA* ppData, int32& nCurrentIndex);

	// 数据库相关的函数
protected:

	// 获取所有的系统的记录ID
	void GetSystemFavoriteId(int nParentId, std::vector<int>& vIdData); 
	void GetSystemFavoriteId(std::vector<int>& vIdData); 

	void InsertIntoDB(int nRootId, PFAVORITELINEDATA* ppData, int32 nDataNum);

private:

	CppSQLite3DB* m_pSqliteDatabase;
};
