#include "stdafx.h"
#include "OptionCenter.h"
#include "MiscHelper.h"
#include "TxConfig.h"
#include "StringHelper.h"

OptionCenter::OptionCenter()
{
	m_bRememberPwd = FALSE;
	m_bAutoLogin = FALSE;
	m_bMinToTray = TRUE;
	m_bShowExitTip = TRUE;
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

	m_bAutoLogin = (txConfig.GetValue(KEY_AUTO_LOGIN) == "0" ? FALSE : TRUE);
	m_bRememberPwd = (txConfig.GetValue(KEY_REMEMBER_PASSWD) == "0" ? FALSE : TRUE);
	m_bShowExitTip = (txConfig.GetValue(KEY_SHOW_EXITTIP) == "0" ? FALSE : TRUE);
	m_bMinToTray = (txConfig.GetValue(KEY_MINTOTRAY) == "0" ? FALSE : TRUE);
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

	txConfig.SetValue(KEY_AUTO_LOGIN, m_bAutoLogin == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_REMEMBER_PASSWD, m_bRememberPwd == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_SHOW_EXITTIP, m_bShowExitTip == TRUE ? "1" : "0" );
	txConfig.SetValue(KEY_MINTOTRAY, m_bMinToTray == TRUE ? "1" : "0" );

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

}

void	OptionCenter::SaveUpdateSetting()
{

}
