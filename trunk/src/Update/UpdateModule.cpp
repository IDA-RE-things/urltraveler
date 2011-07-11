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
#include "UpdateWnd.h"
#include "MainFrameDefine.h"
#include "TrayIconDefine.h"
#include "UIBase.h"

HMODULE	g_hModule = NULL;

// ��ǰ�ͻ��˵İ汾��������
#define INI_FILE_NAME L"UTUpdate.ini"

// ���°�����
#define UPDATE_PACKAGE	L"UpdatePackage.zip"

#ifdef _DEBUG
#define UPDATE_PROGRAM L"UTUpdated.exe"
#else
#define UPDATE_PROGRAM L"UTUpdate.exe"
#endif


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
	m_bDownloading	=	FALSE;

	m_pUpdateWnd	=	NULL;
	m_pUpdateHintWnd	=	NULL;
}

UpdateModule::~UpdateModule()
{
	if( m_pUpdateWnd)
	{
		m_pUpdateWnd->SendMessage(WM_CLOSE, 0, 0);
	}

	if( m_pUpdateHintWnd)
	{
		m_pUpdateHintWnd->SendMessage(WM_CLOSE,0);
	}
}

BEGIN_EVENT_MAP(UpdateModule)
	ON_EVENT(EVENT_VALUE_UPDATE_CHECK_UPDATEINFO, OnEvent_CheckUpdateInfo)
	ON_EVENT(EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP,OnEvent_UpdateInfoArrive)
	ON_EVENT(EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP, OnEvent_UpdateFileDownloaded)
	ON_EVENT(EVENT_VALUE_UPDATE_SHOW_UPDATE_HINT_WND,OnEvent_ShowUpdateInfoWnd)
	ON_EVENT(EVENT_VALUE_UPDATE_SHOW_DOWNLOADING_WND, OnEvent_ShowUpdateDownloadingWnd)
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
	return MODULE_ID_UPDATE;
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
	checkUpdateConfigService.srcMId = GetModuleId();
	GetModuleManager()->CallService(checkUpdateConfigService.serviceId,(param)&checkUpdateConfigService);
}

void	UpdateModule::OnEvent_UpdateInfoArrive(Event* pEvent)
{
	Web_CheckUpdateConfigRespEvent* pResp = (Web_CheckUpdateConfigRespEvent*)pEvent->m_pstExtraInfo;
	if( pResp == NULL ||  pResp->eventValue != EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP)
		return;

	// �������ʧ�ܣ������κε���ʾ
	if( pResp->param0 != WEB_RET_SUCCESS)
		return;

	m_strUpdateXml = pResp->szUpdateXml;
	ProcessUpdateConfig();
}

// ֪ͨ���°��Ѿ����ؽ���
void	UpdateModule::OnEvent_UpdateFileDownloaded(Event* pEvent)
{
	m_bDownloading	=	FALSE;

	Web_DownloadUpdateFileRespEvent* pE = (Web_DownloadUpdateFileRespEvent*)pEvent->m_pstExtraInfo;
	if( pE == NULL || pE->eventValue != EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP)
		return;

	// д�����ʧ��
	if( pE->param0 == web::WEB_RET_COMMON_WRITE_FILE_ERROR)
	{
		if( m_pUpdateWnd && m_pUpdateWnd->GetHWND() != NULL)
		{
			::MessageBox(m_pUpdateWnd->GetHWND(), _T("д����ʧ�ܣ���������Ƿ��п����ÿռ�"), 
				_T("3+�ղؼ����δ�ʦ������"),  MB_ICONQUESTION|MB_OK);
			m_pUpdateWnd->ShowWindow(SW_HIDE);
		}
		else
		{
			::MessageBox(NULL, _T("д����ʧ�ܣ���������Ƿ��п����ÿռ�"), 
				_T("3+�ղؼ����δ�ʦ������"),  MB_ICONQUESTION|MB_OK);
		}
		GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_TRAYICON>()(EVENT_VALUE_CORE_MAIN_LOOP_EXIT, 
			MODULE_ID_CORE));

		return;
	}
	// ���糬ʱ
	else if(pE->param0 == web::WEB_RET_NET_ERROR_TIMEOUT)
	{
		int nRet =0;
		if( m_pUpdateWnd && m_pUpdateWnd->GetHWND() != NULL)
		{
			nRet = ::MessageBox(m_pUpdateWnd->GetHWND(), _T("�����������ʧ��!\r\n�����ȷ������ת�������ֶ��������°汾"), 
				_T("3+�ղؼ����δ�ʦ������"),  MB_ICONQUESTION|MB_OKCANCEL);
			m_pUpdateWnd->ShowWindow(SW_HIDE);
		}
		else
		{
			nRet = ::MessageBox(NULL, _T("�����������ʧ��!\r\n�����ȷ������ת�������ֶ��������°汾"), 
				_T("3+�ղؼ����δ�ʦ������"),  MB_ICONQUESTION|MB_OKCANCEL);
		}

		if(nRet == IDOK)
		{
			ShellExecute(NULL, L"open",L"http://www.baidu.com", NULL,NULL,SW_SHOWMAXIMIZED);
		}

		GetModuleManager()->PushEvent(
			MakeEvent<MODULE_ID_TRAYICON>()(EVENT_VALUE_CORE_MAIN_LOOP_EXIT, 
			MODULE_ID_CORE));

		return;
	}

	if( pE->param0 == web::WEB_RET_SUCCESS)
	{
		if(m_pUpdateWnd && ::IsWindow(m_pUpdateWnd->GetHWND()))
		{
			m_pUpdateWnd->SetDownLoadProgress( 100);

			// �ٴμ��MD5ֵ

			//	����UpdateExe�ļ�
			LaunchUpdateExe();

			GetModuleManager()->PushEvent(
				MakeEvent<MODULE_ID_TRAYICON>()(EVENT_VALUE_CORE_MAIN_LOOP_EXIT, 
				MODULE_ID_CORE));
		}
	}
}

