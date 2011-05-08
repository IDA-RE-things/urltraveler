/**********************************************************
* ��Ȩ���� (C)2008, 51.com
*
* �ļ����ƣ�BlowFishCWrapper.h
* ����ժҪ����ɶ�BlowFish�����㷨��C���Է�װ
*           ��ʹsqlite��ʹ�øü��ܹ��ܸ�����
*           
* ����˵����
* ��ǰ�汾��
* ��    �ߣ��»���
* ������ڣ�2008-8-27
*
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ�
**********************************************************/

#include <stdlib.h>
#include <wtypes.h>
#include "blowfish.h"
#include "BlowFishCWrapper.h"

DWORD_BLOWFISH BlowFishCWrapper_GetNecessaryBufLen(
	DWORD_BLOWFISH dwDataSize
	)
{
	DWORD_BLOWFISH dwNecessaryBufLen = 0;

#if 1
	CBlowFish___ blowFish;
	dwNecessaryBufLen = blowFish.GetOutputLength(dwDataSize);
#else
	int iDifference = (dwDataSize % CRYPT_OFFSET);
	dwNecessaryBufLen = dwDataSize;
	if( iDifference	!= 0 )
	{
		dwNecessaryBufLen = (dwDataSize + (CRYPT_OFFSET - iDifference));
	}
#endif

	return dwNecessaryBufLen;
}

void *BlowFishCWrapper_CreateCryptObj(
	 BYTE_BLOWFISH *pKey, 
	 DWORD_BLOWFISH dwKeyLen
	 )
{
	CBlowFish___ *ptrCryptObj = new CBlowFish___();
	
	if (NULL != ptrCryptObj)
	{
		ptrCryptObj->Initialize((BYTE *) pKey, dwKeyLen);
	}

	return ptrCryptObj;
}

void BlowFishCWrapper_DestroyCryptObj(
	  void *ptrCryptObj
	  )
{
	if (NULL != ptrCryptObj)
	{
		delete (CBlowFish___ *)ptrCryptObj;
	}
}

DWORD_BLOWFISH BlowFishCWrapper_Encode (
	void *ptrCryptObj,
	BYTE_BLOWFISH * pInput, 
	BYTE_BLOWFISH * pOutput, 
	DWORD_BLOWFISH lSize
	)
{
	DWORD_BLOWFISH dwEncodeSize = 0;

	if (NULL != ptrCryptObj)
	{
		dwEncodeSize = ((CBlowFish___ *)ptrCryptObj)->Encode(pInput, pOutput, lSize);
	}

	return dwEncodeSize;
}

void BlowFishCWrapper_Decode (
	void *ptrCryptObj,
	BYTE_BLOWFISH * pInput, 
	BYTE_BLOWFISH * pOutput, 
	DWORD_BLOWFISH lSize
	)
{
	if (NULL != ptrCryptObj)
	{
		((CBlowFish___ *)ptrCryptObj)->Decode(pInput, pOutput, lSize);
	}
}

void *BlowFishCWrapper_DuplicateCryptObj(
	void *ptrCryptObj
	)
{
	CBlowFish___ *ptrCryptObjReturn = NULL;
	
	ptrCryptObjReturn = new CBlowFish___();
	///����ptrCryptObj�е����ݹ���
	*ptrCryptObjReturn = *((CBlowFish___ *)ptrCryptObj);

	return ptrCryptObjReturn;
}

#if 0
void BlowFish::Encode(byte* key,ByteBuf & buf)
{
	CBlowFish blow;
	blow.Initialize((byte*)key,16);
	int iLength = buf.first;
	int iDifference = iLength % 8;
	int iNewLength = iLength;
	if( iDifference	!= 0 )
		iNewLength = (iLength - iDifference) + 8;
	buf.first=blow.Encode(buf.second,buf.second,iNewLength);
}

void BlowFishCWrapper_Decode(BYTE_BLOWFISH * pInput, BYTE_BLOWFISH * pOutput, DWORD_BLOWFISH lSize)
{
	CBlowFish blow;
	/*blow.Initialize((byte*)&key,sizeof(key));*/
	blow.Initialize((byte*)key,16);
	int iLength = buf.first;
	int iDifference = iLength % 8;
	int iNewLength = iLength;
	if( iDifference	!= 0 )
		iNewLength = (iLength - iDifference) + 8;
	/*buf.first=*/blow.Decode(buf.second,buf.second,iNewLength);
}
#endif