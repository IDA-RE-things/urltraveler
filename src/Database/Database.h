#pragma  once


// 描述收藏夹相关的数据库以及表格
#define FAVORITE_FLLENAME		"fav.db"


#define FAVORITE_TABLE			"fav_icon"
#define FAVORITE_CREATE_TABLE_FIELDS	" url,data  "
#define FAVORITE_CREATE_TABLE			"CREATE TABLE fav_icon (  id INTEGER PRIMARY KEY, url LONGVARCHAR UNIQUE, data BLOB )" 


/** 二进制缓冲区
包含了缓冲区的长度和缓冲区
*/
class CBinaryBuffer
{
public:
	///缓冲区长度
	UINT m_uiBufferLen;
	///缓冲区
	char m_binaryBuffer[DB_MAX_LEN_BINARY_BUFFER];

	///缺省memset初始化构造函数
	MEMSET_CONSTRUCTOR(CBinaryBuffer);
};