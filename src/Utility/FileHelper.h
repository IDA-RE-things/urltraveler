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
///һЩͨ�ú궨��
class UTILITY_API FileHelper
{
public:

	/** �����ļ���HANDLE��ȡ�ļ�������
	@return BOOL.
	*/
	static wchar_t* GetFileNameFromHandler(HANDLE hFile);

	/// ��ȡ�ļ���С
	static DWORD GetFileSize(String sFilePath, DWORD* pdwHigh = NULL);

 	/// �ļ�/Ŀ¼�Ƿ����
	static BOOL IsExist(LPCTSTR szFilePath);

	/// Ŀ¼�Ƿ����
	static BOOL IsDirExist(LPCTSTR szFilePath);

	/// �ļ��Ƿ����
	static BOOL IsFileExist(LPCTSTR szFilePath);

	/// �ļ��Ƿ�ɶ�(���Ƿ񱻱�ĳ���ռ��)
	static BOOL IsFileReadable(LPCTSTR szFilePath);

	/// �ļ��Ƿ��д(���Ƿ񱻱�ĳ���ռ��)
	static BOOL IsFileWriteable(LPCTSTR szFilePath);

	static HICON   GetFileIcon(LPCTSTR szFilePath,BOOL bDir);

	// �޸�Ϊ������	 ModifyFileAttribute(sFile, 0, FILE_ATTRIBUTE_READONLY);
	static BOOL	ModifyFileAttribute(String sPath, DWORD dwAttributeAdd, DWORD dwAttributeRemove);

	/// ���ļ����ݶ���������buffer��ע������buffer��delete[] pBufOut
	static BOOL File2Buffer(String sFile, BYTE** ppBufOut, int& nBufLenOut);

	static BOOL CreateFile(String fileName);
};

#endif // !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)








