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

//////////////////////////////////////////////////////////////////////////
///һЩͨ�ú궨��
class UTILITY_API FileHelper
{
public:

	/** �����ļ���HANDLE��ȡ�ļ�������
	@return BOOL.
	*/
	static wchar_t* GetFileNameFromHandler(HANDLE hFile);
};

#endif // !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)








