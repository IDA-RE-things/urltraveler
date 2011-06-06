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
	* @function	对给定的字符串数组进行Basd64编码
	* @param		source	需要编码的字符串
	* @param		len		字符串的长度
	* @retrun	编码后的字符串
	*/
	static string Encode( const char * source, int len);

	/**
	* @function 将给定的字符串 Base64编码
	* @param		data	需要进行base64编码的字符串
	* @return	编码后的结果
	*/
	static string Encode(const string & data);

	/**
	* @function 对给定的Base64编码进行解码
	* @param		data	需要解码的Base64字符串
	* @return	解码后的结果
	*/
	static string Decode(const string& data);
};

#endif // BASE_64_CODING_CLASS_HEADER
