#include "StdAfx.h"
#include "TxConfig.h"

CTxConfig::CTxConfig(void)
{
}

CTxConfig::~CTxConfig(void)
{
}

BOOL CTxConfig::ParseConfig( BYTE *pConfigBuf, DWORD dwLen )
{
	TxConfig *pConfig = (TxConfig *)pConfigBuf;
	TxItem   *pItem = NULL;

	pItem = &pConfig->stItem[0];

	for (int i = 0; i < pConfig->wNumOfItems; i++)
	{
		WORD wKeyLen = pItem->wKeyLen;

		for (int j = 0; j < wKeyLen; j++)
		{
			pItem->abyKey[j] = LOBYTE(wKeyLen) ^ HIBYTE(wKeyLen) ^ ~pItem->abyKey[j];
		}

		TxItem *pTemp = (TxItem *)((BYTE *)pItem + wKeyLen - 1);

		DWORD dwValueLen = pTemp->dwValueLen;

		for (int j = 0; j < dwValueLen; j++)
		{
			pTemp->abyValue[j] = LOBYTE(dwValueLen) ^ HIBYTE(dwValueLen) ^ ~pTemp->abyValue[j];
		}

		wchar_t szKey[MAX_PATH] = {0};

		wcscpy_s(szKey, MAX_PATH - 1, (wchar_t *)pItem->abyKey);
		szKey[wKeyLen / 2] = 0;

		std::string strValue;

		strValue.assign((char *)&pTemp->abyValue[0], dwValueLen);

		m_mapKey2Value[szKey] = strValue;

		


		pItem = (TxItem *)((BYTE *)pItem + sizeof(TxItem) + wKeyLen + dwValueLen - 2);
	}

	return TRUE;
}

BOOL CTxConfig::ParseConfig( std::string strFilePath )
{
	FILE *configFile = fopen(strFilePath.c_str(), "rb");

	long lOldPos = ftell(configFile);

	fseek(configFile, 0, SEEK_END);

	size_t fileSize = ftell(configFile);

	fseek(configFile, lOldPos, SEEK_SET);

	BYTE *pConfigBuffer = new BYTE[fileSize];

	fread(pConfigBuffer, 1, fileSize, configFile);

	BOOL bRet = ParseConfig(pConfigBuffer, fileSize);

	if (pConfigBuffer)
	{
		delete []pConfigBuffer;
	}

	return bRet;
}

std::string CTxConfig::GetValue( std::wstring strKey )
{
	std::map<std::wstring, std::string>::iterator iter;
	iter = m_mapKey2Value.find(strKey);

	if (iter != m_mapKey2Value.end())
	{
		return iter->second;
	}

	return "";
}

void CTxConfig::SetValue( std::wstring strKey, std::string strValue )
{
	m_mapKey2Value[strKey] = strValue;
}

BOOL CTxConfig::MakeConfig( std::string strFilePath )
{
	if (m_mapKey2Value.size() == 0)
	{
		return FALSE;
	}

	FILE *configFile = fopen(strFilePath.c_str(), "rb");

	return true;
}
