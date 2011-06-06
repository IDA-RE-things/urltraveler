#pragma once
#include "PlugIn.h"
#include "json/json.h"
#include <vector>
#include <map>

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
	virtual BOOL ExportFavoriteData(PFAVORITELINEDATA pData, int32& nDataNum);

	//----------------------------------------------------------------------------------------
	//����: ImportFavoriteData
	//����: ����ǰ�ļ�¼���뵽�������
	//����: 
	//		@param	pData			��Ҫ����ĵ��ղؼ���������
	//		@param	nDataNum		��Ҫ������ղؼ���Ŀ������
	//----------------------------------------------------------------------------------------
	virtual BOOL ImportFavoriteData(PFAVORITELINEDATA pData, int32 nDataNum);

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
	BOOL ExportFolder(Json::Value& folder_obj, int32 nPid, PFAVORITELINEDATA pData, int32& nDataNum);
	BOOL ExportUrl(Json::Value& url_obj, int32 nPid, PFAVORITELINEDATA pData, int32& nDataNum);
	BOOL MakeFolderNode(FAVORITELINEDATA stData, Json::Value& folder_obj, int32& nIndex);
	BOOL MakeUrlNode(FAVORITELINEDATA stData, Json::Value& url_obj, int32& nIndex);

	BOOL MakeSpecialFolderNode(wchar_t *pszName, int32& nIndex, Json::Value& folder_obj);
	BOOL EnumNode(Json::Value& folder_obj, int32& nCount);

	BOOL TraverseNode(PFAVORITELINEDATA pData, int32 nDepth);

private:
	typedef struct NodeInfo
	{
		int32 nIndex;
		Json::Value node_obj;
	} NODEINFO, *PNODEINFO;

	int32 m_nMaxDepth;
	int32 m_nIndex;

	typedef std::multimap<int32, int32> MAP_DEPTH_INFO;
	MAP_DEPTH_INFO m_mapDepthInfo;

	typedef std::map<int32, int32> MAP_PID_INFO;
	MAP_PID_INFO m_mapPidInfo;

	typedef std::map<int32, Json::Value> MAP_PID_NODE_INFO;
	MAP_PID_NODE_INFO m_mapPidNodeInfo;
};