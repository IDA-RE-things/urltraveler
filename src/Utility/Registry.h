#pragma once

#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "SndaBase.h"
#include "XString.h"
#include <string>

using namespace std;

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

};// end of Registry class definition


#endif