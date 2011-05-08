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

/** �������ܶ���
ͬʱ�����˼��ܶ���ĳ�ʼ�����ú��˼�����Կ
@return void *, 
*/
extern void *BlowFishCWrapper_CreateCryptObj(
	BYTE_BLOWFISH *pKey, 
	DWORD_BLOWFISH dwKeyLen
	);

/** ���ټ��ܶ���
@return void , 
*/
extern void BlowFishCWrapper_DestroyCryptObj(
	void *ptrCryptObj
	);

/** ��ȡ���ܻ������Ҫ����С����������
@return DWORD_BLOWFISH, 
*/
extern DWORD_BLOWFISH BlowFishCWrapper_GetNecessaryBufLen(
				DWORD_BLOWFISH dwDataSize
				);

/** ����
���ø����ļ��ܶ�����м���
@return DWORD_BLOWFISH, 
*/
extern void BlowFishCWrapper_Decode (
			 void *ptrCryptObj,
			 BYTE_BLOWFISH * pInput, 
			 BYTE_BLOWFISH * pOutput, 
			 DWORD_BLOWFISH lSize
			 );

/** ����
���ø����ļ��ܶ�����н���
@return DWORD_BLOWFISH, 
*/
extern DWORD_BLOWFISH BlowFishCWrapper_Encode (
			 void *ptrCryptObj,
			 BYTE_BLOWFISH * pInput, 
			 BYTE_BLOWFISH * pOutput, 
			 DWORD_BLOWFISH lSize
			 );

 /** ���ƶ�̬����һ���¼��ܶ���
 ���ø����ļ��ܶ��������Createһ���µļ��ܶ���
 @return  void *, 
 */
extern void *BlowFishCWrapper_DuplicateCryptObj(
	 void *ptrCryptObj
	 );

#ifdef __cplusplus
}
#endif

#endif ///BLOWFISHCWRAPPER_H