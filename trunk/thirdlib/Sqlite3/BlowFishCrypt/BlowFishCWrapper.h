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

#ifndef BLOWFISHCWRAPPER_H
#define BLOWFISHCWRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CRYPT_OFFSET
	#define CRYPT_OFFSET 8
#endif

#ifndef DWORD_BLOWFISH
	#define DWORD_BLOWFISH unsigned int
#endif

#ifndef WORD_BLOWFISH_BLOWFISH
	#define WORD_BLOWFISH_BLOWFISH unsigned short
#endif

#ifndef BYTE_BLOWFISH
	#define BYTE_BLOWFISH unsigned char
#endif

/** 创建加密对象
同时进行了加密对象的初始化设置好了加密密钥
@return void *, 
*/
extern void *BlowFishCWrapper_CreateCryptObj(
	BYTE_BLOWFISH *pKey, 
	DWORD_BLOWFISH dwKeyLen
	);

/** 销毁加密对象
@return void , 
*/
extern void BlowFishCWrapper_DestroyCryptObj(
	void *ptrCryptObj
	);

/** 获取加密或解密需要的最小缓冲区长度
@return DWORD_BLOWFISH, 
*/
extern DWORD_BLOWFISH BlowFishCWrapper_GetNecessaryBufLen(
				DWORD_BLOWFISH dwDataSize
				);

/** 加密
采用给定的加密对象进行加密
@return DWORD_BLOWFISH, 
*/
extern void BlowFishCWrapper_Decode (
			 void *ptrCryptObj,
			 BYTE_BLOWFISH * pInput, 
			 BYTE_BLOWFISH * pOutput, 
			 DWORD_BLOWFISH lSize
			 );

/** 解密
采用给定的加密对象进行解密
@return DWORD_BLOWFISH, 
*/
extern DWORD_BLOWFISH BlowFishCWrapper_Encode (
			 void *ptrCryptObj,
			 BYTE_BLOWFISH * pInput, 
			 BYTE_BLOWFISH * pOutput, 
			 DWORD_BLOWFISH lSize
			 );

 /** 复制动态创建一个新加密对象
 采用给定的加密对象进行来Create一个新的加密对象
 @return  void *, 
 */
extern void *BlowFishCWrapper_DuplicateCryptObj(
	 void *ptrCryptObj
	 );

#ifdef __cplusplus
}
#endif

#endif ///BLOWFISHCWRAPPER_H