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
//����: ����һ���ַ������ַ�������
//----------------------------------------------------------------------------------------
String::String(TCHAR ch)
{
	m_strString.push_back(ch);
}

//----------------------------------------------------------------------------------------
//����: �������캯��
//----------------------------------------------------------------------------------------
String::String( String& src )
{
	LPCTSTR pszStr = src.GetData();
	Assign(pszStr);
}

//----------------------------------------------------------------------------------------
//����: Ĭ�Ϲ��캯��
//����: ����ָ�����ַ��������ɶ���
//����: 
//	@param	lpszSrc	Դ�ַ���
//	@param	nLen		ԭ�ַ�����ʹ�õ��ĳ��ȣ�Ĭ��Ϊ-1����ʾȫ�������������й���
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
//����: Compare
//����: ��ǰ�ַ���lpszString���бȽϡ��Ƚ����ִ�Сд
//����: �����ȣ�����0�������ǰ�ַ���С��lpszString���򷵻�-1�����򷵻�1
//----------------------------------------------------------------------------------------
int	String::Compare(LPCTSTR lpszString)
{
	return m_strString.compare(lpszString);
}

//----------------------------------------------------------------------------------------
//����: Compare
//����: ��ǰ�ַ���lpszString���бȽϡ��Ƚϲ����ִ�Сд
//����: �����ȣ�����0�������ǰ�ַ���С��lpszString���򷵻�-1�����򷵻�1
//----------------------------------------------------------------------------------------
int	String::CompareNoCase(LPCTSTR lpszString)
{
	LPCTSTR lpsz = (LPCTSTR)m_strString.c_str();
	return _tcsicmp(lpsz, lpszString);
}


//----------------------------------------------------------------------------------------
//����: IsEmpty
//����: �ж��Ƿ��ǿ��ַ���������ַ���Ϊ""����Ϊ���ַ���������true�����򷵻�false
//----------------------------------------------------------------------------------------
bool String::IsEmpty() const
{
	return m_strString.empty();
}

//----------------------------------------------------------------------------------------
//����: Clear
//����: ���ַ������
//----------------------------------------------------------------------------------------
void String::Clear()
{
	m_strString.clear();
}

//----------------------------------------------------------------------------------------
//����: GetLength
//����: �����ַ����ĳ���
//----------------------------------------------------------------------------------------
int String::GetLength() const
{
	return (int)m_strString.size();
}

//----------------------------------------------------------------------------------------
//����: GetAt
//����: ����ָ��λ�õ��ַ�
//----------------------------------------------------------------------------------------
TCHAR String::GetAt(int nIndex) const
{
	return m_strString.at(nIndex);
}

