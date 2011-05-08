#ifndef TSIMPLETEXTSTREAM_H_
#define TSIMPLETEXTSTREAM_H_

#include <string.h>
#include <stdio.h>

class TSimpleTextStream_Recv
{
public:
	TSimpleTextStream_Recv(unsigned char *buf,int buflen);

	int Pop(unsigned char *str,int maxlen,int *retlen = NULL);

	int PopLen(int maxlen,int *retlen);
	
	int PopContent(unsigned char *str,int maxlen,int &len);

	int IsEnd();

	int MovePos(int len);

public:
	unsigned char *m_str;
	int m_len;
	int m_pos;
};

#endif

