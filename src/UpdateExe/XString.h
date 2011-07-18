#ifndef __OPENUI_STRING_H_
#define __OPENUI_STRING_H_

#pragma once
#include "stdafx.h"
#include "SndaBase.h"
#include <string>
#include "SndaBase.h"

using namespace std;

class String
{
public:
	enum
	{
		NPOS	=	0xffffffff,	
	};
	//----------------------------------------------------------------------------------------
	//����: ���ɿ��ַ�������
	//----------------------------------------------------------------------------------------
	String();

	//----------------------------------------------------------------------------------------
	//����: ����һ���ַ������ַ�������
	//----------------------------------------------------------------------------------------
	String(TCHAR ch);

	//----------------------------------------------------------------------------------------
	//����: ���ɵ�Ĭ�ϵ���ɫֵΪ��ɫ
	//----------------------------------------------------------------------------------------
	String(String& src );

	//----------------------------------------------------------------------------------------
	//����: ����ָ�����ַ��������ɶ���
	//����: 
	//	@param	lpszSrc	Դ�ַ���
	//	@param	nLen		ԭ�ַ�����ʹ�õ��ĳ��ȣ�Ĭ��Ϊ-1����ʾȫ�������������й���
	//----------------------------------------------------------------------------------------
	String(LPCTSTR lpszSrc, int nLen = -1);

	virtual ~String();

	//----------------------------------------------------------------------------------------
	//����: ��������ؽӿ�
	//----------------------------------------------------------------------------------------
	operator LPCTSTR() const;
	TCHAR operator[] (int nIndex) const;

	const String& operator=(const String& src);
	const String& operator=(String& src);
	const String& operator=(LPCTSTR lpszStr);
	const String& operator=(LPTSTR lpszStr);
	const String& operator=(const TCHAR ch);

	const String& operator+(const String& src);
	const String& operator+(String& src);
	const String& operator+(LPCTSTR lpszStr);
	const String& operator+(LPTSTR lpszStr);
	const String& operator+(const TCHAR ch);

	const String& operator+=(const String& src);
	const String& operator+=(String& src);
	const String& operator+=(LPCTSTR pstr);
	const String& operator+=(LPTSTR pstr);
	const String& operator+=(const TCHAR ch);

	bool operator == (LPCTSTR str) const;
	bool operator != (LPCTSTR str) const;
	bool operator <= (LPCTSTR str) const;
	bool operator <  (LPCTSTR str) const;
	bool operator >= (LPCTSTR str) const;
	bool operator >  (LPCTSTR str) const;

	//----------------------------------------------------------------------------------------
	//����: ���Խӿ�
	//----------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------
	//����: Compare
	//����: ��ǰ�ַ���lpszString���бȽϡ��Ƚ����ִ�Сд
	//����: �����ȣ�����0�������ǰ�ַ���С��lpszString���򷵻�-1�����򷵻�1
	//----------------------------------------------------------------------------------------
	int	Compare(LPCTSTR lpszString);

	//----------------------------------------------------------------------------------------
	//����: Compare
	//����: ��ǰ�ַ���lpszString���бȽϡ��Ƚϲ����ִ�Сд
	//����: �����ȣ�����0�������ǰ�ַ���С��lpszString���򷵻�-1�����򷵻�1
	//----------------------------------------------------------------------------------------
	int	CompareNoCase(LPCTSTR lpszString);

	//----------------------------------------------------------------------------------------
	//����: IsEmpty
	//����: �ж��Ƿ��ǿ��ַ���������ַ���Ϊ""����Ϊ���ַ���������true�����򷵻�false
	//----------------------------------------------------------------------------------------
	bool	IsEmpty() const;

	//----------------------------------------------------------------------------------------
	//����: Clear
	//����: ���ַ������
	//----------------------------------------------------------------------------------------
	void	Clear();

