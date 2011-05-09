#pragma  once

#include "StdAfx.h"
#include "SndaBase.h"
#include <string>
#include <vector>

using namespace  std;

class UTILITY_API StringHelper
{
public:
	StringHelper(void);
	~StringHelper(void);
	
	static string	MakeLower( string in_str);
	static string	MakeUpper( string in_str );
	
	static string	ConvertFromInt( long	in_int);
	static int		ConvertToInt( string in_str);

	static wstring	Utf8ToUnicode( string strUtf8);
	static string	UnicodeToUtf8( wstring strUnicode);

	static wstring	ANSIToUnicode( string strAnsi);
	static string	UnicodeToANSI( wstring strUnicode);

	static string	ANSIToUft8( string strAnsi);
	static string	Utf8ToANSI( string strUtf8);

	static string	Reverse(string& src_string);
	static bool	IsNumberic(const string& num); 
	static string	TrimAll( string src_str);

	// ���ӷָ��ִ����� [5/7/2011 linjinming]
	static std::vector<std::string> Split(std::string strOri, char _Ch);

	// srcStrΪԴ�ַ�����������������Ӵ������滻
	// replacedStrΪ�滻�ַ���
	// replacingStrΪ��Ҫ�滻���ַ���
	// ���������滻����ַ���
	// ���磬�����Ҫ��abcedef�е�e�滻Ϊm�����÷�����
	// Replace("abcedef", "e", "m");
	static string	Replace( std::string strSrc, 
							const std::string &strReplaced, 
							const std::string &strReplacing);
};