void	UpdateModule::OnEvent_ShowUpdateDownloadingWnd(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_UPDATE_SHOW_DOWNLOADING_WND)
		return;

	Update_ShowUpdateDownloadingEvent* pDownloadEvent = (Update_ShowUpdateDownloadingEvent*)pEvent->m_pstExtraInfo;
	int nLastestVersion	=	 pDownloadEvent->nLastestVersion;
	wstring	wstrUrl	=	pDownloadEvent->szDownloadUrl;
	wstring	wstrPath	=	pDownloadEvent->szSavePath;
	BOOL bForce = pDownloadEvent->bForce;

	Web_DownloadUpdateFileService downloadUpdateFileService;
	downloadUpdateFileService.srcMId = MODULE_ID_UPDATE;
	STRNCPY(downloadUpdateFileService.szUpdateFileUrl, wstrUrl.c_str());
	STRNCPY(downloadUpdateFileService.szSavePath, wstrPath.c_str());
	m_nDownloadSeqNo = GetModuleManager()->CallService(downloadUpdateFileService.serviceId, (param)&downloadUpdateFileService);

	m_bDownloading	=	TRUE;

	// ��������������
	if( m_pUpdateWnd == NULL)
		m_pUpdateWnd = new CUpdateWnd();

	if( m_pUpdateWnd != NULL )
	{ 
		m_pUpdateWnd->SetNewVersion(nLastestVersion);
		m_pUpdateWnd->SetCurrentVersion(MiscHelper::GetCurrentVersion());

		if( bForce == TRUE)
		{
			// ǿ�Ƶ������´��ڽ�������
			mainframe::MainFrame_GetWndService stGetWndService;
			m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWndService);

			CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
			ASSERT(pMainFrameWnd != NULL);

			if( m_pUpdateWnd->GetHWND() == NULL)
			{
				m_pUpdateWnd->Create(*pMainFrameWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
			}
			m_pUpdateWnd->CenterWindow();
			m_pUpdateWnd->SetHintMsg(L"{b}��İ汾̫�ͣ������������ܼ���ʹ�ã�{/b}");
			pMainFrameWnd->ShowModal(*m_pUpdateWnd);
		}
		else
		{
			if( m_pUpdateWnd->GetHWND() == NULL)
			{
				m_pUpdateWnd->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
			}
			m_pUpdateWnd->CenterWindow();

			m_pUpdateWnd->SetHintMsg(L"{b}��⵽�µİ汾�����ڽ���������{/b}");
			m_pUpdateWnd->ShowWindow(true);
		}
	}	
}

void	UpdateModule::OnEvent_ShowUpdateInfoWnd(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_UPDATE_SHOW_UPDATE_HINT_WND)
		return;

	Update_ShowUpdateInfoEvent* pUpdateInfoEvent = (Update_ShowUpdateInfoEvent*)pEvent->m_pstExtraInfo;

	// ��������������
	// ǿ�Ƶ������´��ڽ�������
	if( m_pUpdateHintWnd == NULL)
		m_pUpdateHintWnd = new CUpdateHintWnd();

	if( m_pUpdateHintWnd != NULL )
	{ 
		if( m_pUpdateHintWnd->GetHWND() == NULL)
		{
			m_pUpdateHintWnd->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
		}
	}
	ASSERT(m_pUpdateHintWnd->GetHWND() != NULL);

	m_pUpdateHintWnd->SetDownloadUrl(pUpdateInfoEvent->szDownloadUrl);
	m_pUpdateHintWnd->SetSavePath(pUpdateInfoEvent->szSavePath);
	m_pUpdateHintWnd->SetVersion(MiscHelper::GetStringFromVersion(pUpdateInfoEvent->nVersion + 12));
	m_pUpdateHintWnd->SetSize((float)2.34);

	for( int i=0; i<pUpdateInfoEvent->nUpdateDetailNum; i++)
	{
		m_pUpdateHintWnd->AddUpdateDetail(pUpdateInfoEvent->szUpdateDetail[i]);
	}

	m_pUpdateHintWnd->BeginToShowUpdateInfoWnd();
}