//----------------------------------------------------------------------------------------
//����: PushBack
//����: �ڵ�ǰ�ַ�����β�����һ���ַ�
//����: 
//	@param	ch		��Ҫ��β����ӵ��ַ�
//	@param	nCount	�ַ�����ظ��Ĵ���
//����: ���֮����ַ���
//----------------------------------------------------------------------------------------	
String	String::Append(TCHAR ch, uint16 nCount)
{
	for(uint16 i=0; i<nCount; i++)
		m_strString.push_back(ch);

	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//����: Append
//����: �ڵ�ǰ�ַ�����β�����һ���ַ���
//����: 
//	@param	ch		��Ҫ��β����ӵ��ַ�
//����: ���֮����ַ���
//----------------------------------------------------------------------------------------	
String	String::Append(const TCHAR* pszAppended)
{
	m_strString.append(pszAppended);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//����: Insert
//����: �ڵ�ǰ�ַ����ĸ�������λ�ò���һ���ַ�
//����: 
//	@param	ch		��Ҫ��β����ӵ��ַ�
//	@param	nIndex	�����ַ����ڵ�����
//	@param	nCount	�����ַ�ch�ĸ���
//����: ����֮����ַ���
//----------------------------------------------------------------------------------------	
String	String::Insert(TCHAR ch, size_t nIndex,size_t nCount)
{
	m_strString.insert(nIndex, nCount, ch);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//����: Insert
//����: �ڵ�ǰ�ַ����ĸ�������λ�ò���һ���ַ���
//����: 
//	@param	pszInserted		��Ҫ��β����ӵ��ַ�
//	@param	nIndex	�����ַ����ڵ�����
//����: ����֮����ַ���
//----------------------------------------------------------------------------------------	
String	String::Insert(const TCHAR* pszInserted, size_t nIndex)
{
	m_strString.insert(nIndex,pszInserted);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//����: Assign
//����: �������ַ�����ֵ����ǰ�ַ���
//����: 
//	@param	lpszSrc	Դ�ַ���
//	@param	nLen		ԭ�ַ�����ʹ�õ��ĳ��ȣ�Ĭ��Ϊ-1����ʾȫ�������������й���
//----------------------------------------------------------------------------------------
void String::Assign(LPCTSTR lpszSrc, int nLength)
{
	if(nLength == -1)
		m_strString.assign(lpszSrc);
	else if( nLength >= 0)
		m_strString.assign(lpszSrc,nLength);
}

//----------------------------------------------------------------------------------------
//����: GetData
//����: �����ַ�����ԭ������ָ��
//----------------------------------------------------------------------------------------
LPCTSTR String::GetData()
{
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//����: ValueOf
//����: ������������ת�����ַ�������
//����: 
//	@param	nValue	��Ҫת��������
//----------------------------------------------------------------------------------------
String String::ValueOf(int nValue)
{
	TCHAR szDest[32];
	_itot( nValue, szDest, 10);
	return String(szDest);
}

//----------------------------------------------------------------------------------------
//����: ValueOf
//����: ������������ת�����ַ�������
//����: 
//	@param	lValue	��Ҫת��������
//----------------------------------------------------------------------------------------
String String::ValueOf(long lValue)
{
	TCHAR szDest[32];
	_itot( lValue, szDest, 10);
	return String(szDest);
}

//----------------------------------------------------------------------------------------
//����: ValueOf
//����: ��������double����ת�����ַ�������
//����: 
//	@param	dwValue	��Ҫת��������
//----------------------------------------------------------------------------------------
String String::ValueOf(double dwValue)
{
	return String(_T(""));
}

//----------------------------------------------------------------------------------------
//����: ValueOf
//����: �������ĸ�����ת��Ϊ�ַ�������
//����: 
//	@param	flValue	��Ҫת��������
//----------------------------------------------------------------------------------------
String String::ValueOf(float flValue)
{
	return String(_T(""));
}

//----------------------------------------------------------------------------------------
//����: ValueOf
//����: ���������ַ�ת�����ַ�������
//����: 
//	@param	nValue	��Ҫת�����ַ�������
//----------------------------------------------------------------------------------------
String String::ValueOf(char ch)
{
	return String(ch);
}

//----------------------------------------------------------------------------------------
//����: ValueOf
//����: �������Ĳ�������ת�����ַ�������
//����: 
//	@param	bValue	��Ҫת���Ĳ���ֵ
//----------------------------------------------------------------------------------------
String String::ValueOf(bool bValue)
{
	if( bValue == true)
		return String(_T("1"));
	else
		return String(_T("0"));
}

//----------------------------------------------------------------------------------------
//����: TrimLeft
//����: �޳������Ŀո� Tab������
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
//����: TrimRight
//����: �޳���Ŀ��Ҳ�� Tab������
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
//����: Trim
//����: �޳��������ߵĿո�
//----------------------------------------------------------------------------------------
String String::Trim()
{
	TrimLeft();
	TrimRight();

	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//����: Find
//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ �����Ӵ�pszToFind��һ�γ��ֵ�λ�á�
//����: 
//	@param	pszToFind	��Ҫ���ҵ����ַ���
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
//----------------------------------------------------------------------------------------
int	String::Find(const TCHAR* pszToFind,  uint16 nOffset )
{
	size_t nIndex = m_strString.find(pszToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//����: ReverseFind
//����: �ӵ�ǰ���ַ���β���ĸ���ƫ��nOffset��ʼ��������Ӵ�pszToFind��һ�γ��ֵ�λ�á�
//����: 
//	@param	pszToFind	��Ҫ���ҵ����ַ���
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����-1������β����ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
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
//����: FindFirstCharOf
//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ�ҵ���һ�����ֵ�chToFind�ַ�
//����: 
//	@param	chToFind	��Ҫ���ҵ��ַ�
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
//----------------------------------------------------------------------------------------
int	String::FindFirstCharOf(TCHAR chToFind,  uint16 nOffset)
{
	size_t nIndex = m_strString.find_first_of(chToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//����: FindFirstCharOf
//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ���ҵ�һ�����ֵ��Ӵ�pszToFind�е��ַ�������
//����: 
//	@param	pszToFind	��Ҫ���ҵ����ַ���
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
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
//����: FindFirstCharNotOf
//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ���ҵ�һ������szToFind���ַ��Ŀ�ʼ����
//����: 
//	@param	pszToFindָ�����ַ�
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
//----------------------------------------------------------------------------------------
int	String::FindFirstCharNotOf(TCHAR szToFind,  uint16 nOffset)
{
	size_t nIndex = m_strString.find_first_not_of(szToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//����: FindFirstCharNotOf
//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ���ҵ�һ������pszToFind���ַ��Ŀ�ʼ����������
//	  FindFirstCharNotOf("abcd")�����ǲ��ҵ�ǰ�ַ����еĵ�һ������a������b������cҲ����d
//	  ���ַ��Ŀ�ʼ����
//����: 
//	@param	pszToFindָ�����ַ�
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
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
//����: FindLastCharOf
//����: �ӵ�ǰ���ַ���β���ĸ���ƫ��nOffset��ʼ������ҵ�һ�����ֵ�chToFind�ַ�
//����: 
//	@param	chToFind	��Ҫ���ҵ��ַ�
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
//----------------------------------------------------------------------------------------
int	String::FindLastCharOf(TCHAR chToFind,  uint16 nOffset)
{
	size_t nIndex = m_strString.find_last_of(chToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//����: FindLastCharOf
//����: �ӵ�ǰ���ַ���β���ĸ���ƫ��nOffset��ʼ������ҵ�һ���������Ӵ�pszToFind�е��ַ�
//	  ������
//����: 
//	@param	pszToFind	��Ҫ���ҵ����ַ���
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
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
//����: FindLastCharNotOf
//����: �ӵ�ǰ���ַ�����β��ƫ��nOffsetλ�÷�����ҵ�һ������szToFind���ַ��Ŀ�ʼ����
//����: 
//	@param	pszToFindָ�����ַ�
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
//----------------------------------------------------------------------------------------
int	String::FindLastCharNotOf(TCHAR szToFind, uint16 nOffset)
{
	size_t nIndex = m_strString.find_last_not_of(szToFind, nOffset);
	if( nIndex == NPOS)
		return -1;

	return (int)nIndex;
}

//----------------------------------------------------------------------------------------
//����: FindLastCharNotOf
//����: �ӵ�ǰ���ַ���β��ƫ��ΪnOffsetλ�ÿ�ʼ���ҵ�һ������pszToFind���ַ��Ŀ�ʼ����������
//	  FindLastCharNotOf("abcd")�����ǲ��ҵ�ǰ�ַ�����β����ʼ�ĵ�һ������a��
//	 ����b������cҲ����d ���ַ��Ŀ�ʼ����
//����: 
//	@param	pszToFindָ�����ַ�
//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
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
//����: SubStr
//����: ��ȡ��ǰ�ַ���������Χ�ڵ����ַ���
//����: 
//	@param	nBegin	��ʼƫ��
//	@param	nEnd		��ֹƫ��
//����: ����nBegin��nEnd����֮����Ӵ�
//----------------------------------------------------------------------------------------
String	String::SubStr(uint16 nBegin, uint16 nNumber) const
{
	return (LPCTSTR)m_strString.substr(nBegin, nNumber).c_str();
}

//----------------------------------------------------------------------------------------
//����: Left
//����: ��ȡ��ǰ�ַ�������ߵ�nLength���ַ�
//----------------------------------------------------------------------------------------
String	String::Left(uint16 nLength) const
{
	if( nLength < 0) nLength = 0;
	if( nLength > GetLength() ) nLength = GetLength();
	return SubStr(0, nLength);
}

//----------------------------------------------------------------------------------------
//����: Mid
//����: ��ȡ��ǰ�ַ�����ָ��λ��iPos��ʼ��nLength���ַ������nLengthΪ-1����Ĭ��Ϊ��
//	  �ַ�����β
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
//����: Right
//����: ��ȡ��ǰ�ַ������ұ߿�ʼ��nLength���ַ���
//----------------------------------------------------------------------------------------
String	String::Right(uint16 nLength) const
{
	int nLeftNumber = GetLength() - nLength;
	if( nLeftNumber < 0) nLeftNumber = 0;
	if( nLeftNumber > GetLength()) nLeftNumber = GetLength();
	return SubStr( nLeftNumber, nLength);
}


//----------------------------------------------------------------------------------------
//����: ToUpper��ToLower
//����: ����ǰ�ַ����е��ַ�ȫ��ת��Ϊ��д����Сд
//����: ת��֮����ַ���
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
//����: Reverse
//����: ����ǰ�ַ�����ɷ���
//����: ����ת��֮����ַ���
//----------------------------------------------------------------------------------------
String	String::Reverse()
{
	std::reverse( m_strString.begin(), m_strString.end());
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//����: Replace
//����: ����ǰ�ַ����и�������nBegin��ʼ��nNumber���ַ��滻ΪpszReplacing
//����: 
//	@param	nBegin	��ʼƫ��
//	@param	nNumber		���ַ�������Ŀ
//	@param	pszReplacing	�滻����ַ���
//����: ��ǰ�ַ����д�nBegin��ʼ�����nNumber���ַ�
//----------------------------------------------------------------------------------------	
String	String::Replace( int nBegin, int nNumber, const TCHAR* pszAfterReplcing)
{
	m_strString.replace(nBegin, nNumber, pszAfterReplcing);
	return (LPCTSTR)m_strString.c_str();
}

//----------------------------------------------------------------------------------------
//����: Replace
//����: ����ǰ�ַ����е����е��ַ���pszBefore�滻ΪpszAfter
//����: 
//	@param	pszBefore		�滻ǰ���ַ���
//	@param	pszAfter		�滻����ַ���
//����: �滻����ַ�������ǰ���ַ�������Ҳ�ᱻ����
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
//����: Format
//����: ��ʽ�����ɵ�ǰ�ַ������ܹ�֧�ֵ�Ŀ���ַ�������Ϊ1024��
//����: 
//	@param	pstrFormat		��ʽ���ַ���
//����: �滻����ַ�������ǰ���ַ�������Ҳ�ᱻ����//----------------------------------------------------------------------------------------	

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
//����: Format
//����: ��ʽ�����ɵ�ǰ�ַ������ܹ�֧�ֵ�Ŀ���ַ�������Ϊ64��
//����: 
//	@param	pstrFormat		��ʽ���ַ���
//����: �滻����ַ�������ǰ���ַ�������Ҳ�ᱻ����
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
//����: IsNumeric
//����: ��ǰ�ַ����Ƿ�ȫ��������
//����: ���ȫ�������֣��򷵻�true�����򷵻�false
//----------------------------------------------------------------------------------------	
bool	String::IsNumeric()
{
	return m_strString.find_first_not_of(_T("0123456789.")) == string::npos;
}
