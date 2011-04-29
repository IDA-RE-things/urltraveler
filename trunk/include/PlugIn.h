#pragma  once

#include "SndaBase.h"

// 定义收藏夹数据结构

// 收藏夹中每一个收藏记录的数据
struct FavoriteLineData
{
	int32	nCatId;				// 分类ID
	int32	nParentCatId;	//	父类的ID
	wchar_t	szUrl;		//	收藏的URL
	wchar_t	szTitle;		//	描述信息
	int32	nClickTimes;	//	点击次数
	int32	nOrder;			//	顺序
};
typedef FavoriteLineData	FAVORITELINEDATA;
typedef FAVORITELINEDATA* PFAVORITELINEDATA;

// 浏览器插件接口，每一个浏览器都必须实现该接口
// 上层应用程序通过该插件了解对应的浏览器的相关信息
class IPlugIn
{
	//----------------------------------------------------------------------------------------
	//名称: IsWorked
	//描述: 检查该插件是否能够正常工作。如果没有安装该PlugIn对应的浏览器，则该插件不需要
	//	处理，只需要返回FALSE即可。否则它将会被调用。
	//返回: 插件的版本号，通常为一整数。
	//----------------------------------------------------------------------------------------
	virtual BOOL IsWorked() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetPluginVersion
	//描述: 获取当前插件的版本号
	//返回: 插件的版本号，通常为一整数。
	//----------------------------------------------------------------------------------------
	virtual int32 GetPluginVersion() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetBrowserName
	//描述: 获取该插件对应的浏览器的名称和版本
	//----------------------------------------------------------------------------------------
	virtual int32 GetBrowserName() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetInstallPath
	//描述: 获取插件对应的浏览器的安装目录
	//----------------------------------------------------------------------------------------
	virtual int32 GetInstallPath() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetFavoriteDataPath
	//描述: 获取浏览器收藏夹对应的文件或者文件夹的路径
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetFavoriteDataPath() PURE;

	//----------------------------------------------------------------------------------------
	//名称: GetHistoryDataPath
	//描述: 获取浏览器收藏夹的历史数据对应的文件或者文件夹
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetHistoryDataPath() PURE;

	//----------------------------------------------------------------------------------------
	//名称: ImportFavoriteData
	//描述: 导出当前浏览器的所有的收藏夹数据
	//参数: 
	//		@param	pData			到处的收藏夹数据数组
	//		@param	nDataNum	收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL		ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum) PURE;
};