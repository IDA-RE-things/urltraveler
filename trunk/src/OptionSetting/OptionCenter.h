#pragma once

#include <vector>
#include <xstring>
#include "SettingDefine.h"

using namespace setting;

// ��������
#define KEY_AUTO_START			L"KeyAutoStart"
#define KEY_AUTO_LOGIN			L"KeyAutoLogin"
#define KEY_REMEMBER_PASSWD		L"KeyRememberPwd"
#define KEY_SHOW_EXITTIP			L"KeyShowExitTip"
#define KEY_MINTOTRAY				L"KeyMinToTray"
#define KEY_LOCAL_SYNC				L"KeyLocalSync"
#define KEY_REMOTE_SYNC				L"KeyRemoteSync"

// ��������
#define KEY_PROXY_TYPE				L"KeyProxyType"
#define KEY_HTTPPROXY_SERVER		L"KeyHttpProxyServer"
#define KEY_HTTPPROXY_PORT		L"KeyHttpProxyPort"
#define KEY_HTTPPROXY_USER		L"KeyHttpProxyUser"
#define KEY_HTTPPROXY_PWD			L"KeyHttpProxyPwd"
#define KEY_HTTPPROXY_DOMAIN		L"KeyHttpProxyDomain"
#define KEY_SOCKETPROXY_SERVER	L"KeySocketProxyServer"
#define KEY_SOCKETPROXY_PORT		L"KeySocketProxyPort"
#define KEY_SOCKETPROXY_USER		L"KeySocketProxyUser"
#define KEY_SOCKETPROXY_PWD		L"KeySocketProxyPwd"
#define KEY_SOCKETPROXY_DOMAIN	L"KeySocketProxyDomain"

// ����
#define KEY_AUTOUPDATE			L"KeyAutoUpdate"

class OptionCenter
{
public:

	OptionCenter();
	~OptionCenter();

	void	LoadSetting();
	void	SaveSetting();

	void	LoadCommonSetting();
	void	SaveCommonSetting();

	void	LoadProxySetting();
	void	SaveProxySetting();

	void	LoadUpdateSetting();
	void	SaveUpdateSetting();

public:
	
	BOOL		m_bNormalSettingChange;	//���������Ƿ����仯
	BOOL		m_bProxySettingChange;		//���������Ƿ����仯
	BOOL		m_bUpdateSettingChange;	//���������Ƿ����仯

	//����
	BOOL		m_bAutoStart;						//	�Ƿ����window�Զ�����
	BOOL		m_bAutoLogin;						//�Ƿ��Զ����е�¼��Ĭ�����Զ���¼
	BOOL		m_bRememberPwd;				//��ס����
	BOOL		m_bShowExitTip;					//�˳�����ʱ�Ƿ���ʾ
	BOOL		m_bMinToTray;						//�ر�ʱ�Ƿ���С������������˳�����
	BOOL		m_bAutoLocalSync;				//��ʱ����ͬ��
	BOOL		m_bAutoRemotingSync;		//��ʱԶ������

	//����
	unsigned int	m_iProxyType;					//��������
	std::wstring	m_strProxyServer[2];			//������
	std::wstring	m_strProxyPort[2];			//�˿�
	std::wstring	m_strProxyUser[2];			//�û���
	std::wstring	m_strProxyPwd[2];			//����
	std::wstring	m_strProxyDomain[2];		//��

	//���·�ʽ
	EUpdateType	m_eUpdateType;			//�Զ����·�ʽ
};