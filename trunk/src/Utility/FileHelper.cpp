#include <stdafx.h>
#include <string>
#include <vector>
#include <queue>
#include "FileHelper.h"
#include "time.h"
#include <tchar.h>
#include "SndaBase.h"
#include "XString.h"
#include "shlwapi.h"
#include "WinVer.h"
#include "XString.h"
#include "shellapi.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////////
/** �����ļ���HANDLE��ȡ�ļ�������
@return BOOL.
*/
wchar_t* FileHelper::GetFileNameFromHandler(HANDLE hFile)
{
	return NULL;
}

/// ��ȡ�ļ���С
DWORD FileHelper::GetFileSize(String sFilePath, DWORD* pdwHigh)
{
	DWORD dwRet = -1;

	HANDLE hFile = ::CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ASSERT(0);
		return 0;
	}
	else
	{
		dwRet = ::GetFileSize(hFile, pdwHigh);
		if (INVALID_FILE_SIZE == dwRet)
		{
			ASSERT(0);
		}

		CloseHandle(hFile);
	}

	return dwRet;

}
BOOL FileHelper::IsExist(LPCTSTR szFilePath)
{
	return PathFileExists(szFilePath);
}

BOOL FileHelper::IsDirExist(LPCTSTR szFilePath)
{
	return PathIsDirectory(szFilePath);
}

/// �ļ��Ƿ����
BOOL FileHelper::IsFileExist(LPCTSTR szFilePath)
{
	return IsExist(szFilePath) && !IsDirExist(szFilePath);
}

/// �ļ��Ƿ�ɶ�(���Ƿ񱻱�ĳ���ռ��)
BOOL FileHelper::IsFileReadable(LPCTSTR szFilePath)
{
	if (!IsFileExist(szFilePath))
	{
		ASSERT(0);
		return false;
	}

	HANDLE hFile = ::CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		CloseHandle(hFile);
		return true;
	}
}

/// �ļ��Ƿ��д(���Ƿ񱻱�ĳ���ռ��)
BOOL FileHelper::IsFileWriteable(LPCTSTR szFilePath)
{
	if (!IsFileExist(szFilePath))
	{
		ASSERT(0);
		return false;
	}

	HANDLE hFile = ::CreateFile(szFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		CloseHandle(hFile);
		return true;
	}
}

HICON FileHelper::GetFileIcon(LPCTSTR szFilePath,BOOL bDir)
{
	String strExt = _T("");
	if (! bDir)
	{
		String strTmp = szFilePath;
		int dotPos = strTmp.ReverseFind(L".");
		if (dotPos >= 0)
		{
			strExt = strTmp.Right(strTmp.GetLength() - dotPos);
			strExt = String(L"*") + strExt;
		}
	}
	SHFILEINFO fileInfo = {0};
	if (
		!SHGetFileInfo(strExt,
		0,
		&fileInfo,
		sizeof(fileInfo),
		SHGFI_LARGEICON | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES
		)
		)
		return NULL;

	return fileInfo.hIcon;
}


/// ���ļ����ݶ���������buffer��ע������buffer��delete[] pBufOut
BOOL FileHelper::File2Buffer(String sFile, BYTE** ppBufOut, int& nBufLenOut)
{
	nBufLenOut = FileHelper::GetFileSize(sFile);

	//��ȡ���µ���ϸ��Ϣ
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, &sa, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) 
	{ 
		*ppBufOut = new BYTE[nBufLenOut+1];

		DWORD dwRead=0;
		if (!::ReadFile(hFile, *ppBufOut, nBufLenOut, &dwRead, NULL))
		{
			ASSERT(0);
			dwRead = 0;

			::CloseHandle(hFile);
			return FALSE;
		}
		else
		{
			ASSERT(nBufLenOut==dwRead);
			(*ppBufOut)[nBufLenOut] = '\0';

			::CloseHandle(hFile);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL FileHelper::ModifyFileAttribute(String sPath, DWORD dwAttributeAdd, DWORD dwAttributeRemove)
{
	DWORD dwAttributeOld = GetFileAttributes(sPath);
	DWORD dwAttributeNew = (dwAttributeOld | dwAttributeAdd) & ~dwAttributeRemove;
	
	return SetFileAttributes(sPath, dwAttributeNew);
}


/*
���ܣ���ָ��·���´����ļ�
@fileName: Ҫ�������ļ���ȫ·��
@content: �ļ�����
@canBeEmptyFile: �ļ������Ƿ����Ϊ�գ�Ĭ��ֵΪFALSE
*/
BOOL FileHelper::CreateFile(String fileName)
{
	ofstream outFile;
	outFile.open(fileName, ios::out);
	outFile.close();
	return TRUE;
}