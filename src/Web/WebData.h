#pragma once

#include "WebDefine.h"
#include <map>
#include <list>
#include <vector>
#include <string>

using namespace std;

/**************************************************************
* ClassName:   CWebData
* Author:	   Zhang Z.Q
* Using:	   ����Ŀǰ��Message���Ʋ��ܸ�����������ݣ���˶��ڸ��ӵ����ݣ��ڷ�����Ϣ֪ͨ��
*			   �ϲ���Ҫ����ȡ��Ϣ��Ϊ��Webģ���ȡ�����ݱ����ܹ���������
* Verion:
*		Created			2007,12,12
**************************************************************/
typedef struct STFilterWords
{
	wstring	wstrFilterWord;
	uint8	nType;

} STFilterWords;


class CWebData
{
public:

	CWebData(void);
	~CWebData(void);
};

