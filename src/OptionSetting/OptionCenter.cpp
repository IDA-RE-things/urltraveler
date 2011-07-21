#include "stdafx.h"
#include "OptionCenter.h"
#include "MiscHelper.h"
#include "TxConfig.h"
#include "StringHelper.h"

OptionCenter::OptionCenter()
{
	m_bNormalSettingChange = FALSE;
	m_bProxySettingChange	= FALSE;
	m_bUpdateSettingChange = FALSE;

	m_bAutoStart = TRUE;
	m_bRememberPwd = FALSE;
	m_bAutoLogin = FALSE;
	m_bMinToTray = TRUE;
	m_bShowExitTip = TRUE;

	m_bAutoLocalSync = TRUE;
	m_bAutoRemotingSync = TRUE;

	m_eUpdateType	=	UPDATE_TIP;
}

OptionCenter::~OptionCenter()
{
}

void	OptionCenter::LoadSetting()
{
	LoadCommonSetting();
	LoadProxySetting();
	LoadUpdateSetting();
}

void	OptionCenter::SaveSetting()
{
	SaveCommonSetting();
	SaveProxySetting();
	SaveUpdateSetting();
}

void	OptionCenter::LoadCommonSetting()
{
	wchar_t* pConfig = MiscHelper::GetConfig();
	if( pConfig == NULL)
		return;

	CTxConfig txConfig;
	BOOL bRet = txConfig.ParseConfig(StringHelper::UnicodeToANSI(pConfig));
	if( bRet == FALSE)
		return ;

	string strValue = txConfig.GetValue(KEY_AUTO_START);
	if( StringHelper::TrimAll(strValue) != "")
	{
		if(StringHelper::TrimAll(strValue) == "0")
			m_bAutoStart = FALSE;
		else
			m_bAutoStart = TRUE;
	}

	strValue = txConfig.GetValue(KEY_AUTO_LOGIN);
	if( StringHelper::TrimAll(strValue) != "")
	{
		if(StringHelper::TrimAll(strValue) == "0")
			m_bAutoLogin = FALSE;
		else
			m_bAutoLogin = TRUE;
	}

	strValue = txConfig.GetValue(KEY_REMEMBER_PASSWD);
	if( StringHelper::TrimAll(strValue) != "")
	{
		if(StringHelper::TrimAll(strValue) == "0")
			m_bRememberPwd = FALSE;
		else
			m_bRememberPwd = TRUE;
	}

	strValue = txConfig.GetValue(KEY_SHOW_EXITTIP);
	if( StringHelper::TrimAll(strValue) != "")
	{
		if(StringHelper::TrimAll(strValue) == "0")
			m_bShowExitTip = FALSE;
		else
			m_bShowExitTip = TRUE;
	}

	strValue = txConfig.GetValue(KEY_MINTOTRAY);
	if( StringHelper::TrimAll(strValue) != "" )
	{
		if(StringHelper::TrimAll(strValue) == "0")
			m_bMinToTray = FALSE;
		else
			m_bMinToTray = TRUE;
	}

	strValue = txConfig.GetValue(KEY_LOCAL_SYNC);
	if( StringHelper::TrimAll(strValue) != "")
	{
		if(StringHelper::TrimAll(strValue) == "0")
			m_bAutoLocalSync = FALSE;
		else
			m_bAutoLocalSync = TRUE;
	}

	strValue = txConfig.GetValue(KEY_REMOTE_SYNC);
	if( StringHelper::TrimAll(strValue) != "" )
	{
		if(StringHelper::TrimAll(strValue) == "0")
			m_bAutoRemotingSync = FALSE;
		else
			m_bAutoRemotingSync = TRUE;
	}
}

void	OptionCenter::SaveCommonSetting()
{
	wchar_t* pConfig = MiscHelper::GetConfig();
	if( pConfig == NULL)
		return;

	CTxConfig txConfig;
	BOOL bRet = txConfig.ParseConfig(StringHelper::UnicodeToANSI(pConfig));
	if( bRet == FALSE)
		return ;

	txConfig.SetValue(KEY_AUTO_START, m_bAutoStart == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_AUTO_LOGIN, m_bAutoLogin == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_REMEMBER_PASSWD, m_bRememberPwd == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_SHOW_EXITTIP, m_bShowExitTip == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_MINTOTRAY, m_bMinToTray == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_LOCAL_SYNC, m_bAutoLocalSync == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_REMOTE_SYNC, m_bAutoRemotingSync == TRUE ? "1" : "0" );

	txConfig.MakeConfig(StringHelper::UnicodeToANSI(pConfig));
}

void	OptionCenter::LoadProxySetting()
{

}

void	OptionCenter::SaveProxySetting()
{

}

void	OptionCenter::LoadUpdateSetting()
{
	wchar_t* pConfig = MiscHelper::GetConfig();
	if( pConfig == NULL)
		return;

	CTxConfig txConfig;
	BOOL bRet = txConfig.ParseConfig(StringHelper::UnicodeToANSI(pConfig));
	if( bRet == FALSE)
		return ;

	string strValue = txConfig.GetValue(KEY_AUTOUPDATE);
	if( StringHelper::TrimAll(strValue) != "")
	{
		int nType =	StringHelper::ConvertToInt(StringHelper::TrimAll(strValue));
		if(nType == (int)UPDATE_AUTO)
			m_eUpdateType = UPDATE_AUTO;
		else  if( nType == (int)UPDATE_INSTALL_QUERY)
			m_eUpdateType = UPDATE_INSTALL_QUERY;
		else  if( nType == (int)UPDATE_TIP)
			m_eUpdateType = UPDATE_TIP;
		else  if( nType == (int)UPDATE_CLOSE)
			m_eUpdateType = UPDATE_CLOSE;
		else
			m_eUpdateType = UPDATE_TIP;
	}
}

void	OptionCenter::SaveUpdateSetting()
{
	wchar_t* pConfig = MiscHelper::GetConfig();
	if( pConfig == NULL)
		return;

	CTxConfig txConfig;
	BOOL bRet = txConfig.ParseConfig(StringHelper::UnicodeToANSI(pConfig));
	if( bRet == FALSE)
		return ;

	txConfig.SetValue(KEY_AUTOUPDATE, StringHelper::ConvertFromInt((int)m_eUpdateType));
	txConfig.MakeConfig(StringHelper::UnicodeToANSI(pConfig));
}
