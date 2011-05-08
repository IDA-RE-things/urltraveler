#include "TSimpleTextStream.h"


TSimpleTextStream_Recv::TSimpleTextStream_Recv(unsigned char *buf,int buflen)
{
	m_str = buf;
	m_len = buflen;
	m_pos = 0;
}
int TSimpleTextStream_Recv::Pop(unsigned char *str,int maxlen,int *retlen )
{
	if(m_pos == m_len)
		return 0;
	int len;
	if((m_pos + 4) > m_len)//长度数据超过字符串长度
		return 0;

	sscanf((char const*)m_str+m_pos,("%4x"),&len);

	if(len < 0 || len >= maxlen)//长度读入不对
		return 0;

	m_pos += 4;
	if((m_pos + len) > m_len)//数据字符串长度不对
		return 0;

	int pos2 = m_pos;
	m_pos += len;
	strncpy((char*)str,(char const*)m_str+pos2,len);
	str[len]=0;
	if(retlen)
		*retlen = len;
	return 1;
}
int TSimpleTextStream_Recv::PopLen(int maxlen,int *retlen)
{
	if(m_pos == m_len)
		return 0;
	if((m_pos + 4) > m_len)//长度数据超过字符串长度
		return 0;

	sscanf((char*)m_str+m_pos,("%4x"),retlen);

	if(*retlen < 0 || *retlen >= maxlen)//长度读入不对
		return 0;

	if((m_pos + *retlen) > m_len)//数据字符串长度不对
		return 0;

	return 1;
}
int TSimpleTextStream_Recv::PopContent(unsigned char *str,int maxlen,int &len)
{
	//if(len < 0 || len >= maxlen)//长度读入不对
	//	return 0;

	m_pos += 4;
	//if((m_pos + len) > m_len)//数据字符串长度不对
	//	return 0;

	int pos2 = m_pos;
	m_pos += len;
	strncpy((char*)str,(char const*)m_str+pos2,len);
	str[len]=0;
	return 1;
}
int TSimpleTextStream_Recv::IsEnd()
{
	if(m_pos >= m_len)
		return 1;
	return 0;
}

int TSimpleTextStream_Recv::MovePos(int i)
{
	if(m_pos+i>=m_len)
		return 0;
	m_pos+=i;
	return 1;
}