#pragma once
#include "PlugIn.h"
#include "json/json.h"

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
	BOOL MakeFolderNode(CHROMEFOLDERNODE stFolderNode, Json::Value& folder_obj);
	BOOL MakeUrlNode(CHROMEURLNODE stUrlNode, Json::Value& url_obj);

	BOOL MakeSpecialFolderNode(wchar_t *pszName, int32& nIndex, Json::Value& folder_obj);
	BOOL EnumNode(Json::Value& folder_obj, int32& nCount);
};