// FilePath.cpp: implementation of the CFilePath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FilePath.h"
#include "assert.h"
#include "string.h"


CFileName::CFileName(wstring strFileName)
{	
	int nIndex = strFileName.find_last_of(L'.');
	if( nIndex != 0)
	{
		m_strFilePrefix = strFileName.substr(0, nIndex);
		m_strFileExtName = strFileName.substr( nIndex, strFileName.length());
	}
}


CFileName::~CFileName()
{
}

const wchar_t* CFileName::GetExtName()
{
	return wcsdup(m_strFileExtName.c_str());
}

const wchar_t* CFileName::GetPrefixName()
{
	return wcsdup(m_strFilePrefix.c_str());
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void string_replace(wstring & strBig, const wstring & strsrc, const wstring &strdst) 
{
	wstring::size_type pos = 0;
	wstring::size_type srclen = strsrc.size();
	wstring::size_type dstlen = strdst.size();
	while( (pos = strBig.find(strsrc, pos)) != wstring::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

CFilePath::CFilePath(const wstring strFullFilePath)
{
	wstring strPath = strFullFilePath;
	m_strFileFullPath = strPath;

	string_replace( strPath, L"/", L"\\");
	AdjustPath(strPath);

	// 根目录路径
	int nIndex = strPath.find_first_of(L'\\');
	m_strRootPath = strPath.substr(0, nIndex + 1);

	// 路径部分
	nIndex = strPath.find_last_of(L'\\');
	m_strFilePath = strPath.substr(0, nIndex + 1);

	// 文件名称
	m_strFileName = strPath.substr(nIndex + 1, strPath.length());

	// 文件名称前缀
	nIndex =  m_strFileName.find_last_of(L'.');
	m_strFileTitle = m_strFileName.substr( 0, nIndex);
	m_strFileExtName = m_strFileName.substr( nIndex + 1, m_strFileName.length());
}


CFilePath::~CFilePath()
{
}

void CFilePath::AdjustPath(std::wstring & strPath)
{
	std::wstring::size_type iPos = std::wstring::npos;

	string_replace(strPath, L"/", L"\\");
	do 
	{
		string_replace(strPath, L"\\\\", L"\\");
		iPos = strPath.find(L"\\\\");
	} while (std::wstring::npos != iPos);
}


const wchar_t* CFilePath::GetFileName()
{
	return wcsdup(m_strFileName.c_str());
}

const wchar_t* CFilePath::GetFilePath()
{
	return wcsdup(m_strFilePath.c_str());
}

const wchar_t* CFilePath::GetRootPath()
{
	return wcsdup(m_strRootPath.c_str());
}

const wchar_t* CFilePath::GetFileExtName()
{
	return wcsdup(m_strFileExtName.c_str());
}

const wchar_t* CFilePath::GetFileTitle()
{
	return wcsdup(m_strFileTitle.c_str());
}
