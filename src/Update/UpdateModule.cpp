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

	m_strUpdatePath	=	MiscHelper::GetUpdatePath();

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
	return MODULE_ID_DATABASE;
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
	checkUpdateConfigService.srcId = MODULE_ID_UPDATE;
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

	//	启动UpdateExe文件
}

void	UpdateModule::ProcessUpdateConfig()
{
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(StringHelper::UnicodeToANSI(m_strUpdateXml).c_str(), root, false))
		return;

	// 版本结点
	Json::Value& versionNode = root["version"];
	string strLowVersion = versionNode["low_version"].asString();
	string strCurrentVersion = versionNode["high_version"].asString();

	int nLowVersion = MiscHelper::GetVersionFromString(strLowVersion.c_str());
	int nHighVersion = MiscHelper::GetVersionFromString(strCurrentVersion.c_str());

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

	// 检查文件是否存在，并且检查MD5
	if( PathHelper::IsFileExist(wstrPath.c_str()) == TRUE)
	{
		string strExistMD5 = CMD5Checksum::GetMD5W(wstrPath);
		if( updateInfo.strMd5 == StringHelper::ANSIToUnicode(strExistMD5))
		{
			m_strUpdateFileName	=	wstrPath;

			// 直接通知下载ok
			Web_DownloadUpdateFileRespEvent* pEvent = new Web_DownloadUpdateFileRespEvent();
			pEvent->desMId	=	MODULE_ID_UPDATE;
			pEvent->srcMId	=	MODULE_ID_WEB;
			pEvent->param0 = web::WEB_RET_SUCCESS;
			GetModuleManager()->PushEvent(*pEvent);

			return;
		}
	}

	free(wszUpdatePath);

	// 此时是强制更新
	int nCurrentVersion = MiscHelper::GetCurrentVersion();
	if( nCurrentVersion <= nLowVersion)
	{
		Web_DownloadUpdateFileService downloadUpdateFileService;
		downloadUpdateFileService.srcId = MODULE_ID_UPDATE;
		STRNCPY(downloadUpdateFileService.szUpdateFileUrl, updateInfo.strDownloadUrl.c_str());
		STRNCPY(downloadUpdateFileService.szSavePath, wstrPath.c_str());
		m_nDownloadSeqNo = GetModuleManager()->CallService(downloadUpdateFileService.serviceId, (param)&downloadUpdateFileService);

		// 强制弹出更新窗口进行升级
		mainframe::MainFrame_GetWndService stGetWndService;
		m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWndService);

		CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
		ASSERT(pMainFrameWnd != NULL);

		m_bDownloading	=	TRUE;

		// 启动进度条界面
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

		//	弹出提示提示用户是否需要进行更新
		
		// 如果用户已经设置了默认更新，则不进行任何的提示
		
		// 右下角弹出更新提示框

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

	// 查询进度
	web::Web_QueryDownFileProcessService queryProcessService;
	queryProcessService.bAllowOffline = true;
	queryProcessService.srcId = MODULE_ID_UPDATE;
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