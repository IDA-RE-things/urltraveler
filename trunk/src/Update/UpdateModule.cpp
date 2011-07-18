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

// 当前客户端的版本描述配置
#define INI_FILE_NAME L"UTUpdate.ini"

// 更新包名称
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
	ON_EVENT(EVENT_VALUE_UPDATE_SILENCE_UPDATE, OnEvent_BeginToSilenceUpdate)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(UpdateModule)
END_SERVICE_MAP()

BEGIN_MESSAGE_MAP(UpdateModule)
	ON_MESSAGE(MESSAGE_VALUE_CORE_CYCLE_TRIGGER, OnMessage_CycleTrigged)
END_MESSAGE_MAP()


//----------------------------------------------------------------------------------------
//名称: Load
//描述: 主程序通过该方法对模块进行加载
//参数: 
//		@param	pManager			主模块总线的指针	
//返回: 
//		如果加载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
BOOL UpdateModule::Load(IModuleManager* pManager) 
{
	__super::Load(pManager);
	return TRUE;
}

//----------------------------------------------------------------------------------------
//名称: GetModuleName
//描述: 主程序通过该方法获取当前模块的名字，每一个模块都有一个唯一的名字
//返回: 
//		如果卸载成功，返回TRUE，否则返回FALSE
//----------------------------------------------------------------------------------------
const wchar_t* UpdateModule::GetModuleName() 
{
	return L"UpdateModule";
}

//----------------------------------------------------------------------------------------
//名称: GetModuleId
//描述: 主程序通过该方法获取当前模块的ID，每一个模块都会分配一个唯一的ID
//返回: 
//		返回该模块的唯一的ID
//----------------------------------------------------------------------------------------
uint32 const UpdateModule::GetModuleId()
{
	return MODULE_ID_UPDATE;
}

//----------------------------------------------------------------------------------------
//名称: ProcessEvent
//描述: 该模块可以处理的所有的点对点事件
//参数: 
//		@param	evt			需要处理的事件
//----------------------------------------------------------------------------------------
void UpdateModule::ProcessEvent(const Event& evt)
{
	PROCESS_EVENT(evt);
}

//----------------------------------------------------------------------------------------
//名称: ProcessMessage
//描述: 该模块可以处理的所有的广播事件。某个模块广播一个消息后，总线将把该消息广播
//			给所有的模块，每个模块检查自己是否需要处理该消息，如果需要处理，则直接处理
//参数: 
//		@param	msg			需要处理的广播消息
//----------------------------------------------------------------------------------------
void UpdateModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
}

//----------------------------------------------------------------------------------------
//名称: CallDirect
//描述: 与Event和Message需要通过总线中转不同，某个模块可以直接调用另外一个模块中的
//			方法而不需要通过总线。
//参数: 
//		@param	lparam			参数1
//		@param	rparam			参数2
//----------------------------------------------------------------------------------------
int32 UpdateModule::CallDirect(const ServiceValue lServiceValue, param wparam) 
{
	CALL_DIRECT(lServiceValue, wparam);
}

void	UpdateModule::OnEvent_CheckUpdateInfo(Event* pEvent)
{
	//先检查是否已经存在下载包

	//从服务器读取更新配置信息
	Web_CheckUpdateConfigService checkUpdateConfigService;
	checkUpdateConfigService.srcMId = GetModuleId();
	GetModuleManager()->CallService(checkUpdateConfigService.serviceId,(param)&checkUpdateConfigService);
}

void	UpdateModule::OnEvent_UpdateInfoArrive(Event* pEvent)
{
	Web_CheckUpdateConfigRespEvent* pResp = (Web_CheckUpdateConfigRespEvent*)pEvent->m_pstExtraInfo;
	if( pResp == NULL ||  pResp->eventValue != EVENT_VALUE_WEB_CHECK_UPDATE_CONFIG_RESP)
		return;

	// 如果更新失败，则不作任何的提示
	if( pResp->param0 != WEB_RET_SUCCESS)
		return;

	m_strUpdateXml = pResp->szUpdateXml;
	ProcessUpdateConfig();
}

void	UpdateModule::OnEvent_BeginToSilenceUpdate(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_UPDATE_SILENCE_UPDATE)
		return;

	Update_SilencetUpdateEvent* pUpdateEvent = (Update_SilencetUpdateEvent*)pEvent;
	wstring	wstrUrl	=	pUpdateEvent->szDownloadUrl;
	wstring	wstrPath	=	pUpdateEvent->szSavePath;   
	wstring	wstrMD5 = pUpdateEvent->szMD5;

	// 检查文件是否存在，并且检查MD5
	if( PathHelper::IsFileExist(m_strUpdateFileName) == TRUE)
	{
		string strExistMD5 = CMD5Checksum::GetMD5W(m_strUpdateFileName);
		if( String(wstrMD5.c_str()).ToLower() == String(StringHelper::ANSIToUnicode(strExistMD5).c_str()).ToLower())
		{
			// 如果已经存在，则记录是否需要更新
			return;
		}
	}

	Web_DownloadUpdateFileService downloadUpdateFileService;
	downloadUpdateFileService.srcMId = MODULE_ID_UPDATE;
	STRNCPY(downloadUpdateFileService.szUpdateFileUrl, wstrUrl.c_str());
	STRNCPY(downloadUpdateFileService.szSavePath, wstrPath.c_str());
	m_nDownloadSeqNo = GetModuleManager()->CallService(downloadUpdateFileService.serviceId, 
		(param)&downloadUpdateFileService);	
}