	//----------------------------------------------------------------------------------------
	//����: GetLength
	//����: �����ַ����ĳ���
	//----------------------------------------------------------------------------------------
	int		GetLength() const;

	//----------------------------------------------------------------------------------------
	//����: GetAt
	//����: ����ָ��λ�õ��ַ�
	//----------------------------------------------------------------------------------------
	TCHAR	GetAt(int nIndex) const;

	//----------------------------------------------------------------------------------------
	//����: Append
	//����: �ڵ�ǰ�ַ�����β������һ���ַ�
	//����: 
	//	@param	ch		��Ҫ��β�����ӵ��ַ�
	//	@param	nCount	�ַ������ظ��Ĵ���
	//����: ����֮����ַ���
	//----------------------------------------------------------------------------------------	
	String	Append(TCHAR ch, uint16 nCount = 1);

	//----------------------------------------------------------------------------------------
	//����: Append
	//����: �ڵ�ǰ�ַ�����β������һ���ַ���
	//����: 
	//	@param	pszAppended		��Ҫ��β�����ӵ��ַ���
	//����: ����֮����ַ���
	//----------------------------------------------------------------------------------------	
	String	Append(const TCHAR* pszAppended);

	//----------------------------------------------------------------------------------------
	//����: Insert
	//����: �ڵ�ǰ�ַ����ĸ�������λ�ò���һ���ַ�
	//����: 
	//	@param	ch		��Ҫ��β�����ӵ��ַ�
	//	@param	nIndex	�����ַ����ڵ�����
	//	@param	nCount	�����ַ�ch�ĸ�����Ĭ��Ϊ1
	//����: ����֮����ַ���
	//----------------------------------------------------------------------------------------	
	String	Insert(TCHAR ch, size_t nIndex, size_t nCount = 1);

	//----------------------------------------------------------------------------------------
	//����: Insert
	//����: �ڵ�ǰ�ַ����ĸ�������λ�ò���һ���ַ���
	//����: 
	//	@param	pszInserted		��Ҫ��β�����ӵ��ַ�
	//	@param	nIndex	�����ַ����ڵ�����
	//����: ����֮����ַ���
	//----------------------------------------------------------------------------------------	
	String	Insert(const TCHAR* pszInserted, size_t nIndex);

	//----------------------------------------------------------------------------------------
	//����: Assign
	//����: �������ַ�����ֵ����ǰ�ַ���
	//����: 
	//	@param	lpszSrc	Դ�ַ���
	//	@param	nLen		ԭ�ַ�����ʹ�õ��ĳ��ȣ�Ĭ��Ϊ-1����ʾȫ�������������й���
	//----------------------------------------------------------------------------------------
	void		Assign(LPCTSTR lpszSrc, int nLength = -1);

	//----------------------------------------------------------------------------------------
	//����: GetData
	//����: �����ַ�����ԭ������ָ��
	//----------------------------------------------------------------------------------------
	LPCTSTR GetData();

	//----------------------------------------------------------------------------------------
	//����: ValueOf
	//����: ������������ת�����ַ�������
	//����: 
	//	@param	nValue	��Ҫת��������
	//----------------------------------------------------------------------------------------
	static String ValueOf(int nValue);

	//----------------------------------------------------------------------------------------
	//����: ValueOf
	//����: ������������ת�����ַ�������
	//����: 
	//	@param	lValue	��Ҫת��������
	//----------------------------------------------------------------------------------------
	static String ValueOf(long lValue);

	//----------------------------------------------------------------------------------------
	//����: ValueOf
	//����: ������������ת�����ַ�������
	//����: 
	//	@param	dwValue	��Ҫת��������
	//----------------------------------------------------------------------------------------
	static String ValueOf(double dwValue);

	//----------------------------------------------------------------------------------------
	//����: ValueOf
	//����: �������ĸ�����ת��Ϊ�ַ�������
	//����: 
	//	@param	flValue	��Ҫת��������
	//	@param	n		С���㾫��λ��
	//----------------------------------------------------------------------------------------
	static String ValueOf(float flValue, int n);

