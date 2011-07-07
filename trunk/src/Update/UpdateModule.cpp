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
#include "UpdateWnd.h";
#include "MainFrameDefine.h"
#include "TrayIconDefine.h"

HMODULE	g_hModule = NULL;

using namespace std;
using namespace update;
using namespace web;
using namespace mainframe;
using namespace trayicon;

namespace update
{
	UpdateModule*	g_UpdateModule = NULL;
	UpdateModuleFactory*	g_UpdateModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(UpdateModule)
EXPORT_RELEASEMODULEFACTORY(UpdateModule)

UpdateModule::UpdateModule()
{
	m_nDownloadSeqNo	=	INVALID_SEQNO;
	m_pUpdateWnd	=	NULL;
	m_bDownloading	=	FALSE;
}

UpdateModule::~UpdateModule()
{
	if( m_pUpdateWnd)
	{
		m_pUpdateWnd->SendMessage(WM_CLOSE, 0, 0);
	}
}

BEGIN_EVENT_MAP(UpdateModule)
	ON_EVENT(EVENT_VALUE_UPDATE_CHECK_UPDATEINFO, OnEvent_CheckUpdateInfo)
	ON_EVENT(EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP,OnEvent_UpdateInfoArrive)
	ON_EVENT(EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP, OnEvent_UpdateFileDownloaded)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(UpdateModule)
END_SERVICE_MAP()

BEGIN_MESSAGE_MAP(UpdateModule)
	ON_MESSAGE(MESSAGE_VALUE_CORE_CYCLE_TRIGGER, OnMessage_CycleTrigged)
END_MESSAGE_MAP()


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
//����: ��ģ����Դ��������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ�������¼�
//----------------------------------------------------------------------------------------
void UpdateModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//����: ProcessMessage
//����: ��ģ����Դ��������еĹ㲥�¼���ĳ��ģ��㲥һ����Ϣ�����߽��Ѹ���Ϣ�㲥
//			�����е�ģ�飬ÿ��ģ�����Լ��Ƿ���Ҫ��������Ϣ�������Ҫ��������ֱ�Ӵ���
//����: 
//		@param	msg			��Ҫ�����Ĺ㲥��Ϣ
//----------------------------------------------------------------------------------------
void UpdateModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
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
	Web_CheckUpdateConfigService checkUpdateConfigService;
	checkUpdateConfigService.srcMId = MODULE_ID_UPDATE;
	GetModuleManager()->CallService(checkUpdateConfigService.serviceId,(param)&checkUpdateConfigService);
}

void	UpdateModule::OnEvent_UpdateInfoArrive(Event* pEvent)
{
	Web_CheckUpdateConfigRespEvent* pResp = (Web_CheckUpdateConfigRespEvent*)pEvent->m_pstExtraInfo;
	if( pResp == NULL ||  pResp->eventValue != EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP)
		return;

	if( pResp->param0 != WEB_RET_SUCCESS)
		return;

	m_strUpdateXml = pResp->szUpdateXml;
	ProcessUpdateConfig();
}

void	UpdateModule::OnEvent_UpdateFileDownloaded(Event* pEvent)
{
	m_bDownloading	=	FALSE;

	if(m_pUpdateWnd)
	{
		m_pUpdateWnd->SetDownLoadProgress( 100);
	}

	Web_DownloadUpdateFileRespEvent* pE = (Web_DownloadUpdateFileRespEvent*)pEvent->m_pstExtraInfo;
	if( pE == NULL || pE->eventValue != EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP)
		return;

	//	����UpdateExe�ļ�
}

