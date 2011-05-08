#if !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)
#define AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SndaBase.h"
#include <Windows.h>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////
///一些通用宏定义
class UTILITY_API PathHelper
{
public:
	///类内部使用类型定义
	///std::string vector
	typedef std::vector<std::wstring> CStdStringVector;

	/**获取当前模块的路径*/
	static std::wstring GetModulePath();

	/** 获取用户Application Data目录
	采用windows api SHGetSpecialFolderPath来获取
	*/
	static std::wstring GetAppDataDir(void);

	/** 获取用户MyDocument目录
	采用windows api SHGetSpecialFolderPath来获取
	*/
	static std::wstring GetMyDocumentDir(void);

	/** 获取所有用户的Application Data目录
	采用windows api SHGetSpecialFolderPath来获取
	*/
	static std::wstring GetCommonAppDataDir(void);

	/** 获取用户主目录
	采用windows api SHGetSpecialFolderPath来获取用户主目录
	*/
	static std::wstring GetHomeDir(void);

	/** 获取用户临时目录
	采用windows api SHGetSpecialFolderPath来获取用户临时目录
	*/
	static std::wstring GetTempDir(void);

	/** 创建多级目录
	*/
	static bool CreateMultipleDirectory(const std::wstring& strPath);

	/** 删除多级目录
	*/
	static bool RemoveMultipleDirectory(const std::wstring& strPath);

#define DIR_MASK_ALL L"*"
#define FILE_MASK_ALL L"*"
	/** 获取strDir目录下的所有子目录
	将.和..两个系统缺省目录过滤掉了
	本函数中采用了模拟递归的方式来将递归算法非递归化
	@return bool,true:表示获取到了子目录，false表示没有获取到任何子目录
	*/
	static bool GetAllSubDir(
		const std::wstring & strDir, 
		CStdStringVector &subDirQueue,
		const std::wstring &strDirMask = DIR_MASK_ALL		///目录匹配符可以包含*,?等
		);

	/** 获取strDir目录下的所有文件，不包括子目录下文件
	将.和..两个系统缺省目录过滤掉了
	@return bool,true:表示获取到了子目录，false表示没有获取到任何子目录
	*/
	static bool GetDirFile(
		const std::wstring & strDir, 
		CStdStringVector &vecFile,
		const std::wstring &strFileMask = FILE_MASK_ALL		///文件匹配符可以包含*,?等
		);

	/** 获取strDir目录下的所有文件，包括子目录下文件
	@return bool,true:表示获取到了文件，false表示没有获取到任何文件
	*/
	static bool GetAllFile(
		const std::wstring & strDir, 
		CStdStringVector &vecFile,
		const std::wstring &strDirMask = DIR_MASK_ALL	,	///目录匹配符可以包含*,?等
		const std::wstring &strFileMask = FILE_MASK_ALL		///文件匹配符可以包含*,?等 
		);

	/** 判断是否是该过滤掉的目录
	将.和..两个系统缺省目录过滤掉了
	@return bool,true:是，false：否
	*/
	static bool DirFilter(const std::wstring &strDirName);

	/** 判断相应目录是否存在	
	@return BOOL,TRUE:存在,FALSE:不存在.
	*/
	static BOOL IsDirExist(const std::wstring &strDirPath);

	/** 判断相应文件是否存在
	@return BOOL,TRUE:存在,FALSE:不存在.
	*/
	static BOOL IsFileExist(const std::wstring &strFilePath);

	/** 删除某个目录下所有信息
	@return bool.
	*/
	static bool RemoveDir(std::wstring & strPath, const bool &bForce = false);

	/** 强制删除某个目录下所有信息,包括目录
	@return bool.
	*/
	static bool ForceRemoveDir(const std::wstring &strPath);

	/** 强制删除某个目录下所有文件，但不包括目录
	@return bool.
	*/
	static bool RemoveDirAllFile(const std::wstring &strPath);

	/** 强制删除某个目录下所有子目录
	@return bool.
	*/
	static bool RemoveDirAllSubDir(const std::wstring &strPath);
};

#endif // !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)








