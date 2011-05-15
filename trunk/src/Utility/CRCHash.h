// ***************************************************************
//  NetSink   version:  1.0   ? date: 03/02/2009
//  -------------------------------------------------------------
//  author:shrewdlin
//  -------------------------------------------------------------
//  Copyright (C) 2009 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#if !defined(AFX_CRCHASH_H__A73D5BCC_D03F_45D8_BA9A_4CFC856FE417__INCLUDED_)
#define AFX_CRCHASH_H__A73D5BCC_D03F_45D8_BA9A_4CFC856FE417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "SndaBase.h"

class UTILITY_API CCRCHash
{
public:
	CCRCHash();
	~CCRCHash();
    virtual BOOL GetHash(BYTE *byInData, DWORD dwInLen, BYTE *pbyOutHash, DWORD dwOutLen);
private:
    VOID InitCRC32Table();
    ULONG Reflect(ULONG ulRef, CHAR ch);
    ULONG GetCRC(BYTE *byData, DWORD dwSize);
protected:
    ULONG m_ulTable[256];
};

#endif
