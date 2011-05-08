/********************************************************************
created:	2008/02/18
created:	18:2:2008   9:52

*********************************************************************/

#pragma once

namespace Auth51
{
class CXTEA
{
public:
	static int Encrypt(char* pbyInBuffer, int nInBufferLength, char* pbyOutBuffer, int nOutBufferLength, char arrbyKey[16]);
	static int Decrypt(char* pbyInBuffer, int nInBufferLength, char* pbyOutBuffer, int nOutBufferLength, char arrbyKey[16]);
};
}