void	UpdateModule::ProcessUpdateConfig()
{
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(StringHelper::UnicodeToANSI(m_strUpdateXml).c_str(), root, false))
		return;

	// �汾���
	Json::Value& versionNode = root["version"];
	string& strLowVersion = versionNode["low_version"].asString();
	string& strCurrentVersion = versionNode["high_version"].asString();
	int nLowVersion = MiscHelper::GetVersionFromString(strLowVersion.c_str());
	int nHighVersion = MiscHelper::GetVersionFromString(strCurrentVersion.c_str());

	// ������ϸ���
	Json::Value& detailNode = root["updateinfo"];
	ASSERT(detailNode.isArray() == true);

	int nNodeNum = detailNode.size();
	if( nNodeNum > MAX_UPDATE_DETAIL_NUM)
		nNodeNum = MAX_UPDATE_DETAIL_NUM;

	std::vector<wstring>	vDetail;
	for( int i=0; i<nNodeNum; i++)
	{
		Json::Value& detail = detailNode[i];
		string& strDetail = detail["item"].asString();
		vDetail.push_back(StringHelper::Utf8ToUnicode(strDetail));
	}

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
			pEvent->desMId	=	MODULE_ID_WEB;
			pEvent->srcMId	=	MODULE_ID_UPDATE;
			pEvent->param0 = web::WEB_RET_SUCCESS;
			GetModuleManager()->PushEvent(*pEvent);

			return;
		}
	}

	m_strUpdateFileName =	 wstring(wszUpdatePath) + updateInfo.strFileName;

	free(wszUpdatePath);

	// ��ʱ��ǿ�Ƹ���
	int nCurrentVersion = MiscHelper::GetCurrentVersion();
	if( nCurrentVersion <= nLowVersion)
	{
		Update_ShowUpdateDownloadingEvent* pEvent = new Update_ShowUpdateDownloadingEvent();
		pEvent->srcMId = MODULE_ID_UPDATE;
		pEvent->nLastestVersion = nHighVersion;
		STRNCPY(pEvent->szDownloadUrl, updateInfo.strDownloadUrl.c_str());
		STRNCPY(pEvent->szSavePath, updateInfo.strTempSavePath.c_str());
		pEvent->bForce = TRUE;
		GetModuleManager()->PushEvent(*pEvent);
	}
	else 
	{
		ASSERT( nCurrentVersion <= nHighVersion);

		// ������ʾ��ʾ�û��Ƿ���Ҫ���и���

		// ����û��Ѿ�������Ĭ�ϸ��£��򲻽����κε���ʾ

		// ���½ǵ���������ʾ��
		Update_ShowUpdateInfoEvent* pEvent = new Update_ShowUpdateInfoEvent();
		pEvent->srcMId = MODULE_ID_UPDATE;
		pEvent->nVersion = nHighVersion;
		STRNCPY(pEvent->szDownloadUrl, updateInfo.strDownloadUrl.c_str());
		STRNCPY(pEvent->szSavePath, updateInfo.strTempSavePath.c_str());

		pEvent->nUpdateDetailNum = vDetail.size();
		for( size_t i=0; i<vDetail.size();i++)
		{
			STRNCPY(pEvent->szUpdateDetail[i], vDetail[i].c_str());
		}

		GetModuleManager()->PushEvent(*pEvent);
	}
}

BOOL UpdateModule::IsHaveUpdatePackage()
{
	return TRUE;
}

void UpdateModule::QueryDownloadUpdateFileProcess()
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
		if(m_pUpdateWnd && ::IsWindow(m_pUpdateWnd->GetHWND()))
		{
			m_pUpdateWnd->SetDownLoadProgress( queryProcessService.uPercent);
		}
	}
}

void	UpdateModule::LaunchUpdateExe()
{
	std::wstring strUpdateExe = PathHelper::GetModuleDir();
	strUpdateExe += UPDATE_PROGRAM;
	if (PathHelper::IsFileExist(strUpdateExe.c_str()))
	{
		// ����
		std::wstring strParam;
		strParam += L"\"";
		strParam += m_strUpdateFileName;
		strParam += L"\"";

		ShellExecuteW(NULL, _T("open"), strUpdateExe.c_str(), strParam.c_str(), NULL, SW_SHOWNORMAL);
	}
	else
	{
		//�޷��ҵ����³���
		ASSERT(L"�޷��ҵ�URUpdate.exe����");
	}
}

void UpdateModule::OnMessage_CycleTrigged(Message* pMessage)
{
	QueryDownloadUpdateFileProcess();
}