	//----------------------------------------------------------------------------------------
	//����: ValueOf
	//����: ���������ַ�ת�����ַ�������
	//����: 
	//	@param	nValue	��Ҫת�����ַ�������
	//----------------------------------------------------------------------------------------
	static String ValueOf(char ch);

	//----------------------------------------------------------------------------------------
	//����: ValueOf
	//����: �������Ĳ�������ת�����ַ�������
	//����: 
	//	@param	bValue	��Ҫת���Ĳ���ֵ
	//----------------------------------------------------------------------------------------
	static String ValueOf(bool bValue);

	//----------------------------------------------------------------------------------------
	//����: �����ӿ�
	//----------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------
	//����: TrimLeft
	//����: �޳������Ŀո� Tab������
	//����: ���������޳�֮����ַ�����ͬʱ��ǰ�ַ�����ֵҲ�ᱻ����
	//----------------------------------------------------------------------------------------
	String TrimLeft(TCHAR ch=_T(' '));

	//----------------------------------------------------------------------------------------
	//����: TrimRight
	//����: �޳���Ŀ��Ҳ�� Tab������
	//����: ���������޳�֮����ַ�����ͬʱ��ǰ�ַ�����ֵҲ�ᱻ����
	//----------------------------------------------------------------------------------------
	String TrimRight(TCHAR ch=_T(' '));

	//----------------------------------------------------------------------------------------
	//����: Trim
	//����: �޳��������ߵĿո�
	//����: ���������޳�֮����ַ�����ͬʱ��ǰ�ַ�����ֵҲ�ᱻ����
	//----------------------------------------------------------------------------------------
	String Trim(TCHAR ch=_T(' '));

	//----------------------------------------------------------------------------------------
	//����: Find
	//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ �����Ӵ�pszToFind��һ�γ��ֵ�λ�á�
	//����: 
	//	@param	pszToFind	��Ҫ���ҵ����ַ���
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	Find(const TCHAR* pszToFind,  uint16 nOffset = 0);

	//----------------------------------------------------------------------------------------
	//����: ReverseFind
	//����: �ӵ�ǰ���ַ���β���ĸ���ƫ��nOffset��ʼ��������Ӵ�pszToFind��һ�γ��ֵ�λ�á�
	//����: 
	//	@param	pszToFind	��Ҫ���ҵ����ַ���
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����-1������β����ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	ReverseFind(const TCHAR* pszToFind,  int16 nOffset = NPOS);

	//----------------------------------------------------------------------------------------
	//����: FindFirstCharOf
	//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ�ҵ���һ�����ֵ�chToFind�ַ�
	//����: 
	//	@param	chToFind	��Ҫ���ҵ��ַ�
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	FindFirstCharOf(TCHAR chToFind,  uint16 nOffset = 0);

	//----------------------------------------------------------------------------------------
	//����: FindFirstCharOf
	//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ���ҵ�һ�����ֵ��Ӵ�pszToFind�е��ַ�������
	//����: 
	//	@param	pszToFind	��Ҫ���ҵ����ַ���
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	FindFirstCharOf(const TCHAR* pszToFind, uint16 nOffset = 0);

	//----------------------------------------------------------------------------------------
	//����: FindFirstCharNotOf
	//����: �ӵ�ǰ���ַ����ĸ���ƫ��nOffset��ʼ���ҵ�һ������szToFind���ַ��Ŀ�ʼ����
	//����: 
	//	@param	pszToFindָ�����ַ�
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����0������ͷ��ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	FindFirstCharNotOf(TCHAR szToFind, uint16 nOffset = 0);

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
	int	FindFirstCharNotOf(const TCHAR* pszToFind, uint16 nOffset = 0 );