void	UpdateModule::ProcessUpdateConfig()
{
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(StringHelper::UnicodeToANSI(m_strUpdateXml).c_str(), root, false))
		return;

	// �汾���
	Json::Value& versionNode = root["version"];
	string strLowVersion = versionNode["low_version"].asString();
	string strCurrentVersion = versionNode["high_version"].asString();

	int nLowVersion = MiscHelper::GetVersionFromString(strLowVersion.c_str());
	int nHighVersion = MiscHelper::GetVersionFromString(strCurrentVersion.c_str());

	// �ļ����
	Json::Value& updateFileNode = root["package"];

	wchar_t* wszUpdatePath = MiscHelper::GetUpdatePath();

	UPDATEFILEINFO	updateInfo;
	updateInfo.strFileName = StringHelper::ANSIToUnicode(updateFileNode["filename"].asString());
	updateInfo.strMd5 = StringHelper::ANSIToUnicode(updateFileNode["md5"].asString());
	updateInfo.strDownloadUrl = StringHelper::ANSIToUnicode(updateFileNode["downloadurl"].asString());

	// �õ����ص��ļ���
	String strUrl = (LPCTSTR)updateInfo.strDownloadUrl.c_str();
	int nInex = strUrl.FindLastCharOf(L"\\/");
	if( nInex == String::NPOS)
		return;

	strUrl = strUrl.Right(strUrl.GetLength() - nInex - 1);
	wstring wstrPath = wstring(wszUpdatePath) + wstring(L"\\") + strUrl.GetData();
	updateInfo.strTempSavePath = wstrPath;

	// ����ļ��Ƿ���ڣ����Ҽ��MD5
	if( PathHelper::IsFileExist(wstrPath.c_str()) == TRUE)
	{
		string strExistMD5 = CMD5Checksum::GetMD5W(wstrPath);
		if( updateInfo.strMd5 == StringHelper::ANSIToUnicode(strExistMD5))
		{
			m_strUpdateFileName	=	wstrPath;

			// ֱ��֪ͨ����ok
			Web_DownloadUpdateFileRespEvent* pEvent = new Web_DownloadUpdateFileRespEvent();
			pEvent->desMId	=	MODULE_ID_UPDATE;
			pEvent->srcMId	=	MODULE_ID_WEB;
			pEvent->param0 = web::WEB_RET_SUCCESS;
			GetModuleManager()->PushEvent(*pEvent);

			return;
		}
	}

	free(wszUpdatePath);

	// ��ʱ��ǿ�Ƹ���
	int nCurrentVersion = MiscHelper::GetCurrentVersion();
	if( nCurrentVersion <= nLowVersion)
	{
		Web_DownloadUpdateFileService downloadUpdateFileService;
		downloadUpdateFileService.srcMId = MODULE_ID_UPDATE;
		STRNCPY(downloadUpdateFileService.szUpdateFileUrl, updateInfo.strDownloadUrl.c_str());
		STRNCPY(downloadUpdateFileService.szSavePath, wstrPath.c_str());
		m_nDownloadSeqNo = GetModuleManager()->CallService(downloadUpdateFileService.serviceId, (param)&downloadUpdateFileService);

		// ǿ�Ƶ������´��ڽ�������
		mainframe::MainFrame_GetWndService stGetWndService;
		m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWndService);

		CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
		ASSERT(pMainFrameWnd != NULL);

		m_bDownloading	=	TRUE;

		// ��������������
		if( m_pUpdateWnd == NULL)
			m_pUpdateWnd = new CUpdateWnd();

		if( m_pUpdateWnd != NULL )
		{ 
			if( m_pUpdateWnd->GetHWND() == NULL)
			{
				m_pUpdateWnd->Create(*pMainFrameWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
			}
			m_pUpdateWnd->CenterWindow();
			pMainFrameWnd->ShowModal(*m_pUpdateWnd);
		}
	}
	else 
	{
		ASSERT( nCurrentVersion <= nHighVersion);

		//	������ʾ��ʾ�û��Ƿ���Ҫ���и���
		
		// ����û��Ѿ�������Ĭ�ϸ��£��򲻽����κε���ʾ
		
		// ���½ǵ���������ʾ��

		TrayIcon_ShowUpdateWndEvent* pEvent = new TrayIcon_ShowUpdateWndEvent();
		pEvent->srcMId = MODULE_ID_UPDATE;
		pEvent->nNewestVersion	=	nHighVersion;
		GetModuleManager()->PushEvent(*pEvent);
	}
}


BOOL UpdateModule::IsHaveUpdatePackage()
{
	std::wstring tempCab = m_strUpdatePath;
	tempCab += _T("*.cab");

	BOOL bExist = PathHelper::IsFileExist(tempCab.c_str());
	return bExist;
}

void UpdateModule::OnMessage_CycleTrigged(Message* pMessage)
{
	if( m_bDownloading == FALSE)
		return;

	// ��ѯ����
	web::Web_QueryDownFileProcessService queryProcessService;
	queryProcessService.bAllowOffline = true;
	queryProcessService.srcMId = MODULE_ID_UPDATE;
	queryProcessService.nSeqNo = m_nDownloadSeqNo;
	queryProcessService.uPercent = 0;
	queryProcessService.uTotalLength = 0;
	queryProcessService.uFinishedLength = 0;

	if (INVALID_SEQNO != GetModuleManager()->CallService(queryProcessService.serviceId, (param)&queryProcessService))
	{
		if(m_pUpdateWnd)
		{
			m_pUpdateWnd->SetDownLoadProgress( queryProcessService.uPercent);
		}
	}
}