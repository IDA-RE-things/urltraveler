#pragma once
#include <Windows.h>
#include <string>
#include <map>

#define TX_CONFIG_MAGIC 0x4454
#define TX_ITEM_FLAG    0x9

#pragma pack(1)
typedef struct TxItem
{
	BYTE  byFlag;
	WORD  wKeyLen;
	BYTE  abyKey[1];
	DWORD dwValueLen;
	BYTE  abyValue[1];
}TXITEM;

typedef struct TxConfig
{
	WORD wMagic;
	BYTE byMajorVer;
	BYTE byMinorVer;
	WORD wNumOfItems;
	TxItem stItem[0];
};
#pragma pop()

class UTILITY_API CTxConfig
{
public:
	CTxConfig(void);
	~CTxConfig(void);
	BOOL ParseConfig(std::string strFilePath);
	BOOL ParseConfig(BYTE *pConfigBuf, DWORD dwLen);
	std::string GetValue(std::wstring strKey);
	void SetValue(std::wstring strKey, std::string strValue);
	BOOL MakeConfig(std::string strFilePath);
private:
	std::map<std::wstring, std::string> m_mapKey2Value;
};