	//----------------------------------------------------------------------------------------
	//����: FindLastCharOf
	//����: �ӵ�ǰ���ַ���β���ĸ���ƫ��nOffset��ʼ������ҵ�һ�����ֵ�chToFind�ַ�
	//����: 
	//	@param	chToFind	��Ҫ���ҵ��ַ�
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����NPOS������β����ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	FindLastCharOf(TCHAR chToFind,  uint16 nOffset = NPOS);

	//----------------------------------------------------------------------------------------
	//����: FindLastCharOf
	//����: �ӵ�ǰ���ַ���β���ĸ���ƫ��nOffset��ʼ������ҵ�һ���������Ӵ�pszToFind�е��ַ�
	//	  ������
	//����: 
	//	@param	pszToFind	��Ҫ���ҵ����ַ���
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����NPOS������β����ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	FindLastCharOf(const TCHAR* pszToFind, uint16 nOffset = NPOS);

	//----------------------------------------------------------------------------------------
	//����: FindLastCharNotOf
	//����: �ӵ�ǰ���ַ�����β��ƫ��nOffsetλ�÷�����ҵ�һ������szToFind���ַ��Ŀ�ʼ����
	//����: 
	//	@param	pszToFindָ�����ַ�
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����NPOS������β����ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	FindLastCharNotOf(TCHAR szToFind, uint16 nOffset = NPOS);

	//----------------------------------------------------------------------------------------
	//����: FindLastCharNotOf
	//����: �ӵ�ǰ���ַ���β��ƫ��ΪnOffsetλ�ÿ�ʼ���ҵ�һ������pszToFind���ַ��Ŀ�ʼ����������
	//	  FindLastCharNotOf("abcd")�����ǲ��ҵ�ǰ�ַ�����β����ʼ�ĵ�һ������a��
	//	 ����b������cҲ����d ���ַ��Ŀ�ʼ����
	//����: 
	//	@param	pszToFindָ�����ַ�
	//	@param	nOffset	���ҵ���ʼƫ�ƣ�Ĭ����NPOS������β����ʼ����
	//����: Ŀ���ַ������������û���ҵ����򷵻�-1��
	//----------------------------------------------------------------------------------------
	int	FindLastCharNotOf(const TCHAR* pszToFind, uint16 nOffset = NPOS );

	//----------------------------------------------------------------------------------------
	//����: SubStr
	//����: ��ȡ��ǰ�ַ���������Χ�ڵ����ַ���
	//����: 
	//	@param	nBegin	��ʼƫ��
	//	@param	nNumber		���ַ�������Ŀ
	//����: ��ǰ�ַ����д�nBegin��ʼ�����nNumber���ַ�
	//----------------------------------------------------------------------------------------
	String	SubStr(uint16 nBegin, uint16 nNumber) const;

	//----------------------------------------------------------------------------------------
	//����: Left
	//����: ��ȡ��ǰ�ַ�������ߵ�nLength���ַ�
	//----------------------------------------------------------------------------------------
	String	Left(uint16 nLength) const;

	//----------------------------------------------------------------------------------------
	//����: Mid
	//����: ��ȡ��ǰ�ַ�����ָ��λ��iPos��ʼ��nLength���ַ������nLengthΪ-1����Ĭ��Ϊ��
	//	  �ַ�����β
	//----------------------------------------------------------------------------------------
	String	Mid(uint16 iPos, int nLength = -1) const;

	//----------------------------------------------------------------------------------------
	//����: Right
	//����: ��ȡ��ǰ�ַ������ұ߿�ʼ��nLength���ַ���
	//----------------------------------------------------------------------------------------
	String	Right(uint16 nLength) const;

	//----------------------------------------------------------------------------------------
	//����: BeginWith
	//����: ��⵱ǰ�ַ����Ƿ����ض����ַ�����ʼ��
	//----------------------------------------------------------------------------------------
	BOOL	BeginWith(String	strBegin) const;