// 通知更新包已经下载结束
void	UpdateModule::OnEvent_UpdateFileDownloaded(Event* pEvent)
{
	m_bDownloading	=	FALSE;

	if( pEvent == NULL)
		return;

	// 写入磁盘失败	
	if( pEvent->param0 != WEB_RET_SUCCESS)
	{
		if( pEvent->param0 == web::WEB_RET_COMMON_WRITE_FILE_ERROR)
		{
			::MessageBox(NULL, _T("写磁盘失败，请检查磁盘是否有可以用空间"), 
				_T("EverFav 云端收藏夹升级向导"),  MB_ICONQUESTION|MB_OK);
		}
		// 网络超时
		else if(pEvent->param0 == web::WEB_RET_NET_ERROR_TIMEOUT)
		{
			int nRet = ::MessageBox(NULL, _T("您的网络连接可能有问题，下载失败!\r\n点击“确定”跳转到官网手动下载最新版本"), 
				_T("EverFav 云端收藏夹升级向导"),  MB_ICONQUESTION|MB_OKCANCEL);
			if(nRet == IDOK)
			{
				ShellExecute(NULL, L"open",L"http://www.baidu.com", NULL,NULL,SW_SHOWMAXIMIZED);
			}
		}
		else if(pEvent->param0 != web::WEB_RET_SUCCESS)
		{
			::MessageBox(m_pUpdateWnd->GetHWND(), _T("更新发生未知故障"), 
				_T("EverFav 云端收藏夹升级向导"),  MB_ICONQUESTION|MB_OK);
		}

		m_pUpdateWnd->Close();
		return;
	}

	Web_DownloadUpdateFileRespEvent* pE = (Web_DownloadUpdateFileRespEvent*)pEvent->m_pstExtraInfo;
	if( pE == NULL || pE->eventValue != EVENT_VALUE_WEB_DOWNLOAD_UPDATE_FILE_RESP)
		return;

	if(m_pUpdateWnd && ::IsWindow(m_pUpdateWnd->GetHWND()))
	{
		m_pUpdateWnd->SetDownLoadProgress( 100);
		m_pUpdateWnd->Close();

		// 再次检查MD5值
		LaunchUpdateExe();
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
	wstring	wstrMD5 = pDownloadEvent->szMD5;
	BOOL bForce = pDownloadEvent->bForce;

	// 检查文件是否存在，并且检查MD5
	if( PathHelper::IsFileExist(m_strUpdateFileName) == TRUE)
	{
		string strExistMD5 = CMD5Checksum::GetMD5W(m_strUpdateFileName);
		if( String(wstrMD5.c_str()).ToLower() == String(StringHelper::ANSIToUnicode(strExistMD5).c_str()).ToLower())
		{
			// 直接执行启动
			LaunchUpdateExe();
			return;
		}
	}

	Web_DownloadUpdateFileService downloadUpdateFileService;
	downloadUpdateFileService.srcMId = MODULE_ID_UPDATE;
	STRNCPY(downloadUpdateFileService.szUpdateFileUrl, wstrUrl.c_str());
	STRNCPY(downloadUpdateFileService.szSavePath, wstrPath.c_str());
	m_nDownloadSeqNo = GetModuleManager()->CallService(downloadUpdateFileService.serviceId, (param)&downloadUpdateFileService);

	m_bDownloading	=	TRUE;

	// 启动进度条界面
	if( m_pUpdateWnd == NULL)
		m_pUpdateWnd = new CUpdateWnd();

	if( m_pUpdateWnd != NULL )
	{ 
		m_pUpdateWnd->SetNewVersion(nLastestVersion);
		m_pUpdateWnd->SetCurrentVersion(MiscHelper::GetCurrentVersion());

		// 强制弹出更新窗口进行升级
		mainframe::MainFrame_GetWndService stGetWndService;
		m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWndService);

		CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
		ASSERT(pMainFrameWnd != NULL);
		pMainFrameWnd->ShowWindow(SW_NORMAL);

		if( m_pUpdateWnd->GetHWND() == NULL)
		{
			m_pUpdateWnd->Create(*pMainFrameWnd, _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
		}
		m_pUpdateWnd->CenterWindow();

		if( bForce == TRUE)
		{
			m_pUpdateWnd->SetHintMsg(L"{b}你的版本太低，必须升级才能继续使用！{/b}");
			pMainFrameWnd->ShowModal(*m_pUpdateWnd);
		}
		else
		{
			m_pUpdateWnd->SetHintMsg(L"{b}检测到新的版本，正在进行升级！{/b}");
			pMainFrameWnd->ShowModal(*m_pUpdateWnd);
		}
	}	
}

void	UpdateModule::OnEvent_ShowUpdateInfoWnd(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_UPDATE_SHOW_UPDATE_HINT_WND)
		return;

	Update_ShowUpdateInfoEvent* pUpdateInfoEvent = (Update_ShowUpdateInfoEvent*)pEvent->m_pstExtraInfo;

	// 启动进度条界面
	// 强制弹出更新窗口进行升级
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

	m_pUpdateHintWnd->SetMD5(pUpdateInfoEvent->szMD5);
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

	// 版本结点
	Json::Value& versionNode = root["version"];
	string& strLowVersion = versionNode["low_version"].asString();
	string&	strHighestVersion = versionNode["high_version"].asString();
	string& strToUpdateVersion = versionNode["version"].asString();
	int nLowVersion = MiscHelper::GetVersionFromString(strLowVersion.c_str());
	int nHighVersion = MiscHelper::GetVersionFromString(strHighestVersion.c_str());
	int nToUpdateVersion = MiscHelper::GetVersionFromString(strToUpdateVersion.c_str());
	ASSERT( nToUpdateVersion <= nHighVersion);

	m_nHighestVersion = nHighVersion;

	// 更新明细结点
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

	// 文件结点
	Json::Value& updateFileNode = root["package"];
	wchar_t* wszUpdatePath = MiscHelper::GetUpdatePath();

	UPDATEFILEINFO	updateInfo;
	updateInfo.strFileName = StringHelper::ANSIToUnicode(updateFileNode["filename"].asString());
	updateInfo.strMd5 = StringHelper::ANSIToUnicode(updateFileNode["md5"].asString());
	updateInfo.strDownloadUrl = StringHelper::ANSIToUnicode(updateFileNode["downloadurl"].asString());

	// 得到下载的文件名
	String strUrl = (LPCTSTR)updateInfo.strDownloadUrl.c_str();
	int nInex = strUrl.FindLastCharOf(L"\\/");
	if( nInex == String::NPOS)
		return;

	strUrl = strUrl.Right(strUrl.GetLength() - nInex - 1);
	wstring wstrPath = wstring(wszUpdatePath) + wstring(L"\\") + strUrl.GetData();
	updateInfo.strTempSavePath = wstrPath;

	m_strUpdateFileName =	 wstring(wszUpdatePath) + updateInfo.strFileName;

	free(wszUpdatePath);

	// 此时是强制更新
	int nCurrentVersion = MiscHelper::GetCurrentVersion();
	if( nCurrentVersion <= nLowVersion)
	{
		Update_ShowUpdateDownloadingEvent* pEvent = new Update_ShowUpdateDownloadingEvent();
		pEvent->srcMId = MODULE_ID_UPDATE;
		pEvent->nLastestVersion = nHighVersion;
		STRNCPY(pEvent->szDownloadUrl, updateInfo.strDownloadUrl.c_str());
		STRNCPY(pEvent->szSavePath, updateInfo.strTempSavePath.c_str());
		STRNCPY(pEvent->szMD5, updateInfo.strMd5.c_str());
		pEvent->bForce = TRUE;
		GetModuleManager()->PushEvent(*pEvent);
	}
	else  if( nCurrentVersion < nToUpdateVersion)
	{
		// 弹出提示提示用户是否需要进行更新

		// 如果用户已经设置了默认更新，则不进行任何的提示

		// 右下角弹出更新提示框
		Update_ShowUpdateInfoEvent* pEvent = new Update_ShowUpdateInfoEvent();
		pEvent->srcMId = MODULE_ID_UPDATE;
		pEvent->nVersion = nHighVersion;
		STRNCPY(pEvent->szDownloadUrl, updateInfo.strDownloadUrl.c_str());
		STRNCPY(pEvent->szSavePath, updateInfo.strTempSavePath.c_str());
		STRNCPY(pEvent->szMD5, updateInfo.strMd5.c_str());
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

	// 查询进度
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
	//	启动UpdateExe文件
	GetModuleManager()->PushEvent(
		MakeEvent<MODULE_ID_TRAYICON>()(EVENT_VALUE_CORE_MAIN_LOOP_EXIT, 
		MODULE_ID_CORE));

	std::wstring strUpdateExe = PathHelper::GetModuleDir();
	strUpdateExe += UPDATE_PROGRAM;
	if (PathHelper::IsFileExist(strUpdateExe.c_str()))
	{
		// 参数
		std::wstring strParam = PathHelper::GetModulePath();
		strParam += L" ";
		strParam += StringHelper::ANSIToUnicode(StringHelper::ConvertFromInt(m_nHighestVersion));
		ShellExecuteW(NULL, _T("open"), strUpdateExe.c_str(), strParam.c_str(), NULL, SW_SHOWNORMAL);
	}
	else
	{
		//无法找到更新程序
		ASSERT(L"无法找到URUpdate.exe程序");
	}
}

void UpdateModule::OnMessage_CycleTrigged(Message* pMessage)
{
	QueryDownloadUpdateFileProcess();
}