#pragma once
#include "PlugIn.h"
#include "json/json.h"
#include <vector>
#include <map>
#include "md5/md5.h"

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
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum);

	//----------------------------------------------------------------------------------------
	//名称: ImportFavoriteData
	//描述: 将当前的记录导入到浏览器中
	//参数: 
	//		@param	pData			需要导入的的收藏夹数据数组
	//		@param	nDataNum		需要导入的收藏夹条目的条数
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum);

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
	BOOL ExportFolder(Json::Value& folder_obj, int32 nPid, PFAVORITELINEDATA* ppData, int nDataNum, int32& nRealDataNum);
	BOOL ExportUrl(Json::Value& url_obj, int32 nPid, PFAVORITELINEDATA*  ppData, int32 nDataNum, int32& nRealDataNum);
	BOOL MakeFolderNode(PFAVORITELINEDATA pstData, Json::Value& folder_obj, uint32& nIndex);
	BOOL MakeUrlNode(PFAVORITELINEDATA pstData, Json::Value& url_obj, uint32& nIndex);
	BOOL MakeSpecialFolderNode(wchar_t *pszName, uint32& nIndex, Json::Value& folder_obj);

	BOOL EnumNode(Json::Value& folder_obj, int32& nCount);
	BOOL TraverseNode(PFAVORITELINEDATA*  ppData, int32 nDepth);

	BOOL StringToInt64(std::string strTime, int64& nTime);
	BOOL Int64ToString(int64 nTime, std::string& strTime);
	inline bool IsValidCharacter(uint32 code_point);
	bool IsStringUTF8(const std::string& str);

	void InitializeChecksum();
	void UpdateChecksum(const std::string& str);
	void UpdateChecksum(const std::wstring& str);
	void UpdateChecksumWithUrlNode(const std::string& id, const std::wstring& title, const std::string& url);
	void UpdateChecksumWithFolderNode(const std::string& id, const std::wstring& title); 
	void FinalizeChecksum();

	void SortByDepth(PFAVORITELINEDATA*  ppData, int32 nDataNum);
	void SortNode(PFAVORITELINEDATA*  ppData, int32 nDataNum,
		PFAVORITELINEDATA* & ppSortData, int32 nParentId, int& k);

private:
	typedef struct NodeInfo
	{
		int32 nIndex;
		Json::Value node_obj;

	} NODEINFO, *PNODEINFO;

	int32 m_nMaxDepth;
	uint32 m_nIndex;

	typedef std::multimap<int32, int32> MAP_DEPTH_INFO;
	MAP_DEPTH_INFO m_mapDepthInfo;

	typedef std::map<int32, int32> MAP_PID_INFO;
	MAP_PID_INFO m_mapPidInfo;

	typedef std::map<int32, Json::Value> MAP_PID_NODE_INFO;
	MAP_PID_NODE_INFO m_mapPidNodeInfo;

	MD5Context m_szMd5Context;
	std::string m_strCheckSum;

	typedef std::map<int32, int32> MAP_ID_INDEX_INFO;
	MAP_ID_INDEX_INFO m_mapIdIndexInfo;
};