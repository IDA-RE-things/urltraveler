#include "StdAfx.h"
#include "IePlugIn.h"

using namespace ieplugin;

namespace ieplugin
{
	IEPlugIn*	g_IEPlugIn	=	NULL;
}

// 导出借口实现
IPlugIn*	GetPlugIn()
{
	if( g_IEPlugIn == NULL)
	{
		g_IEPlugIn = new IEPlugIn();
		ASSERT( g_IEPlugIn != NULL);
	}

	return g_IEPlugIn;
}

void	ReleasePlugIn( IPlugIn* p)
{
	ASSERT( g_IEPlugIn == p);
	if( g_IEPlugIn  != NULL)
	{
		delete g_IEPlugIn;
		g_IEPlugIn = NULL;
	}
}

IEPlugIn::IEPlugIn(void)
{
}

IEPlugIn::~IEPlugIn(void)
{
}


//----------------------------------------------------------------------------------------
//名称: IsWorked
//描述: 检查该插件是否能够正常工作。如果没有安装该PlugIn对应的浏览器，则该插件不需要
//	处理，只需要返回FALSE即可。否则它将会被调用。
//返回: 插件的版本号，通常为一整数。
//----------------------------------------------------------------------------------------
BOOL IEPlugIn::IsWorked()
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: GetPluginVersion
//描述: 获取当前插件的版本号
//返回: 插件的版本号，通常为一整数。
//----------------------------------------------------------------------------------------
int32 IEPlugIn::GetPluginVersion() 
{
	return 0;
}

//----------------------------------------------------------------------------------------
//名称: GetBrowserName
//描述: 获取该插件对应的浏览器的名称和版本
//----------------------------------------------------------------------------------------
const wchar_t* IEPlugIn::GetBrowserName() 
{
	return NULL;
}

//----------------------------------------------------------------------------------------
//名称: GetInstallPath
//描述: 获取插件对应的浏览器的安装目录
//----------------------------------------------------------------------------------------
const wchar_t* IEPlugIn::GetInstallPath() 
{
	return NULL;
}

//----------------------------------------------------------------------------------------
//名称: GetFavoriteDataPath
//描述: 获取浏览器收藏夹对应的文件或者文件夹的路径
//----------------------------------------------------------------------------------------
const wchar_t* IEPlugIn::GetFavoriteDataPath() 
{
	return NULL;
}

//----------------------------------------------------------------------------------------
//名称: GetHistoryDataPath
//描述: 获取浏览器收藏夹的历史数据对应的文件或者文件夹
//----------------------------------------------------------------------------------------
const wchar_t* IEPlugIn::GetHistoryDataPath() 
{
	return NULL;
}

//----------------------------------------------------------------------------------------
//名称: ExportFavoriteData
//描述: 导出当前浏览器的所有的收藏夹数据
//参数: 
//		@param	pData			导出的收藏夹数据数组
//		@param	nDataNum		导出的收藏夹条目的条数
//----------------------------------------------------------------------------------------
BOOL IEPlugIn::ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: ImportFavoriteData
//描述: 将当前的记录导入到浏览器中
//参数: 
//		@param	pData			需要导入的的收藏夹数据数组
//		@param	nDataNum		需要导入的收藏夹条目的条数
//----------------------------------------------------------------------------------------
BOOL IEPlugIn::ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum)
{
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: ImportFavoriteData
//描述: 将一条记录导入到浏览器中
//参数: 
//		@param	stData			需要导入的的收藏夹记录
//----------------------------------------------------------------------------------------
BOOL IEPlugIn::ImportFavoriteData(FAVORITELINEDATA stData) 
{
	return TRUE;
}