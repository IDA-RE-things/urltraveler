#pragma once

#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "SndaBase.h"
#include "XString.h"
#include <string>

using namespace std;

//-----------------------------------------------------------------------------------------------------------   
// ����: (ע�������(SearchInReg)�ص��������Ͷ���)   
// ����: SearchInReg ���������������л�ʵʱ���øú���, �Ա�����߼����������״̬.   
// ����: [in] bool - �Ƿ��ҵ�   
//       [in] const TCHAR* - ����ֶ�(������ʾ)   
//       [in] const TCHAR* - ��ƥ�䲿�ֵ��ֶ�   
//       [in] const HKEY& - ��ǰ���ľ��, �����ڻص��жԸü�����   
// ����: void   
// ˵��:    
//-----------------------------------------------------------------------------------------------------------   
typedef void (CALLBACK* LPONSEARCHPROC)(void*,bool, const TCHAR*, const TCHAR*, const HKEY&);

class UTILITY_API Registry
{
public:
	Registry();
	~Registry();

	int m_nLastError;

	// Registry properties	
protected:
	HKEY m_hRootKey;
	BOOL m_bLazyWrite;
	String m_strCurrentPath;

public:
	inline BOOL PathIsValid() {
		return (m_strCurrentPath.GetLength() > 0); }

	inline String GetCurrentPath() {
		return m_strCurrentPath; }
	inline HKEY GetRootKey() {
		return m_hRootKey; }


	//Registry	methods
public:
	BOOL ClearKey();
	BOOL SetRootKey(HKEY hRootKey);
	BOOL CreateKey(String strKey);
	BOOL DeleteKey(String strKey);
	BOOL DeleteValue(String strName);
	int GetDataSize(String strValueName);
	DWORD GetDataType(String strValueName);
	int GetSubKeyCount();
	int GetValueCount();
	BOOL KeyExists(String strKey, HKEY hRootKey = NULL);
	BOOL SetKey(String strKey, BOOL bCanCreate);
	BOOL ValueExists(String strName);
	void RenameValue(String strOldName, String strNewName);

	// data reading functions
	double ReadFloat(String strName, double fDefault);
	String ReadString(String strName, String strDefault);
	int ReadInt(String strName, int nDefault);
	BOOL ReadBool(String strName, BOOL bDefault);
	COLORREF ReadColor(String strName, COLORREF rgbDefault);
	DWORD ReadDword(String strName, DWORD dwDefault);

	// data writing functions
	BOOL WriteBool(String strName, BOOL bValue);
	BOOL WriteString(String strName, String strValue);
	BOOL WriteFloat(String strName, double fValue);
	BOOL WriteInt(String strName, int nValue);
	BOOL WriteColor(String strName, COLORREF rgbValue);
	BOOL WriteDword(String strName, DWORD dwValue);

	//-----------------------------------------------------------------------------------------------------------   
	// ����: SearchInReg   
	// ����: ��ע���ָ��λ������ĳָ���ַ���.   
	// ����: [in] LPONSEARCHPROC fnOnSearchProc - �����ص�����   
	//       [in] const TCHAR* ptszRegPath - ����·��(ֱ�Ӱ���������֧����д)   
	//       [in] const TCHAR* ptszSearchKey - �����ؼ��ַ���   
	//       [in] bool isFuzzy = true - �Ƿ����ģ����ʽ   
	//       [in] bool isSearchKey = true - �Ƿ�������   
	//       [in] bool isSearchValueName = true - �Ƿ�����ֵ��   
	//       [in] bool isSearchValueData = true - �Ƿ�����ֵ   
	// ����: void   
	// ˵��: ��Ҫͳ����������, ��ʹ���ⲿ�����ڻص�����������ͳ��.   
	//-----------------------------------------------------------------------------------------------------------   
	void SearchInReg(void* p,LPONSEARCHPROC fnOnSearchProc, const TCHAR* ptszRegPath, 
		const TCHAR* ptszSearchKey, bool isFuzzy = true , bool isSearchKey = true , 
		bool isSearchValueName  = true , bool isSearchValueData  = true);


};// end of Registry class definition


#endif