#include <stdafx.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <shlobj.h>
#include <string>
#include <vector>
#include <queue>
#include "FileHelper.h"
#include "time.h"
#include <tchar.h>
#include "SndaBase.h"
#include "XString.h"

//////////////////////////////////////////////////////////////////////////
/** 根据文件的HANDLE获取文件的名称
@return BOOL.
*/
wchar_t* FileHelper::GetFileNameFromHandler(HANDLE hFile)
{
	return NULL;
}

/// 获取文件大小
DWORD FileHelper::GetFileSize(String sFilePath, DWORD* pdwHigh)
{
	DWORD dwRet = -1;

	HANDLE hFile = CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, 
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

/// 把文件内容读出，放入buffer，注意用完buffer后，delete[] pBufOut
BOOL FileHelper::File2Buffer(String sFile, BYTE** ppBufOut, int& nBufLenOut)
{
	nBufLenOut = FileHelper::GetFileSize(sFile);

	//读取更新的详细信息
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
