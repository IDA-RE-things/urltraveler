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
	//����: Load 
	//����: �����Load��������Ҫ�������ݵȳ�ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL Load();

	//----------------------------------------------------------------------------------------
	//����: UnLoad 
	//����: �����UnLoad��������Ҫ�������ݵȷ���ʼ�� 
	//����: ��ʼ���ɹ�����TRUE����ʼ��ʧ�ܷ���FALSE��
	//----------------------------------------------------------------------------------------
	virtual BOOL UnLoad();

	//----------------------------------------------------------------------------------------
	//����: GetPlugInVersion
	//����: ��ȡ��ǰ����İ汾��
	//����: ����İ汾�ţ�ͨ��Ϊһ������
	//----------------------------------------------------------------------------------------
	virtual int32 GetPlugInVersion() ;

	//----------------------------------------------------------------------------------------
	//����: GetBrowserName
	//����: ��ȡ�ò����Ӧ������������ƺͰ汾
	//----------------------------------------------------------------------------------------
	virtual const wchar_t* GetBrowserName() ;

	//----------------------------------------------------------------------------------------
	//����: GetInstallPath
	//����: ��ȡ�����Ӧ��������İ�װĿ¼
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetInstallPath() ;

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteDataPath
	//����: ��ȡ������ղؼж�Ӧ���ļ������ļ��е�·��
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetFavoriteDataPath() ;

	//----------------------------------------------------------------------------------------
	//����: GetHistoryDataPath
	//����: ��ȡ������ղؼе���ʷ���ݶ�Ӧ���ļ������ļ���
	//----------------------------------------------------------------------------------------
	virtual wchar_t* GetHistoryDataPath() ;

	//----------------------------------------------------------------------------------------
	//����: ExportFavoriteData
	//����: ������ǰ����������е��ղؼ�����
	//����: 
	//		@param	pData			�������ղؼ���������
	//		@param	nDataNum		�������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum);

	//----------------------------------------------------------------------------------------
	//����: ImportFavoriteData
	//����: ����ǰ�ļ�¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA* ppData, int32& nDataNum);

	//----------------------------------------------------------------------------------------
	//����: ImportFavoriteData
	//����: ��һ����¼���뵽�������
	//����: 
	//		@param	stData			��Ҫ����ĵ��ղؼм�¼
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(FAVORITELINEDATA stData);

	//----------------------------------------------------------------------------------------
	//����: GetFavoriteCount
	//����: ��ȡ��������ղ���ַ������ 
	//����:
	//      �ط��ղ���ַ����
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