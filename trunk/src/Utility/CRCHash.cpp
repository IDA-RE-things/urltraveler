#include "StdAfx.h"
#include "CRCHash.h"

CCRCHash::CCRCHash()
{
    memset(m_ulTable, 0, 256);
    InitCRC32Table();
}

CCRCHash::~CCRCHash()
{

}

void CCRCHash::InitCRC32Table()
{
    ULONG ulPolynomial = 0x04C11DB7;
    
    for(int i = 0; i <= 0xFF; i++)
    {
        m_ulTable[i] = Reflect(i, 8) << 24;

        for (int j = 0; j < 8; j++)
        {
            m_ulTable[i] = (m_ulTable[i] << 1) ^ (m_ulTable[i] & (1 << 31) ? ulPolynomial : 0);
        }

        m_ulTable[i] = Reflect(m_ulTable[i], 32);
    }
}

ULONG CCRCHash::Reflect(ULONG ulRef, CHAR ch)
{

    ULONG ulValue(0);
    
    for(int i = 1; i < (ch + 1); i++)
    {
        if(ulRef & 1)
        {
            ulValue |= 1 << (ch - i);
        }

        ulRef >>= 1;
    }

    return ulValue;
}

ULONG CCRCHash::GetCRC(BYTE *byData, DWORD dwSize)
{
    ULONG  ulCRC(0xFFFFFFFF);
    int nLen;
    
    nLen = dwSize;

    while(nLen--)
    {
        ulCRC = (ulCRC >> 8) ^ m_ulTable[(ulCRC & 0xFF) ^ *byData++];
    }

    return ulCRC ^ 0xFFFFFFFF;
}

BOOL CCRCHash::GetHash(BYTE *byInData, DWORD dwInLen, BYTE *pbyOutHash, DWORD dwOutLen)
{
    BOOL bRet = FALSE;
    BOOL bParamOK = byInData != NULL && dwInLen > 0 && pbyOutHash != NULL && dwOutLen >= 4;

    if (bParamOK)
    {
        *((ULONG *)pbyOutHash) = GetCRC(byInData, dwInLen);
        bRet = TRUE;
    }

    return bRet;
}
