#include "stdafx.h"
#include "UpdateModule.h"
#include "PathHelper.h"
#include "UpdateDefine.h"
#include "StringHelper.h"
#include <string>
#include "ImageHelper.h"
#include "MiscHelper.h"
#include "WebDefine.h"
#include "json/json.h"
#include "XString.h"
#include "MD5Checksum.h"

HMODULE	g_hModule = NULL;

using namespace std;
using namespace update;
using namespace web;

namespace update
{
	UpdateModule*	g_UpdateModule = NULL;
	UpdateModuleFactory*	g_UpdateModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(UpdateModule)
EXPORT_RELEASEMODULEFACTORY(UpdateModule)

UpdateModule::UpdateModule()
{

}

UpdateModule::~UpdateModule()
{

}

BEGIN_EVENT_MAP(UpdateModule)
	ON_EVENT(EVENT_VALUE_UPDATE_CHECK_UPDATEINFO, OnEvent_CheckUpdateInfo)
	ON_EVENT(EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG,OnEvent_UpdateInfoArrive)
	ON_EVENT(EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP, OnEvent_UpdateFileDownloaded)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(UpdateModule)
END_SERVICE_MAP()

//----------------------------------------------------------------------------------------
//����: Load
//����: ������ͨ���÷�����ģ����м���
//����: 
//		@param	pManager			��ģ�����ߵ�ָ��	
//����: 
//		������سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
BOOL UpdateModule::Load(IModuleManager* pManager) 
{
	__super::Load(pManager);

	m_strUpdatePath	=	MiscHelper::GetUpdatePath();

	return TRUE;
}

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* UpdateModule::GetModuleName() 
{
	return L"UpdateModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const UpdateModule::GetModuleId()
{
	return MODULE_ID_DATABASE;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void UpdateModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//����: ProcessMessage
//����: ��ģ����Դ�������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ�������Ϣ�������Ҫ������ֱ�Ӵ���
//����: 
//		@param	msg			��Ҫ����Ĺ㲥��Ϣ
//----------------------------------------------------------------------------------------
void UpdateModule::ProcessMessage(const Message& msg) 
{
}

//----------------------------------------------------------------------------------------
//����: CallDirect
//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
//			����������Ҫͨ�����ߡ�
//����: 
//		@param	lparam			����1
//		@param	rparam			����2
//----------------------------------------------------------------------------------------
int32 UpdateModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void	UpdateModule::OnEvent_CheckUpdateInfo(Event* pEvent)
{
	//�ȼ���Ƿ��Ѿ��������ذ�

	//�ӷ�������ȡ����������Ϣ
	GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_UPDATE>()(EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG, 
		MODULE_ID_WEB));
}

void	UpdateModule::OnEvent_UpdateInfoArrive(Event* pEvent)
{
	Web_CheckUpdateConfigRespEvent* pResp = (Web_CheckUpdateConfigRespEvent*)pEvent->m_pstExtraInfo;
	if( pResp == NULL ||  pResp->eventValue != EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG)
		return;

	if( pResp->param0 != WEB_RET_SUCCESS)
		return;

	m_strUpdateXml = pResp->szUpdateXml;
	ProcessXmlUpdate();
}

void	UpdateModule::OnEvent_UpdateFileDownloaded(Event* pEvent)
{
	Web_DownloadUpdateFileRespEvent* pE = (Web_DownloadUpdateFileRespEvent*)pEvent->m_pstExtraInfo;
	if( pE == NULL || pE->eventValue != EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP)
		return;

	int nId = pE->nId;
	for( int i=0; i<m_vUpdateInfo.size(); i++)
	{
		UPDATEFILEINFO* pUpdateInfo = &m_vUpdateInfo[i];
		if( pUpdateInfo->nId == nId)
		{
			pUpdateInfo->bDownloaded = TRUE;
			break;
		}
	}
}

void	UpdateModule::ProcessXmlUpdate()
{
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(StringHelper::UnicodeToANSI(m_strUpdateXml).c_str(), root, false))
		return;

	// �汾���
	Json::Value& versionNode = root["version"];
	string strLowVersion = versionNode["low_version"].asString();
	string strCurrentVersion = versionNode["high_version"].asString();

	// �ļ����
	Json::Value& updateListNode = root["filelist"];
	ASSERT(updateListNode.isArray());

	wchar_t* wszUpdatePath = MiscHelper::GetUpdatePath();

	for(size_t i=0; i<updateListNode.size(); i++)
	{
		Json::Value& fileNode = updateListNode[i];

		UPDATEFILEINFO	updateInfo;
		updateInfo.nId = StringHelper::ConvertToInt(fileNode["id"].asString());
		updateInfo.strFileName = StringHelper::ANSIToUnicode(fileNode["filename"].asString());
		updateInfo.strMd5 = StringHelper::ANSIToUnicode(fileNode["md5"].asString());
		updateInfo.strLocatePath = StringHelper::ANSIToUnicode(fileNode["locatepath"].asString());
		updateInfo.strDownloadUrl = StringHelper::ANSIToUnicode(fileNode["downloadurl"].asString());

		// �õ����ص��ļ���
		String strUrl = (LPCTSTR)updateInfo.strDownloadUrl.c_str();
		int nInex = strUrl.FindLastCharOf(L"\\/");
		if( nInex == String::NPOS)
			continue;

		strUrl = strUrl.Right(strUrl.GetLength() - nInex - 1);
		wstring wstrPath = wstring(wszUpdatePath) + wstring(L"\\") + strUrl.GetData();
		updateInfo.strTempSavePath = wstrPath;

		// ����ļ��Ƿ���ڣ����Ҽ��MD5
		if( PathHelper::IsFileExist(wstrPath.c_str()) == TRUE)
		{
			string strExistMD5 = CMD5Checksum::GetMD5W(wstrPath);
			if( updateInfo.strMd5 == StringHelper::ANSIToUnicode(strExistMD5))
			{
				updateInfo.bDownloaded = TRUE;
				continue;
			}
		}

		Web_DownloadUpdateFileReqEvent* pEvent = new Web_DownloadUpdateFileReqEvent();
		pEvent->srcMId = MODULE_ID_UPDATE;
		pEvent->nId = updateInfo.nId;
		STRNCPY(pEvent->szUpdateFileUrl, updateInfo.strDownloadUrl.c_str());
		STRNCPY(pEvent->szSavePath, wstrPath.c_str());
		GetModuleManager()->PushEvent(*pEvent);
	}

	free(wszUpdatePath);
}


BOOL UpdateModule::IsHaveUpdatePackage()
{
	std::wstring tempCab = m_strUpdatePath;
	tempCab += _T("*.cab");

	BOOL bExist = PathHelper::IsFileExist(tempCab.c_str());
	return bExist;
}
