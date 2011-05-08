/********************************************************************
	created:	2008/02/18
	created:	18:2:2008   9:52
	
*********************************************************************/
#ifdef WIN32
    #include "windows.h"
#include <boost/cstdint.hpp>
/*using namespace boost;*/
#else
    #include <stdlib.h>
    #include <string.h>
#endif

#include "Crypt.h"
#include "Portable.h"
using namespace Auth51;

void xtea(boost::int32_t* v, // 64bit of data [in/out]
		  boost::int32_t* o, // 64bits of data [out]
		  boost::int32_t* k, // 128bit key [in]
		  boost::int32_t N) // Routine rounds [in]
{
	boost::uint32_t y = v[0];
	boost::uint32_t z = v[1];
	boost::uint32_t DELTA = 0x9e3779b9; // 0x00000000 - 0x61C88647 == 0x9e3779b9

	if(N>0)
	{
		// Encoding
		boost::uint32_t limit = DELTA*N;
		boost::uint32_t sum = 0;
		while(sum != limit)
		{
			y += (z<<4 ^ z>>5) + z ^ sum + k[sum&3];
			sum += DELTA;
			z += (y<<4 ^ y>>5) + y ^ sum + k[sum>>11&3];
		}
	}
	else
	{
		// Decoding
		boost::uint32_t sum = DELTA*(-N);
		while(sum)
		{
			z -= (y<<4 ^ y>>5) + y ^ sum + k[sum>>11&3];
			sum -= DELTA;
			y -= (z<<4 ^ z>>5) + z ^ sum + k[sum&3];
		}
	}

	o[0] = y;
	o[1] = z;
}

/*****************************************************************************
//	���ܺ��Buffer�ṹ
//  ���������������Щ����������������������Щ��������Щ���������
//  �� PadLength  ��  Padded Random BYTEs ��  Data  �� Zero s ��
//  ���������������੤���������������������੤�������੤��������
//  ��    1Byte   ��    PadLength Bytes   �� N Bytes�� 7Bytes ��
//  ���������������ة����������������������ة��������ة���������
// Pad�������ڽ�����Buffer���뵽8�ֽڶ���
******************************************************************************/

#define ZERO_LENGTH 7
#define ENCRYPT_ROUNDS (32)
#define DECRYPT_ROUNDS (-32)
#define ENCRYPT_BLOCK_LENGTH_IN_BYTE (8)

