#pragma  once


// �����ղؼ���ص����ݿ��Լ����
#define FAVORITE_FLLENAME		"fav.db"


#define FAVORITE_TABLE			"fav_icon"
#define FAVORITE_CREATE_TABLE_FIELDS	" url,data  "
#define FAVORITE_CREATE_TABLE			"CREATE TABLE fav_icon (  id INTEGER PRIMARY KEY, url LONGVARCHAR UNIQUE, data BLOB )" 


/** �����ƻ�����
�����˻������ĳ��Ⱥͻ�����
*/
class CBinaryBuffer
{
public:
	///����������
	UINT m_uiBufferLen;
	///������
	char m_binaryBuffer[DB_MAX_LEN_BINARY_BUFFER];

	///ȱʡmemset��ʼ�����캯��
	MEMSET_CONSTRUCTOR(CBinaryBuffer);
};