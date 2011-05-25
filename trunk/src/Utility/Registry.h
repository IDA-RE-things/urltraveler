#pragma once

#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "SndaBase.h"
#include "XString.h"
#include <string>

using namespace std;

//-----------------------------------------------------------------------------------------------------------   
// 函数: (注册表搜索(SearchInReg)回调函数类型定义)   
// 功能: SearchInReg 函数在搜索过程中会实时调用该函数, 以便调用者监控搜索进程状态.   
// 参数: [in] bool - 是否找到   
//       [in] const TCHAR* - 结果字段(用于显示)   
//       [in] const TCHAR* - 被匹配部分的字段   
//       [in] const HKEY& - 当前键的句柄, 用于在回调中对该键操作   
// 返回: void   
// 说明:    
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
	// 函数: SearchInReg   
	// 功能: 在注册表指定位置搜索某指定字符串.   
	// 参数: [in] LPONSEARCHPROC fnOnSearchProc - 搜索回调函数   
	//       [in] const TCHAR* ptszRegPath - 搜索路径(直接包含根键并支持缩写)   
	//       [in] const TCHAR* ptszSearchKey - 搜索关键字符串   
	//       [in] bool isFuzzy = true - 是否采用模糊方式   
	//       [in] bool isSearchKey = true - 是否搜索键   
	//       [in] bool isSearchValueName = true - 是否搜索值名   
	//       [in] bool isSearchValueData = true - 是否搜索值   
	// 返回: void   
	// 说明: 若要统计搜索数量, 请使用外部变量在回调函数中自行统计.   
	//-----------------------------------------------------------------------------------------------------------   
	void SearchInReg(void* p,LPONSEARCHPROC fnOnSearchProc, const TCHAR* ptszRegPath, 
		const TCHAR* ptszSearchKey, bool isFuzzy = true , bool isSearchKey = true , 
		bool isSearchValueName  = true , bool isSearchValueData  = true);


};// end of Registry class definition


#endif