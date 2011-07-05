#include "stdafx.h"
#include "MainFrameModule.h"
#include "MainFrameDefine.h"
#include "MainFrameWnd.h"
#include "TrayIconDefine.h"
#include "DataCenterDefine.h"
#include "PlugInDefine.h"
#include "DatabaseDefine.h"
#include "WebDefine.h"

using namespace mainframe;
using namespace datacenter;
using namespace plugin;
using namespace web;

HMODULE	g_hModule = NULL;


namespace mainframe
{
	MainFrameModule*	g_MainFrameModule = NULL;
	MainFrameModuleFactory*	g_MainFrameModuleFactory = NULL;
}

EXPORT_GETMODULEFACTORY(MainFrameModule)
EXPORT_RELEASEMODULEFACTORY(MainFrameModule)

MainFrameModule::MainFrameModule()
{
	m_pMainFrame	=	NULL;
}

MainFrameModule::~MainFrameModule()
{
	::CoUninitialize();

	if( m_pMainFrame)
	{
		m_pMainFrame->SendMessage(WM_CLOSE, 0, 0);
	}
}

BEGIN_EVENT_MAP(MainFrameModule)
	ON_EVENT(EVENT_VALUE_MAINFRAME_OPEN, OnEvent_OpenMainDlg)
	ON_EVENT(EVENT_VALUE_MAINFRAME_CLOSE, OnEvent_CloseMainDlg)
	ON_EVENT(EVENT_VALUE_MAINFRAME_SHOW, OnEvent_ShowMainDlg)
	ON_EVENT(EVENT_VALUE_MAINFRAME_HIDE, OnEvent_HideMainDlg)
	ON_EVENT(EVENT_VALUE_DATACENTER_FAVORITE_ICON_ARRIVE, OnEvent_FavoriteIconArrive)
	ON_EVENT(EVENT_VALUE_MAINFRAME_ADD_URL, OnEvent_AddUrl)
	ON_EVENT(EVENT_VALUE_MAINFRAME_DELETE_FAVORITE, OnEvent_DeleteFavorite)
	ON_EVENT(EVENT_VALUE_MAINFRAME_DELETE_FAVORITE_FOLD, OnEvent_DeleteFavoriteFold)
	ON_EVENT(EVENT_VALUE_MAINFRAME_OPEN_URL, OnEvent_OpenUrl)
	ON_EVENT(EVENT_VALUE_MAINFRAME_COPY_URL, OnEvent_CopyUrl)
	ON_EVENT(EVENT_VALUE_DATACENTER_TREELIST_SELECT, OnEvent_SelectTreeListItem)
END_EVENT_MAP()


BEGIN_MESSAGE_MAP(MainFrameModule)
	ON_MESSAGE(MESSAGE_VALUE_CORE_BEGIN_SHOW, OnMessage_Show)
	ON_MESSAGE(MESSAGE_VALUE_CORE_CYCLE_TRIGGER, OnMessage_CycleTrigged)
	ON_MESSAGE(MESSAGE_VALUE_CORE_PRE_APP_EXIT, OnMessage_PreExit)
	ON_MESSAGE(MESSAGE_VALUE_PLUGIN_LOAD_FAVORITE_DATA_FINISHED, OnMessage_FavoriteLoaded)
	ON_MESSAGE(MESSAGE_VALUE_PLUGIN_LOAD_ALL_FINISHED, OnMessage_PlugInLoaded)
END_MESSAGE_MAP()

BEGIN_SERVICE_MAP(MainFrameModule)
	ON_SERVICE(SERVICE_VALUE_MAINFRAME_GET_MAINWND, OnService_GetMainWnd)
