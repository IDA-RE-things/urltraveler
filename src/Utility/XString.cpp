#include "stdafx.h"
#include "XString.h"
#include "algorithm"
#include "tchar.h"

using namespace std;

String::String()
{
	m_strString = _T("");
}


String::~String()
{
}

//----------------------------------------------------------------------------------------
//描述: 根据一个字符生成字符串对象
//----------------------------------------------------------------------------------------
String::String(TCHAR ch)
{
	m_strString.push_back(ch);
}

//----------------------------------------------------------------------------------------
//名称: 拷贝构造函数
//----------------------------------------------------------------------------------------
String::String( String& src )
{
	LPCTSTR pszStr = src.GetData();
	Assign(pszStr);
}

//----------------------------------------------------------------------------------------
//名称: 默认构造函数
//描述: 根据指定的字符串，生成对象
//参数: 
//	@param	lpszSrc	源字符串
//	@param	nLen		原字符串中使用到的长度，默认为-1，表示全部被是用来进行构造
//----------------------------------------------------------------------------------------
String::String(LPCTSTR lpszSrc, int nLen)
{
	ASSERT(!::IsBadStringPtr(lpszSrc,-1));

	Assign(lpszSrc, nLen);
}

String::operator LPCTSTR() const 
{ 
	return (LPCTSTR)m_strString.c_str();
}

TCHAR String::operator[] (int nIndex) const
{
	return (TCHAR)m_strString.at(nIndex);
}

const String& String::operator=(const String& src)
{
	Assign(src);
	return *this;
}

const String& String::operator=(const TCHAR ch)
{
	Clear();
	m_strString.push_back(ch);
	return *this;
}

const String& String::operator=(LPCTSTR lpszStr)
{
	ASSERT(!::IsBadStringPtr(lpszStr,-1));

	Assign(lpszStr);
	return *this;
}

const String& String::operator+(const String& src)
{
	Append(src);
	return *this;
}

const String& String::operator+(const TCHAR ch)
{
	Append(ch);
	return *this;
}

const String& String::operator+(LPCTSTR lpszStr)
{
	Append(lpszStr);
	return *this;
}

const String& String::operator+=(const String& src)
{
	Append(src);
	return *this;
}

const String& String::operator+=(LPCTSTR pstr)
{
	ASSERT(!::IsBadStringPtr(pstr,-1));
	Append(pstr);
	return *this;
}

const String& String::operator+=(const TCHAR ch)
{
	Append(ch);
	return *this;
}


bool String::operator == (LPCTSTR str) const
{
	return m_strString.compare(str) == 0;
}

bool String::operator != (LPCTSTR str) const
{
	return m_strString.compare(str) != 0;
}

bool String::operator <= (LPCTSTR str) const
{
	return m_strString.compare(str) <= 0;
}

bool String::operator <  (LPCTSTR str) const
{
	return m_strString.compare(str) < 0;
}

bool String::operator >= (LPCTSTR str) const
{
	return m_strString.compare(str) >= 0;
}

bool String::operator >  (LPCTSTR str) const
{
	return m_strString.compare(str) > 0;
}

//----------------------------------------------------------------------------------------
//名称: Compare
//描述: 当前字符与lpszString进行比较。比较区分大小写
//返回: 如果相等，返回0，如果当前字符串小于lpszString，则返回-1，否则返回1
//----------------------------------------------------------------------------------------
int	String::Compare(LPCTSTR lpszString)
{
	return m_strString.compare(lpszString);
}

//----------------------------------------------------------------------------------------
//名称: Compare
//描述: 当前字符与lpszString进行比较。比较不区分大小写
//返回: 如果相等，返回0，如果当前字符串小于lpszString，则返回-1，否则返回1
//----------------------------------------------------------------------------------------
int	String::CompareNoCase(LPCTSTR lpszString)
{
	LPCTSTR lpsz = (LPCTSTR)m_strString.c_str();
	return _tcsicmp(lpsz, lpszString);
}


//----------------------------------------------------------------------------------------
//名称: IsEmpty
//描述: 判断是否是空字符串。如果字符串为""，则为空字符串，返回true，否则返回false
//----------------------------------------------------------------------------------------
bool String::IsEmpty() const
{
	return m_strString.empty();
}

