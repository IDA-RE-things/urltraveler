
// UrlTraveler.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "UrlTraveler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUrlTravelerApp

BEGIN_MESSAGE_MAP(CUrlTravelerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CUrlTravelerApp ����

CUrlTravelerApp::CUrlTravelerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CUrlTravelerApp ����

CUrlTravelerApp theApp;


// CUrlTravelerApp ��ʼ��

BOOL CUrlTravelerApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