int CXTEA::Encrypt( char* pbyInBuffer, int nInBufferLength, char* pbyOutBuffer, int nOutBufferLength, char arrbyKey[16] )
{
	if(pbyInBuffer == NULL || nInBufferLength <= 0)
	{
		return 0;
	}

	// ������Ҫ�����Buffer��С
	int nPadDataZero = 1/*Pad Length*/ + nInBufferLength + ZERO_LENGTH;
	int nPadLength = nPadDataZero % ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	if(nPadLength != 0)
	{
		nPadLength = ENCRYPT_BLOCK_LENGTH_IN_BYTE - nPadLength;
	}

	int nTotalLength = nPadDataZero + nPadLength;

	if(nTotalLength > nOutBufferLength || pbyOutBuffer == NULL)
	{
		return 0;
	}

	char* pbyInCursor = pbyInBuffer;
	char* pbyOutCurosr = pbyOutBuffer;

	memset(pbyOutBuffer, 0, nOutBufferLength);

	char arrbyFirst8Bytes[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	// Pad length, ֻʹ�������λ����5λ����������
	arrbyFirst8Bytes[0] = (((char)rand()) & 0xf8) | ((char)nPadLength);

	// ����������Pad��
	for(int i =1; i<=nPadLength; ++i)
	{
		arrbyFirst8Bytes[i] = (char)rand();
	}

	// �ô��������ݲ�����һ������
	for(int i = 1+nPadLength; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		arrbyFirst8Bytes[i] = *pbyInCursor++;
	}

	// ��һ�����ܿ�����������ģ����ں����������
	char* pbyLast8BytesCryptedData = pbyOutBuffer;
	char* pbyLast8BytesPlainData = arrbyFirst8Bytes;

	// ��һ��Buffer������Ҫ������
	xtea((boost::int32_t*)arrbyFirst8Bytes, (boost::int32_t*)pbyOutCurosr, (boost::int32_t*)arrbyKey, ENCRYPT_ROUNDS);
//	xtea((Int32*)pbyOutCurosr, (Int32*)arrbyFirst8Bytes, (Int32*)arrbyKey, DECRYPT_ROUNDS);
	pbyOutCurosr += ENCRYPT_BLOCK_LENGTH_IN_BYTE;

	// ��������������ڲ�����InBuffer�ļ��ܹ���
	char arrbySrcBuffer[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	while((pbyInCursor - pbyInBuffer) < (nInBufferLength - 1))
	{
		memcpy(arrbySrcBuffer, pbyInCursor, ENCRYPT_BLOCK_LENGTH_IN_BYTE);
		// ����һ���������
		for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			arrbySrcBuffer[i] ^= pbyLast8BytesCryptedData[i];
		}
		xtea((boost::int32_t*)arrbySrcBuffer, (boost::int32_t*)pbyOutCurosr, (boost::int32_t*)arrbyKey, ENCRYPT_ROUNDS);
		// ����һ���������
		for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyOutCurosr[i] ^= pbyLast8BytesPlainData[i];
		}

		pbyLast8BytesCryptedData = pbyOutCurosr;
		pbyLast8BytesPlainData = pbyInCursor;

		pbyOutCurosr += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
		pbyInCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	}

	// ��һ�κ���һ�α�ע�͵��Ĺ�����ͬ��ֻ�������޸�InBuffer������
/*	while((pbyInCursor - pbyInBuffer) < (nInBufferLength - 1))
	{
		// ����һ���������
		for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyInCursor[i] ^= pbyLast8BytesCryptedData[i];
		}
		xtea((Int32*)pbyInCursor, (Int32*)pbyOutCurosr, (Int32*)arrbyKey, ENCRYPT_ROUNDS);
		// ����һ���������
		for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyOutCurosr[i] ^= pbyLast8BytesPlainData[i];
		}

		pbyLast8BytesCryptedData = pbyOutCurosr;
		pbyLast8BytesPlainData = pbyInCursor;

		pbyOutCurosr += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
		pbyInCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	}
*/
	// ��β�� 1Byte���� + 7Byte У��
	char arrbyLast8Bytes[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	arrbyLast8Bytes[0] = *pbyInCursor;

	// ����һ���������
	for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		arrbyLast8Bytes[i] ^= pbyLast8BytesCryptedData[i];
	}
	xtea((boost::int32_t*)arrbyLast8Bytes, (boost::int32_t*)pbyOutCurosr, (boost::int32_t*)arrbyKey, ENCRYPT_ROUNDS);
	// ����һ���������
	for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		pbyOutCurosr[i] ^= pbyLast8BytesPlainData[i];
	}

	return nTotalLength;
}
	// TODO: OutBufferLength�ж�
