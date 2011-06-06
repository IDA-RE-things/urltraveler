#if ! defined( BASE_64_CODING_CLASS_HEADER )

/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** You can use it any way you like as long as you don't try to sell it.
**
** Any attempt to sell WFC in source code form must have the permission
** of the original author. You can produce commercial executables with
** WFC but you can't sell WFC.
**
** Copyright, 2000, Samuel R. Blackburn
**
** $Workfile: CBase64Coding.hpp $
** $Revision: 3 $
** $Modtime: 1/04/00 4:39a $
*/

#define BASE_64_CODING_CLASS_HEADER

#ifdef UTILITY_EXPORTS
#define UTILITY_API __declspec(dllexport)
#else
#define UTILITY_API __declspec(dllimport)
#endif

#include <string>

using namespace std;


class CBase64Coding
{ 
private:
	static const string CBase64Coding::Base64Table;
	static const string::size_type CBase64Coding::DecodeTable[];

public:

	CBase64Coding();
	virtual ~CBase64Coding();

	/**
	* @function	�Ը������ַ����������Basd64����
	* @param		source	��Ҫ������ַ���
	* @param		len		�ַ����ĳ���
	* @retrun	�������ַ���
	*/
	static string Encode( const char * source, int len);

	/**
	* @function ���������ַ��� Base64����
	* @param		data	��Ҫ����base64������ַ���
	* @return	�����Ľ��
	*/
	static string Encode(const string & data);

	/**
	* @function �Ը�����Base64������н���
	* @param		data	��Ҫ�����Base64�ַ���
	* @return	�����Ľ��
	*/
	static string Decode(const string& data);
};

#endif // BASE_64_CODING_CLASS_HEADER
