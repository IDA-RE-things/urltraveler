#pragma once

#include <vector>
#include <xstring>

// 常规配置
#define KEY_AUTO_LOGIN			L"KeyAutoLogin"
#define KEY_REMEMBER_PASSWD		L"KeyRememberPwd"
#define KEY_SHOW_EXITTIP			L"KeyShowExitTip"
#define KEY_MINTOTRAY				L"KeyMinToTray"

// 代理类型
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

// 更新
#define KEY_AUTOUPDATE			L"KeyAutoUpdate"

// 更新的类型
enum EUpdateType
{
	UPDATE_AUTO,			//	有更新的时候自动更新
	UPDATE_INSTALL_QUERY,	//	自动下载，安装的时候询问
	UPDATE_TIP,			//	不自动更新，有更新的时候进行提示
	UPDATE_CLOSE,		//	关闭自动更新
};

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
	
	BOOL		m_bSettingChange;			//设置是否发生变化

	//常规
	BOOL		m_bAutoLogin;				//是否自动进行登录。默认是自动登录
	BOOL		m_bRememberPwd;			//记住密码
	BOOL		m_bShowExitTip;			//退出时是否提示
	BOOL		m_bMinToTray;				//关闭时是否最小化，否则就是退出程序

	//代理
	unsigned int	m_iProxyType;				//代理类型
	std::wstring	m_strProxyServer[2];			//服务器
	std::wstring	m_strProxyPort[2];			//端口
	std::wstring	m_strProxyUser[2];			//用户名
	std::wstring	m_strProxyPwd[2];			//密码
	std::wstring	m_strProxyDomain[2];		//域

	//更新方式

};