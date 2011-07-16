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

	if (pConfig->wMagic != TX_CONFIG_MAGIC)
	{
		return FALSE;
	}

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

		for (size_t j = 0; j < dwValueLen; j++)
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

	std::map<std::wstring, std::string>::iterator iterPointer = m_mapKey2Value.begin();
	std::map<std::wstring, std::string>::const_iterator iterEnd = m_mapKey2Value.end();

	size_t configSize = sizeof(TxConfig);

	for (; iterPointer != iterEnd; ++iterPointer)
	{
		std::wstring strKey = iterPointer->first;
		std::string  strValue = iterPointer->second;

		configSize = configSize + sizeof(TxItem) - 2 + strKey.length() * 2 + strValue.length();
	}

	BYTE *configBuffer = new BYTE[configSize];

	TxConfig *pConfig = (TxConfig *)configBuffer;

	pConfig->byMajorVer = 1;
	pConfig->byMinorVer = 1;
	pConfig->wNumOfItems = m_mapKey2Value.size();
	pConfig->wMagic = TX_CONFIG_MAGIC;

	TxItem   *pItem = NULL;
	pItem = &pConfig->stItem[0];

	iterPointer = m_mapKey2Value.begin();

	for (int i = 0; iterPointer != iterEnd; ++iterPointer, i++)
	{
		std::wstring strKey = iterPointer->first;
		std::string  strValue = iterPointer->second;
		pItem->byFlag = TX_ITEM_FLAG;

		pItem->wKeyLen = strKey.length() * 2;

		memcpy(&pItem->abyKey[0], strKey.c_str(), strKey.length() * 2);

		for (int j = 0; j < pItem->wKeyLen; j++)
		{
			pItem->abyKey[j] = LOBYTE(pItem->wKeyLen) ^ HIBYTE(pItem->wKeyLen) ^ ~pItem->abyKey[j];
		}

		TxItem *pTemp = (TxItem *)((BYTE *)pItem + pItem->wKeyLen - 1);

		pTemp->dwValueLen = strValue.length();
		memcpy(&pTemp->abyValue[0], strValue.c_str(), strValue.length());

		for (size_t j = 0; j < pTemp->dwValueLen; j++)
		{
			pTemp->abyValue[j] = LOBYTE(pTemp->dwValueLen) ^ HIBYTE(pTemp->dwValueLen) ^ ~pTemp->abyValue[j];
		}


		pItem = (TxItem *)((BYTE *)pItem + sizeof(TxItem) + pItem->wKeyLen + pTemp->dwValueLen - 2);
	}


	FILE *configFile = fopen(strFilePath.c_str(), "wb");

	fwrite(configBuffer, 1, configSize, configFile);

	fclose(configFile);

	if (configBuffer)
	{
		delete []configBuffer;
	}

	return true;
}
