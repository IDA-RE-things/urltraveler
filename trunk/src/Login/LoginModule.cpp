#include "stdafx.h"
#include "UIlib.h"
#include "LoginModule.h"
#include "MiscHelper.h"
#include "LoginDefine.h"
#include "WebDefine.h"
#include "ImageHelper.h"
#include "TxConfig.h"
#include "MiscHelper.h"
#include "StringHelper.h"
#include "MainFrameDefine.h"

using namespace login;
using namespace mainframe;

namespace login
{
	LoginModule*	g_LoginModule = NULL;
	LoginModuleFactory*	g_LoginModuleFactory = NULL;
}

// �������ʵ��
EXPORT_GETMODULEFACTORY(LoginModule)
EXPORT_RELEASEMODULEFACTORY(LoginModule)

LoginModule::LoginModule()
{

}

LoginModule::~LoginModule()
{

}

BEGIN_EVENT_MAP(LoginModule)
	ON_EVENT(EVENT_VALUE_LOGIN_OPEN, OnEvent_OpenLoginDlg)
END_EVENT_MAP()

BEGIN_SERVICE_MAP(LoginModule)
END_SERVICE_MAP()

BEGIN_MESSAGE_MAP(LoginModule)
END_MESSAGE_MAP()

//----------------------------------------------------------------------------------------
//����: GetModuleName
//����: ������ͨ���÷�����ȡ��ǰģ������֣�ÿһ��ģ�鶼��һ��Ψһ������
//����: 
//		���ж�سɹ�������TRUE�����򷵻�FALSE
//----------------------------------------------------------------------------------------
const wchar_t* LoginModule::GetModuleName() 
{
	return L"LoginModule";
}

//----------------------------------------------------------------------------------------
//����: GetModuleId
//����: ������ͨ���÷�����ȡ��ǰģ���ID��ÿһ��ģ�鶼�����һ��Ψһ��ID
//����: 
//		���ظ�ģ���Ψһ��ID
//----------------------------------------------------------------------------------------
uint32 const LoginModule::GetModuleId()
{
	return MODULE_ID_LOGIN;
}

//----------------------------------------------------------------------------------------
//����: ProcessEvent
//����: ��ģ����Դ�������еĵ�Ե��¼�
//����: 
//		@param	evt			��Ҫ������¼�
//----------------------------------------------------------------------------------------
void LoginModule::ProcessEvent(const Event& evt)
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
void LoginModule::ProcessMessage(const Message& msg) 
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
int32 LoginModule::CallDirect(const param lparam, param wparam) 
{
	CALL_DIRECT(lparam, wparam);
}

//----------------------------------------------------------------------------------------
//����: PaybackExtraInfo
//����: ĳ��ģ��������Զ���ĸ��ӵ�������Ҫ���������ģ�飬��ô�����Թ���һ��ExtraInfo�ṹ
//	�������ģ��ʹ����Ϻ󣬸ýṹ���뱻�ͷţ����������ڴ�й¶���ͷű�����ģ��������ɡ�
//	ĳ��ģ�鶼�����ṩһ��PaybackExtraInfo�ӿڣ��ͷ��Լ����Զ������������
//����: 
//		@param	valudId			��Ӧ��pExtraInfo��ֵ���ڲ����ݸ�ֵ���ж�Ӧ���ͷţ���ֵֻ��ģ���Լ����
//		@param	pExtraInfo	��Ҫ�ͷŵ�ExtraInfo����
//----------------------------------------------------------------------------------------
void LoginModule::PaybackExtraInfo(uint32 valudId, void* pExtraInfo)
{
	return;
}

void	LoginModule::OnEvent_OpenLoginDlg(Event* pEvent)
{
	if( pEvent == NULL || pEvent->eventValue != EVENT_VALUE_LOGIN_OPEN)
		return;

	// ǿ�Ƶ������´��ڽ�������
	mainframe::MainFrame_GetWndService stGetWndService;
	m_pModuleManager->CallService(mainframe::SERVICE_VALUE_MAINFRAME_GET_MAINWND, (param)&stGetWndService);

	CWindowWnd* pMainFrameWnd = reinterpret_cast<CWindowWnd*>(stGetWndService.pBaseWnd);
	ASSERT(pMainFrameWnd != NULL);
	pMainFrameWnd->ShowWindow(SW_NORMAL);

	CLoginPreWnd* pLoginFrame = new CLoginPreWnd();
	if( pLoginFrame == NULL )
		return ;

	pLoginFrame->Create(pMainFrameWnd->GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, UI_WNDSTYLE_EX_DIALOG, 0, 0, 0, 0, NULL);
	pLoginFrame->CenterWindow();
	pMainFrameWnd->ShowModal(*pLoginFrame);
}