//----------------------------------------------------------------------------------------
//名称: Clear
//描述: 将字符串清空
//----------------------------------------------------------------------------------------
void String::Clear()
{
	m_strString.clear();
}

//----------------------------------------------------------------------------------------
//名称: GetLength
//描述: 返回字符串的长度
//----------------------------------------------------------------------------------------
int String::GetLength() const
{
	return (int)m_strString.size();
}

//----------------------------------------------------------------------------------------
//名称: GetAt
//描述: 返回指定位置的字符
//----------------------------------------------------------------------------------------
TCHAR String::GetAt(int nIndex) const
{
	return m_strString.at(nIndex);
}

//----------------------------------------------------------------------------------------
//名称: PushBack
//描述: 在当前字符串的尾部添加一个字符
//参数: 
//	@param	ch		需要在尾部添加的字符
//	@param	nCount	字符添加重复的次数
//返回: 添加之后的字符串
//----------------------------------------------------------------------------------------	
String	String::Append(TCHAR ch, uint16 nCount)
{
	for(uint16 i=0; i<nCount; i++)
		m_strString.push_back(ch);

	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Append
//描述: 在当前字符串的尾部添加一个字符串
//参数: 
//	@param	ch		需要在尾部添加的字符
//返回: 添加之后的字符串
//----------------------------------------------------------------------------------------	
String	String::Append(const TCHAR* pszAppended)
{
	m_strString.append(pszAppended);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Insert
//描述: 在当前字符串的给定索引位置插入一个字符
//参数: 
//	@param	ch		需要在尾部添加的字符
//	@param	nIndex	插入字符所在的索引
//	@param	nCount	插入字符ch的个数
//返回: 插入之后的字符串
//----------------------------------------------------------------------------------------	
String	String::Insert(TCHAR ch, size_t nIndex,size_t nCount)
{
	m_strString.insert(nIndex, nCount, ch);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Insert
//描述: 在当前字符串的给定索引位置插入一个字符串
//参数: 
//	@param	pszInserted		需要在尾部添加的字符
//	@param	nIndex	插入字符所在的索引
//返回: 插入之后的字符串
//----------------------------------------------------------------------------------------	
String	String::Insert(const TCHAR* pszInserted, size_t nIndex)
{
	m_strString.insert(nIndex,pszInserted);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Assign
//描述: 将给定字符串赋值给当前字符串
//参数: 
//	@param	lpszSrc	源字符串
//	@param	nLen		原字符串中使用到的长度，默认为-1，表示全部被是用来进行构造
//----------------------------------------------------------------------------------------
void String::Assign(LPCTSTR lpszSrc, int nLength)
{
	if(nLength == -1)
		m_strString.assign(lpszSrc);
	else if( nLength >= 0)
		m_strString.assign(lpszSrc,nLength);
}

//----------------------------------------------------------------------------------------
//名称: GetData
//描述: 返回字符串的原生数据指针
//----------------------------------------------------------------------------------------
LPCTSTR String::GetData()
{
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: ValueOf
//描述: 将给定的整数转换成字符串对象
//参数: 
//	@param	nValue	需要转换的整数
//----------------------------------------------------------------------------------------
String String::ValueOf(int nValue)
{
	TCHAR szDest[32];
	_itot( nValue, szDest, 10);
	return String(szDest);
}

//----------------------------------------------------------------------------------------
//名称: ValueOf
//描述: 将给定的整数转换成字符串对象
//参数: 
//	@param	lValue	需要转换的整数
//----------------------------------------------------------------------------------------
String String::ValueOf(long lValue)
{
	TCHAR szDest[32];
	_itot( lValue, szDest, 10);
	return String(szDest);
}

//----------------------------------------------------------------------------------------
//名称: ValueOf
//描述: 将给定的double整数转换成字符串对象
//参数: 
//	@param	dwValue	需要转换的整数
//----------------------------------------------------------------------------------------
String String::ValueOf(double dwValue)
{
	return String(_T(""));
}

//----------------------------------------------------------------------------------------
//名称: ValueOf
//描述: 将给定的浮点数转换为字符串对象
//参数: 
//	@param	flValue	需要转换的整数
//----------------------------------------------------------------------------------------
String String::ValueOf(float flValue)
{
	return String(_T(""));
}

//----------------------------------------------------------------------------------------
//名称: ValueOf
//描述: 将给定的字符转换成字符串对象
//参数: 
//	@param	nValue	需要转换的字符串整数
//----------------------------------------------------------------------------------------
String String::ValueOf(char ch)
{
	return String(ch);
}

//----------------------------------------------------------------------------------------
//名称: ValueOf
//描述: 将给定的布尔变量转换成字符串对象
//参数: 
//	@param	bValue	需要转换的布尔值
//----------------------------------------------------------------------------------------
String String::ValueOf(bool bValue)
{
	if( bValue == true)
		return String(_T("1"));
	else
		return String(_T("0"));
}

//----------------------------------------------------------------------------------------
//名称: TrimLeft
//描述: 剔除最左侧的空格、 Tab等内容
//----------------------------------------------------------------------------------------
String String::TrimLeft()
{
	if ( !m_strString.empty())
	{
		m_strString.erase( 0, m_strString.find_first_not_of(_T(" \r\n\t")));
	}

	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: TrimRight
//描述: 剔除最的空右侧格、 Tab等内容
//----------------------------------------------------------------------------------------
String String::TrimRight()
{
	if (!m_strString.empty())
	{
		m_strString.erase( m_strString.find_last_not_of(_T(" \r\n\t")) + 1, m_strString.length()-1);
	}

	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Trim
//描述: 剔除左右两边的空格
//----------------------------------------------------------------------------------------
String String::Trim()
{
	TrimLeft();
	TrimRight();

	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Find
//描述: 从当前的字符串的给定偏移nOffset开始 查找子串pszToFind第一次出现的位置。
//参数: 
//	@param	pszToFind	需要查找的子字符串
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::Find(const TCHAR* pszToFind,  uint16 nOffset )
{
	size_t nIndex = m_strString.find(pszToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: ReverseFind
//描述: 从当前的字符串尾部的给定偏移nOffset开始反向查找子串pszToFind第一次出现的位置。
//参数: 
//	@param	pszToFind	需要查找的子字符串
//	@param	nOffset	查找的起始偏移，默认是-1，即从尾部开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::ReverseFind(const TCHAR* pszToFind,  int16 nOffset)
{
	size_t nIndex = -1;
	if( nOffset == -1)
		nIndex = m_strString.rfind(pszToFind);
	else
		nIndex = m_strString.rfind(pszToFind, nOffset);

	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}


//----------------------------------------------------------------------------------------
//名称: FindFirstCharOf
//描述: 从当前的字符串的给定偏移nOffset开始找到第一个出现的chToFind字符
//参数: 
//	@param	chToFind	需要查找的字符
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::FindFirstCharOf(TCHAR chToFind,  uint16 nOffset)
{
	size_t nIndex = m_strString.find_first_of(chToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: FindFirstCharOf
//描述: 从当前的字符串的给定偏移nOffset开始查找第一个出现的子串pszToFind中的字符的索引
//参数: 
//	@param	pszToFind	需要查找的子字符串
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::FindFirstCharOf(const TCHAR* pszToFind, uint16 nOffset)
{
	ASSERT(!::IsBadStringPtr(pszToFind,-1));

	size_t nIndex = m_strString.find_first_of(pszToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: FindFirstCharNotOf
//描述: 从当前的字符串的给定偏移nOffset开始查找第一个不是szToFind的字符的开始索引
//参数: 
//	@param	pszToFind指定的字符
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::FindFirstCharNotOf(TCHAR szToFind,  uint16 nOffset)
{
	size_t nIndex = m_strString.find_first_not_of(szToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: FindFirstCharNotOf
//描述: 从当前的字符串的给定偏移nOffset开始查找第一个不是pszToFind中字符的开始索引，比如
//	  FindFirstCharNotOf("abcd")，则是查找当前字符串中的第一个不是a，不是b，不是c也不是d
//	  的字符的开始索引
//参数: 
//	@param	pszToFind指定的字符
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::FindFirstCharNotOf(const TCHAR* pszToFind,  uint16 nOffset )
{
	ASSERT(!::IsBadStringPtr(pszToFind,-1));

	size_t nIndex = m_strString.find_first_not_of(pszToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: FindLastCharOf
//描述: 从当前的字符串尾部的给定偏移nOffset开始反向查找第一个出现的chToFind字符
//参数: 
//	@param	chToFind	需要查找的字符
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::FindLastCharOf(TCHAR chToFind,  uint16 nOffset)
{
	size_t nIndex = m_strString.find_last_of(chToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: FindLastCharOf
//描述: 从当前的字符串尾部的给定偏移nOffset开始反向查找第一个出现在子串pszToFind中的字符
//	  的索引
//参数: 
//	@param	pszToFind	需要查找的子字符串
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::FindLastCharOf(const TCHAR* pszToFind, uint16 nOffset)
{
	ASSERT(!::IsBadStringPtr(pszToFind,-1));

	size_t nIndex = m_strString.find_last_of(pszToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: FindLastCharNotOf
//描述: 从当前的字符串的尾部偏移nOffset位置反向查找第一个不是szToFind的字符的开始索引
//参数: 
//	@param	pszToFind指定的字符
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::FindLastCharNotOf(TCHAR szToFind, uint16 nOffset)
{
	size_t nIndex = m_strString.find_last_not_of(szToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: FindLastCharNotOf
//描述: 从当前的字符串尾部偏移为nOffset位置开始查找第一个不是pszToFind中字符的开始索引，比如
//	  FindLastCharNotOf("abcd")，则是查找当前字符串的尾部开始的第一个不是a，
//	 不是b，不是c也不是d 的字符的开始索引
//参数: 
//	@param	pszToFind指定的字符
//	@param	nOffset	查找的起始偏移，默认是0，即从头开始查找
//返回: 目标字符的索引，如果没有找到，则返回-1；
//----------------------------------------------------------------------------------------
int	String::FindLastCharNotOf(const TCHAR* pszToFind, uint16 nOffset)
{
	ASSERT(!::IsBadStringPtr(pszToFind,-1));

	size_t nIndex = m_strString.find_last_not_of(pszToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//名称: SubStr
//描述: 获取当前字符串给定范围内的子字符串
//参数: 
//	@param	nBegin	起始偏移
//	@param	nEnd		终止偏移
//返回: 返回nBegin和nEnd所以之间的子串
//----------------------------------------------------------------------------------------
String	String::SubStr(uint16 nBegin, uint16 nNumber) const
{
	return (LPCTSTR)m_strString.substr(nBegin, nNumber).c_str();
}

//----------------------------------------------------------------------------------------
//名称: Left
//返回: 获取当前字符串最左边的nLength个字符
//----------------------------------------------------------------------------------------
String	String::Left(uint16 nLength) const
{
	if( nLength < 0) nLength = 0;
	if( nLength > GetLength() ) nLength = GetLength();
	return SubStr(0, nLength);
}

//----------------------------------------------------------------------------------------
//名称: Mid
//返回: 获取当前字符串从指定位置iPos开始的nLength个字符。如果nLength为-1，则默认为到
//	  字符串结尾
//----------------------------------------------------------------------------------------
String	String::Mid(uint16 iPos, int nLength) const
{
	if( iPos > GetLength() - 1)
		return _T("");

	int nLeftNumber = GetLength()  - iPos;
	if( nLeftNumber < 0)
		nLeftNumber = 0;

	if( nLeftNumber > GetLength())
		nLeftNumber = GetLength();

	if( nLength == -1)
		nLength = nLeftNumber;
	else
	{
		if(nLength >= nLeftNumber)
			nLength = nLeftNumber;
	}

	return SubStr(iPos, nLength);
}

//----------------------------------------------------------------------------------------
//名称: Right
//返回: 获取当前字符串从右边开始的nLength个字符。
//----------------------------------------------------------------------------------------
String	String::Right(uint16 nLength) const
{
	int nLeftNumber = GetLength() - nLength;
	if( nLeftNumber < 0) nLeftNumber = 0;
	if( nLeftNumber > GetLength()) nLeftNumber = GetLength();
	return SubStr( nLeftNumber, nLength);
}


//----------------------------------------------------------------------------------------
//名称: ToUpper和ToLower
//描述: 将当前字符串中的字符全部转换为大写或者小写
//返回: 转换之后的字符串
//----------------------------------------------------------------------------------------
String	String::ToUpper()
{
	std::transform( m_strString.begin(), m_strString.end(), m_strString.begin(),toupper);
	return (LPCTSTR)m_strString.c_str();
}

String	String::ToLower()
{
	std::transform( m_strString.begin(), m_strString.end(), m_strString.begin(),tolower);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Reverse
//描述: 将当前字符串变成反向
//返回: 反向转换之后的字符串
//----------------------------------------------------------------------------------------
String	String::Reverse()
{
	std::reverse( m_strString.begin(), m_strString.end());
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Replace
//描述: 将当前字符串中给定索引nBegin开始的nNumber个字符替换为pszReplacing
//参数: 
//	@param	nBegin	起始偏移
//	@param	nNumber		子字符串的数目
//	@param	pszReplacing	替换后的字符串
//返回: 当前字符串中从nBegin开始的最多nNumber个字符
//----------------------------------------------------------------------------------------	
String	String::Replace( int nBegin, int nNumber, const TCHAR* pszAfterReplcing)
{
	m_strString.replace(nBegin, nNumber, pszAfterReplcing);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Replace
//描述: 将当前字符串中的所有的字符串pszBefore替换为pszAfter
//参数: 
//	@param	pszBefore		替换前的字符串
//	@param	pszAfter		替换后的字符串
//返回: 替换后的字符串，当前的字符串内容也会被更改
//----------------------------------------------------------------------------------------	
String	String::Replace( const TCHAR* pszBefore, const TCHAR* pszAfter)
{
	ASSERT(!::IsBadStringPtr(pszBefore,-1));
	ASSERT(!::IsBadStringPtr(pszAfter,-1));

	int nIndex = FindFirstCharOf(pszBefore);
	while( nIndex != -1)
	{
		Replace(nIndex, String(pszBefore).GetLength(), pszAfter);
		nIndex = FindFirstCharOf(pszBefore);
	}

	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//名称: Format
//描述: 格式化生成当前字符串，能够支持的目标字符串长度为1024。
//参数: 
//	@param	pstrFormat		格式化字符串
//返回: 替换后的字符串，当前的字符串内容也会被更改//----------------------------------------------------------------------------------------	

int __cdecl String::Format(LPCTSTR pstrFormat, ...)
{
	String sFormat = pstrFormat;
	// Do ordinary printf replacements
	// NOTE: Documented max-length of wvsprintf() is 1024
	TCHAR szBuffer[1025] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = ::wvsprintf(szBuffer, sFormat, argList);
	va_end(argList);
	Assign(szBuffer);
	return iRet;
}

//----------------------------------------------------------------------------------------
//名称: Format
//描述: 格式化生成当前字符串，能够支持的目标字符串长度为64。
//参数: 
//	@param	pstrFormat		格式化字符串
//返回: 替换后的字符串，当前的字符串内容也会被更改
//----------------------------------------------------------------------------------------	
int __cdecl String::SmallFormat(LPCTSTR pstrFormat, ...)
{
	String sFormat = pstrFormat;
	TCHAR szBuffer[65] = { 0 };
	va_list argList;
	va_start(argList, pstrFormat);
	int iRet = ::wvsprintf(szBuffer, sFormat, argList);
	va_end(argList);
	Assign(szBuffer);
	return iRet;
}

//----------------------------------------------------------------------------------------
//名称: IsNumeric
//描述: 当前字符串是否全部是数字
//返回: 如果全部是数字，则返回true，否则返回false
//----------------------------------------------------------------------------------------	
bool	String::IsNumeric()
{
	return m_strString.find_first_not_of(_T("0123456789.")) == string::npos;
}
