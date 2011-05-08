/**********************************************************
* 版权所有 (C)2008, 51.com
*
* 文件名称：BlowFishCWrapper.h
* 内容摘要：完成对BlowFish加密算法的C语言封装
*           以使sqlite中使用该加密功能更容易
*           
* 其它说明：
* 当前版本：
* 作    者：温辉敏
* 完成日期：2008-8-27
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
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
	///拷贝ptrCryptObj中的数据过来
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