END_SERVICE_MAP();

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* MainFrameModule::GetModuleName() 
{
	return L"MainFrameModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const MainFrameModule::GetModuleId()
{
	return MODULE_ID_MAINFRAME;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void MainFrameModule::ProcessEvent(const Event& evt)
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
void MainFrameModule::ProcessMessage(const Message& msg) 
{
	PROCESS_MESSAGE(msg);
}

//----------------------------------------------------------------------------------------
//����: CallDirect
//����: ��Event��Message��Ҫͨ��������ת��ͬ��ĳ��ģ�����ֱ�ӵ�������һ��ģ���е�
//			����������Ҫͨ�����ߡ�
//����: 
//		@param	lServiceValue		����1
//		@param	rparam			����2
//----------------------------------------------------------------------------------------
int32 MainFrameModule::CallDirect(const ServiceValue lServiceValue, param rparam) 
{	
	CALL_DIRECT(lServiceValue, rparam);
}


void MainFrameModule::OnEvent_OpenMainDlg(Event* pEvent)
{
	MainFrame_OpenEvent* pE = (MainFrame_OpenEvent*)pEvent->m_pstExtraInfo;
	return;
}

void MainFrameModule::OnEvent_CloseMainDlg(Event* pEvent)
{

}

void MainFrameModule::OnEvent_ShowMainDlg(Event* pEvent)
{
	m_pMainFrame->ShowWindow(TRUE);
}

void MainFrameModule::OnEvent_HideMainDlg(Event* pEvent)
{
	m_pMainFrame->ShowWindow(FALSE);
}

void	MainFrameModule::OnEvent_FavoriteIconArrive(Event* pEvent)
{
	DataCenter_FavoriteIconArriveEvent*	pGetFavIconRespEvent = (DataCenter_FavoriteIconArriveEvent*)pEvent->m_pstExtraInfo;
	if( pGetFavIconRespEvent == NULL)
		return;

	wstring	wstrDomainUrl = pGetFavIconRespEvent->szDomain;
	HICON hIcon = pGetFavIconRespEvent->hIcon;
	m_pMainFrame->UpdateFavoriteIcon((wchar_t*)wstrDomainUrl.c_str(), hIcon);
}

void MainFrameModule::OnEvent_AddUrl(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_MAINFRAME_ADD_URL)
	{
		ASSERT(0);
		return;
	}

	m_pMainFrame->AddUrl();
}

void	MainFrameModule::OnEvent_DeleteFavorite(Event* pEvent)
{
	MainFrame_DeleteFavoriteEvent* pDeleteFavoriteEvent = (MainFrame_DeleteFavoriteEvent*)pEvent->m_pstExtraInfo;
	if( pDeleteFavoriteEvent == NULL)
		return;

	int nDeleteNodeId = pDeleteFavoriteEvent->nDeleteNodeId;
	m_pMainFrame->DeleteFavorite(nDeleteNodeId);
}

void	MainFrameModule::OnEvent_OpenUrl(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_MAINFRAME_OPEN_URL )
	{
		ASSERT(0);
		return;
	}

	int nSel = pEvent->param0;
	m_pMainFrame->OpenUrl(nSel);
}

void	MainFrameModule::OnEvent_CopyUrl(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_MAINFRAME_COPY_URL )
	{
		ASSERT(0);
		return;
	}

	int nSel = pEvent->param0;
	m_pMainFrame->CopyUrl(nSel);
}

void	MainFrameModule::OnEvent_DeleteFavoriteFold(Event* pEvent)
{
	MainFrame_DeleteFavoriteFoldEvent* pDeleteFavoriteFoldEvent = (MainFrame_DeleteFavoriteFoldEvent*)pEvent->m_pstExtraInfo;
	if( pDeleteFavoriteFoldEvent == NULL || pDeleteFavoriteFoldEvent->eventValue != EVENT_VALUE_MAINFRAME_DELETE_FAVORITE_FOLD)
	{
		ASSERT(0);
		return;
	}

	int nIndex  = pDeleteFavoriteFoldEvent->nDeleteIndex;
	m_pMainFrame->DeleteFavoriteFold(nIndex);
}

void	MainFrameModule::OnEvent_SelectTreeListItem(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_DATACENTER_TREELIST_SELECT)
		return;

	int nSelectIndex = pEvent->param0;
	m_pMainFrame->SelectTreeList(nSelectIndex);
}

void MainFrameModule::OnMessage_Show(Message* pMessage)
{
	CPaintManagerUI::SetInstance(GetModuleHandle(NULL));
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("\\skin\\UrlTraveler"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return;

	ASSERT(m_pMainFrame == NULL);

	if( m_pMainFrame != NULL)
	{
		m_pMainFrame->ShowWindow(true);
		return;
	}

	m_pMainFrame = new CMainFrameWnd();
	if( m_pMainFrame == NULL ) return;
	m_pMainFrame->Create(NULL, L"��ַ����", UI_WNDSTYLE_FRAME/*UI_WNDSTYLE_DIALOG*/, 0);
	m_pMainFrame->CenterWindow();
	m_pMainFrame->ShowWindow(true);

	//	֪ͨ�������е���������
	m_pModuleManager->PushEvent(
		MakeEvent<MODULE_ID_MAINFRAME>()(EVENT_VALUE_PLUGIN_LOAD_ALL,
		MODULE_ID_PLUGIN));
}

void MainFrameModule::OnMessage_PreExit(Message* pMessage)
{
}

void MainFrameModule::OnMessage_CycleTrigged(Message* pMessage)
{
}

void MainFrameModule::OnMessage_FavoriteLoaded(Message* pMessage)
{
	// �ڽ�������ʾ�����ղؼ���
	DataCenter_GetFavoriteService favoriteData;
	m_pModuleManager->CallService(SERVICE_VALUE_DATACENTER_GET_FAVORITE_DATA,(param)&favoriteData); 

	if( favoriteData.nNum > 0)
	{
		m_pMainFrame->LoadFavoriteTree(favoriteData.pFavoriteData, favoriteData.nNum);
	}
}


void MainFrameModule::OnMessage_PlugInLoaded( Message* pMessage )
{
	// ֪ͨMainFrame��ȡ	
	m_pMainFrame->GetAvailableBrowser();
}

int32 MainFrameModule::OnService_GetMainWnd(ServiceValue lServiceValue, param	lParam)
{
	MainFrame_GetWndService* pService = (MainFrame_GetWndService*)lParam;
	pService->pBaseWnd =	 static_cast<CWindowWnd*>(m_pMainFrame);

	return -1;
}