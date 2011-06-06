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
* Using:	   由于目前的Message机制不能附带额外的数据，因此对于复杂的数据，在发送消息通知后
*			   上层需要来获取消息，为此Web模块获取额数据必须能够缓存起来
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

