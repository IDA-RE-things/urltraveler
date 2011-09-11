#ifndef __FILEPATH_H_
#define __FILEPATH_H_

#define MAX_PATH	1024

#include <string>
#include "SndaBase.h"


using namespace std;

/**************************************************************
 * ClassName:   CFileName
 * Author:		Zhang Z.Q
 * Using:		The class is used to operate a file name such as x.mp3, such
 *              as get file ext name, root path ect.
 * Verion:
 *		Created			2007,12,12
 **************************************************************/
class UTILITY_API CFileName
{
public:
	CFileName(wstring strFileName);
	~CFileName();

	const wchar_t* GetExtName();
	const wchar_t* GetPrefixName();

private:
	wstring	m_strFileName;
	wstring	m_strFileExtName;				//文件的扩展名称
	wstring	m_strFilePrefix;				//文件的剔除扩展名称之后的部分
};


/**************************************************************
 * ClassName:   CFilePath
 * Author:		Zhang Z.Q
 * Using:		The class is used to operate a file path, such
 *              as get file ext name, root path ect.
 * Verion:
 *		Created			2007,12,12
 **************************************************************/

class UTILITY_API CFilePath  
{
public:
	CFilePath(const wstring strFullFilePath);

	virtual ~CFilePath();
	
	/** 对std::wstring进行路径字符串调整操作
	1.将/替换成\
	2.将两个连续的分隔符\\替换成\
	@return void.
	*/
	void AdjustPath(std::wstring & strPath);

	/**
	 * Function 获取给定路径的根路径部分，
	 * @return 比如c:\cc\aa\a.txt的c:\部分
	 **/
	const wchar_t* GetRootPath();

	/**
	 * Function 获取路径部分，
	 * @return 即c:\1\1\a.txt的c:\1\1\部分
	 **/
	const wchar_t* GetFilePath();

	/**
	 * Function 获取路径中的文件名，
	 * @return 比如c:\1\1\a.txt，返回a.txt
	 **/
	const wchar_t* GetFileName();

	/**
	 * Function 获取给定路径中文件名的剔除扩展名后的部分，
	 * @return 比如c:\1\1\a.txt，返回a
	 **/
	const wchar_t* GetFileTitle();

	/**
	 * Function 获取给定路径中文件名中的扩展名部分
	 * @return	比如c:\1\1\a.txt，返回txt
	 **/
	const wchar_t* GetFileExtName();

private:
	wstring	m_strFileFullPath;					//用于记录保存的文件名称
	wstring	m_strFileName;						//用于保存文件名称，包括后缀名称，不包括路径部分
												//比如c:\a.txt，m_strFileName即为a.txt部分
	wstring	m_strFilePath;						//路径部分，即c:\a.txt的c:\部分
	wstring	m_strRootPath;						//跟路径部分，即c:\cc\aa\a.txt的c:\部分
	wstring	m_strFileExtName;					//路径中文件的后缀名
	wstring	m_strFileTitle;						//路径中文件的前缀
};


#endif
