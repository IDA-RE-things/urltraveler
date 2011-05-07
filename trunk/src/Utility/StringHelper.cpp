#include "stdafx.h"
#include "StringHelper.h"
#include <algorithm>
#include <locale.h>
#include "algorithm"
#include <assert.h>
#include "Windows.h"
#include <locale.h>
#include <vector>

using namespace std;

StringHelper::StringHelper(void)
{
}

StringHelper::~StringHelper(void)
{
}

string	StringHelper::MakeLower( string in_str)
{
	string instr = in_str;
	std::transform( instr.begin(), instr.end(), instr.begin(),tolower);
	
	return instr;
}

string	StringHelper::MakeUpper( string in_str )
{
	string instr = in_str;
	std::transform( instr.begin(), instr.end(), instr.begin(),toupper);
	
	return instr;
}

string	StringHelper::ConvertFromInt( long	in_int)
{
	char instr[256];
	sprintf(instr, "%d", in_int);
	return instr;
}

int	StringHelper::ConvertToInt( string in_str)
{
	return atoi( in_str.c_str());
}


string	StringHelper::Reverse(string& src_string)
{
	string inner_src = src_string;
	std::reverse( inner_src.begin(), inner_src.end());
	
	return inner_src;
}

bool StringHelper::IsNumberic(const string& num) 
{ 
	return num.find_first_not_of("0123456789.") == string::npos;
} 

string StringHelper::TrimAll( string src_str )
{
	if (src_str.empty())
	{
		return src_str;
	}
	src_str.erase(0,src_str.find_first_not_of(" \r\n\t"));
	src_str.erase(src_str.find_last_not_of(" \r\n\t") + 1);
	return src_str;
}

wstring	StringHelper::Utf8ToUnicode( string strUtf8)
{
	int len = ::MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);   
    if (len == 0) return L"";   
	
    std::vector<wchar_t> unicode(len);   
    ::MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, &unicode[0], len);   
	
    return &unicode[0];   
}

string	StringHelper::UnicodeToUtf8( wstring strUnicode)
{
	int len = ::WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);   
    if (len == 0) return "";   
	
    std::vector<char> utf8(len);   
    ::WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, &utf8[0], len, NULL, NULL);   
	
    return &utf8[0];   
}

wstring	StringHelper::ANSIToUnicode( string strAnsi)
{
    int len = ::MultiByteToWideChar(CP_ACP, 0, strAnsi.c_str(), -1, NULL, 0);   
    if (len == 0) return L"";   
	
    std::vector<wchar_t> unicode(len);   
    ::MultiByteToWideChar(CP_ACP, 0, strAnsi.c_str(), -1, &unicode[0], len);   
	
    return &unicode[0];   
}

string	StringHelper::UnicodeToANSI( wstring strUnicode)
{
    int len = ::WideCharToMultiByte(CP_ACP, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);   
    if (len == 0) return "";   
	
    std::vector<char> utf8(len);   
    ::WideCharToMultiByte(CP_ACP, 0, strUnicode.c_str(), -1, &utf8[0], len, NULL, NULL);   
	
    return &utf8[0];   
}


string	StringHelper::Replace( string strSrc, 
							  const string &strReplaced, 
							  const string &strReplacing )
{
	string strResult = strSrc;

	string::size_type pos = 0;
	string::size_type srclen = strReplaced.size();
	string::size_type dstlen = strReplacing.size();
	
	while( (pos =strSrc.find(strReplaced, pos)) != string::npos )
	{
		strResult.replace( pos, srclen, strReplacing );
		pos += dstlen;
	}

	return strResult;
}

std::vector<std::string> StringHelper::Split(std::string strOri, char _Ch)
{
	int i = 0;
	int nLastPos = 0;
	std::vector<std::string> vecResutl;
	std::string strSub;

	i = strOri.find(_Ch);

	while (i != -1)
	{
		strSub = strOri.substr(nLastPos, i - nLastPos);
		nLastPos = i + 1;
		i = strOri.find(_Ch, nLastPos);

		vecResutl.push_back(strSub);
	}

	if (nLastPos != 0)
	{
		strSub = strOri.substr(nLastPos, strOri.length() - nLastPos);
		vecResutl.push_back(strSub);
	}

	return vecResutl;
}
