#if !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_CDC33D76A15C__INCLUDED_)
#define AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_CDC33D76A15C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "SndaBase.h"
#include <Windows.h>
#include <string>
#include <vector>
#include "XString.h"

//////////////////////////////////////////////////////////////////////////
///一些通用宏定义
class UTILITY_API FileHelper
{
public:

	/** 根据文件的HANDLE获取文件的名称
	@return BOOL.
	*/
	static wchar_t* GetFileNameFromHandler(HANDLE hFile);

	/// 获取文件大小
	static DWORD GetFileSize(String sFilePath, DWORD* pdwHigh = NULL);

	/// 把文件内容读出，放入buffer，注意用完buffer后，delete[] pBufOut
	static BOOL File2Buffer(String sFile, BYTE** ppBufOut, int& nBufLenOut);
};

#endif // !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)








