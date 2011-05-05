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

#ifndef LEFT_SHIFT
///左移宏定义
#define LEFT_SHIFT(uiData) \
	( (uiData) * 2)
#endif

//////////////////////////////////////////////////////////////////////////
/** 星期几
星期几 = [星期一 | 星期二| 星期三| 星期四| 星期五 | 星期六| 星期天]
以上几项可以进行或操作
*/
enum E_WeekDay
{
	e_Monday	= 1,
	e_Tuesday	= LEFT_SHIFT(e_Monday),
	e_Wednesday = LEFT_SHIFT(e_Tuesday),
	e_Thursday	= LEFT_SHIFT(e_Wednesday),
	e_Friday	= LEFT_SHIFT(e_Thursday),
	e_Saturday	= LEFT_SHIFT(e_Friday),
	e_Sunday	= LEFT_SHIFT(e_Saturday),
};

//////////////////////////////////////////////////////////////////////////
class UTILITY_API PathHelper
{
public:
	//////////////////////////////////////////////////////////////////////////
	///类内部使用类型定义
	

	///枚举类型:操作系统版本号
	enum E_OperationVersion
	{
		///Unknown Operation System
		e_UnKnown = -1,
		///Win32
		e_Win32 = 0,
		///Win95
		e_Win95,
		///Win98
		e_Win98,
		///WinME
		e_WinME,
		///WinNT
		e_WinNT,
		///Win2000
		e_Win2000,
		///WinXP
		e_WinXP,
		///Win2003
		e_Win2003,
		///Win Vista
		e_WinVista,
		///Win2008
		e_Win2008
	};

	//////////////////////////////////////////////////////////////////////////

	~PathHelper()
	{
	};
	
#if 1
	//////////////////////////////////////////////////////////////////////////
	///类内部使用类型定义
	///std::string vector
	typedef std::vector<std::wstring> CStdStringVector;

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

#else 
	//////////////////////////////////////////////////////////////////////////
	///类内部使用类型定义
	///std::string vector
	typedef std::vector<std::string> CStdStringVector;

	/** 获取用户Application Data目录
	采用windows api SHGetSpecialFolderPath来获取
	*/
	static std::string GetAppDataDir(void);

	/** 获取所有用户的Application Data目录
	采用windows api SHGetSpecialFolderPath来获取
	*/
	static std::string GetCommonAppDataDir(void);

	/** 获取用户主目录
	采用windows api SHGetSpecialFolderPath来获取用户主目录
	*/
	static std::string GetHomeDir(void);

	/** 获取用户临时目录
	采用windows api SHGetSpecialFolderPath来获取用户临时目录
	*/
	static std::string GetTempDir(void);
	
	/** 创建多级目录
	*/
	static bool CreateMultipleDirectory(const std::string& strPath);
	
	/** 删除多级目录
	*/
	static bool RemoveMultipleDirectory(const std::string& strPath);

#define DIR_MASK_ALL "*"
#define FILE_MASK_ALL "*"
	/** 获取strDir目录下的所有子目录
	将.和..两个系统缺省目录过滤掉了
	本函数中采用了模拟递归的方式来将递归算法非递归化
	@return bool,true:表示获取到了子目录，false表示没有获取到任何子目录
	*/
	static bool GetAllSubDir(
		const std::string & strDir, 
		CStdStringVector &subDirQueue,
		const std::string &strDirMask = DIR_MASK_ALL		///目录匹配符可以包含*,?等
		);

	/** 获取strDir目录下的所有文件，不包括子目录下文件
	将.和..两个系统缺省目录过滤掉了
	@return bool,true:表示获取到了子目录，false表示没有获取到任何子目录
	*/
	static bool GetDirFile(
		const std::string & strDir, 
		CStdStringVector &vecFile,
		const std::string &strFileMask = FILE_MASK_ALL		///文件匹配符可以包含*,?等
		);

	/** 获取strDir目录下的所有文件，包括子目录下文件
	@return bool,true:表示获取到了文件，false表示没有获取到任何文件
	*/
	static bool GetAllFile(
		const std::string & strDir, 
		CStdStringVector &vecFile,
		const std::string &strDirMask = DIR_MASK_ALL	,	///目录匹配符可以包含*,?等
		const std::string &strFileMask = FILE_MASK_ALL		///文件匹配符可以包含*,?等 
		);

	/** 判断是否是该过滤掉的目录
	将.和..两个系统缺省目录过滤掉了
	@return bool,true:是，false：否
	*/
	static bool DirFilter(const std::string &strDirName);

	/** 判断相应目录是否存在	
	@return BOOL,TRUE:存在,FALSE:不存在.
	*/
	static BOOL IsDirExist(const std::string &strDirPath);
#endif

	/** 获取操作系统版本
	返回操作系统的枚举类型E_OperationVersion
	本函数的代码改造自
	MSDN:
	http://msdn.microsoft.com/en-us/library/ms724429.aspx.
	ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.WIN32COM.v10.en/sysinfo/base/getting_the_system_version.htm
	@return E_OperationVersion.具体的操作系统枚举
	*/
	static E_OperationVersion GetOperationVersion();

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

#if 0
	/** 
	**本代码参考sqlite3中shell.c中的find_home_dir函数而来
	** Return a pathname which is the user's home directory.  A
	** 0 return indicates an error of some kind.  Space to hold the
	** resulting string is obtained from malloc().  The calling
	** function should free the result.
	*/
	static std::string GetHomeDir2(void);
#endif
	
	/** 转换字符串格式为SYSTEMTIME
	时间格式:2008-09-01
	@return void.
	*/
	static void StrSysTime2Date(const std::wstring &strFormatDate, SYSTEMTIME &time);
	
	/** 转换SYSTEMTIME为time_t
	时间格式:2008-09-01
	@return void.
	*/
	static void SysTime2Time(const SYSTEMTIME &sysTime,  time_t &time);
	
	/** 转换time_t为SYSTEMTIME
	时间格式:2008-09-01
	@return void.
	*/
	static void Time2SysTime(const time_t &time, SYSTEMTIME &sysTime);
	
	/** 获取当前时间的字符串形式
	@return const char*.返回当前时间的字符串形式
	*/
	static const WCHAR *GetCurrentStrTime();

	/** 获取当前时间的字符串形式
	显示的格式为 YYmmDDHHMM
	@return const char*.返回当前时间的字符串形式
	*/
	static const WCHAR *GetStrTime2(const SYSTEMTIME &time);

	/** 获取当前时间的字符串形式
	显示的格式为 YYmmDDHHMM
	@return const char*.返回当前时间的字符串形式
	*/
	static const WCHAR *GetCurrentStrTime2();

	/** 根据时间的字符串形式转换为相应时间
	字符串的格式为 YYmmDDHHMM(年年月月日日时时分分)
	@return SYSTEMTIME.返回字符串形式转换后的时间
	*/
	static const SYSTEMTIME &GetTimeFromStr2(const WCHAR *strTime);
	
	/** 获取枚举类型E_WeekDay对应的具体的值
	e_Monday为1
	e_Tuesday为2....
	@return int.
	*/
	static int GetWeekDayValue(const E_WeekDay&eWeekDay);

	/** 对std::wstring进行ltrim操作
	@return void.
	*/
	static void StringUtils_ltrim(std::wstring& str);

	/** 对std::wstring进行rtrim操作
	@return void.
	*/
	static void StringUtils_rtrim(std::wstring& str);

	/** 对std::wstring进行trim操作
	@return void.
	*/
	static void StringUtils_trim(std::wstring& str);

private:
	PathHelper()
	{
	};
};



#endif // !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)