int CXTEA::Decrypt( char* pbyInBuffer, int nInBufferLength, char* pbyOutBuffer, int nOutBufferLength, char arrbyKey[16] )
{
	if(pbyInBuffer == NULL || nInBufferLength <= 0)
	{
		return false;
	}

	// Buffer����Ӧ�����ܱ� ENCRYPT_BLOCK_LENGTH_IN_BYTE ������
	if(nInBufferLength%ENCRYPT_BLOCK_LENGTH_IN_BYTE || nInBufferLength <= 8)
	{
		return 0;
	}

	char* pbyInCursor = pbyInBuffer;
	char* pbyOutCursor = pbyOutBuffer;

	// �Ƚ����ǰ�����Pad��ENCRYPT_BLOCK_LENGTH_IN_BYTE��Byte
	char arrbyFirst8Bytes[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	xtea((boost::int32_t*)pbyInCursor, (boost::int32_t*)arrbyFirst8Bytes, (boost::int32_t*)arrbyKey, DECRYPT_ROUNDS);
	pbyInCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	// Pad����ֻ�����˵�һ���ֽڵ����3bit����5bit�������
	int nPadLength = arrbyFirst8Bytes[0] & 0x07;

	// ����ԭʼ���ݵĳ���
	int nPlainDataLength = nInBufferLength - 1/*PadLength Length*/ - nPadLength - ZERO_LENGTH;
	if(nPlainDataLength <= 0 || pbyOutBuffer == NULL)
	{
		return 0;
	}

	// OutBuffer����
	if(nPlainDataLength > nOutBufferLength)
	{
		return 0;
	}

	// ǰһ������ĺ����ģ����ں����������
	char* pbyLast8BytesCryptedData = pbyInBuffer;
	char* pbyLast8BytesPlainData = arrbyFirst8Bytes;

	// ����һ����Pad��Ϣ֮��������Ƶ����Buffer
	for(int i=0; i < 7/*ENCRYPT_BLOCK_LENGTH_IN_BYTE - 1*/ - nPadLength; ++i)
	{
		*pbyOutCursor++ = arrbyFirst8Bytes[1+nPadLength + i];
	}

	// ���ܳ������һ����������п�
	// ͬ���ܹ��̣�����ע�͵��ģ��ǲ��Ķ�InBuffer��

	char arrbySrcBuffer[8] = {0};
	//for(int i=8; i<nInBufferLength-8; i+=8)
	while(pbyInCursor - pbyInBuffer < nInBufferLength - 8)
	{
		memcpy(arrbySrcBuffer, pbyInCursor, 8);
		// ����һ��8char�������
		for(int i=0; i<8; ++i)
		{
			arrbySrcBuffer[i] ^= pbyLast8BytesPlainData[i];
		}
		xtea((boost::int32_t*)arrbySrcBuffer, (boost::int32_t*)pbyOutCursor, (boost::int32_t*)arrbyKey, DECRYPT_ROUNDS);
		// ����һ��8char�������
		for(int i=0; i<8; ++i)
		{
			pbyOutCursor[i] ^= pbyLast8BytesCryptedData[i];
		}

		pbyLast8BytesCryptedData = pbyInCursor;
		pbyLast8BytesPlainData = pbyOutCursor;

		pbyInCursor += 8;
		pbyOutCursor += 8;
	}

	// ֱ�Ӹ�InBuffer�İ汾
/*	while(pbyInCursor - pbyInBuffer < nInBufferLength - ENCRYPT_BLOCK_LENGTH_IN_BYTE)
	{
		// ����һ��8Byte�������
		for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyInCursor[i] ^= pbyLast8BytesPlainData[i];
		}
		xtea((Int32*)pbyInCursor, (Int32*)pbyOutCursor, (Int32*)arrbyKey, DECRYPT_ROUNDS);
		// ����һ��8Byte�������
		for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
		{
			pbyOutCursor[i] ^= pbyLast8BytesCryptedData[i];
		}

		pbyLast8BytesCryptedData = pbyInCursor;
		pbyLast8BytesPlainData = pbyOutCursor;

		pbyInCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
		pbyOutCursor += ENCRYPT_BLOCK_LENGTH_IN_BYTE;
	}
*/
	// ���8Byte�� �����7Byte��У��
	char arrbyLast8Bytes[ENCRYPT_BLOCK_LENGTH_IN_BYTE] = {0};
	// ����һ��8Byte�������
	memcpy(arrbySrcBuffer, pbyInCursor, 8);
	for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		arrbySrcBuffer[i] ^= pbyLast8BytesPlainData[i];
	}
	xtea((boost::int32_t*)arrbySrcBuffer, (boost::int32_t*)arrbyLast8Bytes, (boost::int32_t*)arrbyKey, DECRYPT_ROUNDS);
	// ����һ��8Byte�������
	for(int i=0; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		arrbyLast8Bytes[i] ^= pbyLast8BytesCryptedData[i];
	}

	// У������0
	for(int i=1; i<ENCRYPT_BLOCK_LENGTH_IN_BYTE; ++i)
	{
		if(arrbyLast8Bytes[i] != 0)
		{
			return 0;
		}
	}
	*pbyOutCursor = arrbyLast8Bytes[0];

	return nPlainDataLength;
}