	//----------------------------------------------------------------------------------------
	//����: EndWith
	//����: ��⵱ǰ�ַ����Ƿ����ض����ַ���Ϊ��β��
	//----------------------------------------------------------------------------------------
	BOOL	EndWith(String	strEnd) const;

	//----------------------------------------------------------------------------------------
	//����: ToUpper��ToLower
	//����: ����ǰ�ַ����е��ַ�ȫ��ת��Ϊ��д����Сд
	//����: ת��֮����ַ���
	//----------------------------------------------------------------------------------------
	String	ToUpper();
	String	ToLower();

	//----------------------------------------------------------------------------------------
	//����: Reverse
	//����: ����ǰ�ַ�����ɷ���
	//����: ����ת��֮����ַ���
	//----------------------------------------------------------------------------------------
	String	Reverse();

	//----------------------------------------------------------------------------------------
	//����: Replace
	//����: ����ǰ�ַ����и�������nBegin��ʼ��nNumber���ַ��滻ΪpszReplacing
	//����: 
	//	@param	nBegin	��ʼƫ��
	//	@param	nNumber		���ַ�������Ŀ
	//	@param	pszReplacing	�滻����ַ���
	//����: ��ǰ�ַ����д�nBegin��ʼ�����nNumber���ַ�
	//----------------------------------------------------------------------------------------	
	String	Replace( int nBegin, int nNumber, const TCHAR* pszAfterReplcing);

	//----------------------------------------------------------------------------------------
	//����: Replace
	//����: ����ǰ�ַ����е����е��ַ���pszBefore�滻ΪpszAfter
	//����: 
	//	@param	pszBefore		�滻ǰ���ַ���
	//	@param	pszAfter		�滻����ַ���
	//����: �滻����ַ�������ǰ���ַ�������Ҳ�ᱻ����
	//----------------------------------------------------------------------------------------	
	String	Replace( const TCHAR* pszBefore, const TCHAR* pszAfter);

	//----------------------------------------------------------------------------------------
	//����: Format
	//����: ��ʽ�����ɵ�ǰ�ַ������ܹ�֧�ֵ�Ŀ���ַ�������Ϊ1024��
	//����: 
	//	@param	pstrFormat		��ʽ���ַ���
	//����: �滻����ַ�������ǰ���ַ�������Ҳ�ᱻ����
	//----------------------------------------------------------------------------------------	
	int __cdecl Format(LPCTSTR pstrFormat, ...);

	//----------------------------------------------------------------------------------------
	//����: Format
	//����: ��ʽ�����ɵ�ǰ�ַ��������ܹ�֧�ֵ�Ŀ���ַ�������Ϊ64��
	//����: 
	//	@param	pstrFormat		��ʽ���ַ���
	//����: �滻����ַ�������ǰ���ַ�������Ҳ�ᱻ����
	//----------------------------------------------------------------------------------------	
	int __cdecl SmallFormat(LPCTSTR pstrFormat, ...);

	//----------------------------------------------------------------------------------------
	//����: IsNumeric
	//����: ��ǰ�ַ����Ƿ�ȫ��������
	//����: ���ȫ�������֣��򷵻�true�����򷵻�false
	//----------------------------------------------------------------------------------------	
	bool	IsNumeric();

	//----------------------------------------------------------------------------------------
	//����: Split
	//����: �Ե�ǰ���ַ������ո������ַ����зָ�
	//����: 
	//	@param	chSplit		�ָ�ķ��ţ����Ҫ���� | ���зָ��chSplitΪ |
	//	@param	nNum		�ָ����ַ�������Ŀ
	//����: �滻����ַ�������ǰ���ַ�������Ҳ�ᱻ����
	//����: ���طָ����ַ�������ָ��
	//----------------------------------------------------------------------------------------	
	void Split(TCHAR chSplit, String* pString, uint16& nNum);

private:

#ifndef _UNICODE
	std::string	m_strString;
#else
	std::wstring m_strString;
#endif
};


#endif