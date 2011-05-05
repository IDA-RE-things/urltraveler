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
///һЩͨ�ú궨��

#ifndef LEFT_SHIFT
///���ƺ궨��
#define LEFT_SHIFT(uiData) \
	( (uiData) * 2)
#endif

//////////////////////////////////////////////////////////////////////////
/** ���ڼ�
���ڼ� = [����һ | ���ڶ�| ������| ������| ������ | ������| ������]
���ϼ�����Խ��л����
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
	///���ڲ�ʹ�����Ͷ���
	

	///ö������:����ϵͳ�汾��
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
	///���ڲ�ʹ�����Ͷ���
	///std::string vector
	typedef std::vector<std::wstring> CStdStringVector;

	/** ��ȡ�û�Application DataĿ¼
	����windows api SHGetSpecialFolderPath����ȡ
	*/
	static std::wstring GetAppDataDir(void);

	/** ��ȡ�û�MyDocumentĿ¼
	����windows api SHGetSpecialFolderPath����ȡ
	*/
	static std::wstring GetMyDocumentDir(void);

	/** ��ȡ�����û���Application DataĿ¼
	����windows api SHGetSpecialFolderPath����ȡ
	*/
	static std::wstring GetCommonAppDataDir(void);

	/** ��ȡ�û���Ŀ¼
	����windows api SHGetSpecialFolderPath����ȡ�û���Ŀ¼
	*/
	static std::wstring GetHomeDir(void);

	/** ��ȡ�û���ʱĿ¼
	����windows api SHGetSpecialFolderPath����ȡ�û���ʱĿ¼
	*/
	static std::wstring GetTempDir(void);

	/** �����༶Ŀ¼
	*/
	static bool CreateMultipleDirectory(const std::wstring& strPath);

	/** ɾ���༶Ŀ¼
	*/
	static bool RemoveMultipleDirectory(const std::wstring& strPath);

#define DIR_MASK_ALL L"*"
#define FILE_MASK_ALL L"*"
	/** ��ȡstrDirĿ¼�µ�������Ŀ¼
	��.��..����ϵͳȱʡĿ¼���˵���
	�������в�����ģ��ݹ�ķ�ʽ�����ݹ��㷨�ǵݹ黯
	@return bool,true:��ʾ��ȡ������Ŀ¼��false��ʾû�л�ȡ���κ���Ŀ¼
	*/
	static bool GetAllSubDir(
		const std::wstring & strDir, 
		CStdStringVector &subDirQueue,
		const std::wstring &strDirMask = DIR_MASK_ALL		///Ŀ¼ƥ������԰���*,?��
		);

	/** ��ȡstrDirĿ¼�µ������ļ�����������Ŀ¼���ļ�
	��.��..����ϵͳȱʡĿ¼���˵���
	@return bool,true:��ʾ��ȡ������Ŀ¼��false��ʾû�л�ȡ���κ���Ŀ¼
	*/
	static bool GetDirFile(
		const std::wstring & strDir, 
		CStdStringVector &vecFile,
		const std::wstring &strFileMask = FILE_MASK_ALL		///�ļ�ƥ������԰���*,?��
		);

	/** ��ȡstrDirĿ¼�µ������ļ���������Ŀ¼���ļ�
	@return bool,true:��ʾ��ȡ�����ļ���false��ʾû�л�ȡ���κ��ļ�
	*/
	static bool GetAllFile(
		const std::wstring & strDir, 
		CStdStringVector &vecFile,
		const std::wstring &strDirMask = DIR_MASK_ALL	,	///Ŀ¼ƥ������԰���*,?��
		const std::wstring &strFileMask = FILE_MASK_ALL		///�ļ�ƥ������԰���*,?�� 
		);

	/** �ж��Ƿ��Ǹù��˵���Ŀ¼
	��.��..����ϵͳȱʡĿ¼���˵���
	@return bool,true:�ǣ�false����
	*/
	static bool DirFilter(const std::wstring &strDirName);

	/** �ж���ӦĿ¼�Ƿ����	
	@return BOOL,TRUE:����,FALSE:������.
	*/
	static BOOL IsDirExist(const std::wstring &strDirPath);

	/** �ж���Ӧ�ļ��Ƿ����
	@return BOOL,TRUE:����,FALSE:������.
	*/
	static BOOL IsFileExist(const std::wstring &strFilePath);

#else 
	//////////////////////////////////////////////////////////////////////////
	///���ڲ�ʹ�����Ͷ���
	///std::string vector
	typedef std::vector<std::string> CStdStringVector;

	/** ��ȡ�û�Application DataĿ¼
	����windows api SHGetSpecialFolderPath����ȡ
	*/
	static std::string GetAppDataDir(void);

	/** ��ȡ�����û���Application DataĿ¼
	����windows api SHGetSpecialFolderPath����ȡ
	*/
	static std::string GetCommonAppDataDir(void);

	/** ��ȡ�û���Ŀ¼
	����windows api SHGetSpecialFolderPath����ȡ�û���Ŀ¼
	*/
	static std::string GetHomeDir(void);

	/** ��ȡ�û���ʱĿ¼
	����windows api SHGetSpecialFolderPath����ȡ�û���ʱĿ¼
	*/
	static std::string GetTempDir(void);
	
	/** �����༶Ŀ¼
	*/
	static bool CreateMultipleDirectory(const std::string& strPath);
	
	/** ɾ���༶Ŀ¼
	*/
	static bool RemoveMultipleDirectory(const std::string& strPath);

#define DIR_MASK_ALL "*"
#define FILE_MASK_ALL "*"
	/** ��ȡstrDirĿ¼�µ�������Ŀ¼
	��.��..����ϵͳȱʡĿ¼���˵���
	�������в�����ģ��ݹ�ķ�ʽ�����ݹ��㷨�ǵݹ黯
	@return bool,true:��ʾ��ȡ������Ŀ¼��false��ʾû�л�ȡ���κ���Ŀ¼
	*/
	static bool GetAllSubDir(
		const std::string & strDir, 
		CStdStringVector &subDirQueue,
		const std::string &strDirMask = DIR_MASK_ALL		///Ŀ¼ƥ������԰���*,?��
		);

	/** ��ȡstrDirĿ¼�µ������ļ�����������Ŀ¼���ļ�
	��.��..����ϵͳȱʡĿ¼���˵���
	@return bool,true:��ʾ��ȡ������Ŀ¼��false��ʾû�л�ȡ���κ���Ŀ¼
	*/
	static bool GetDirFile(
		const std::string & strDir, 
		CStdStringVector &vecFile,
		const std::string &strFileMask = FILE_MASK_ALL		///�ļ�ƥ������԰���*,?��
		);

	/** ��ȡstrDirĿ¼�µ������ļ���������Ŀ¼���ļ�
	@return bool,true:��ʾ��ȡ�����ļ���false��ʾû�л�ȡ���κ��ļ�
	*/
	static bool GetAllFile(
		const std::string & strDir, 
		CStdStringVector &vecFile,
		const std::string &strDirMask = DIR_MASK_ALL	,	///Ŀ¼ƥ������԰���*,?��
		const std::string &strFileMask = FILE_MASK_ALL		///�ļ�ƥ������԰���*,?�� 
		);

	/** �ж��Ƿ��Ǹù��˵���Ŀ¼
	��.��..����ϵͳȱʡĿ¼���˵���
	@return bool,true:�ǣ�false����
	*/
	static bool DirFilter(const std::string &strDirName);

	/** �ж���ӦĿ¼�Ƿ����	
	@return BOOL,TRUE:����,FALSE:������.
	*/
	static BOOL IsDirExist(const std::string &strDirPath);
#endif

	/** ��ȡ����ϵͳ�汾
	���ز���ϵͳ��ö������E_OperationVersion
	�������Ĵ��������
	MSDN:
	http://msdn.microsoft.com/en-us/library/ms724429.aspx.
	ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.WIN32COM.v10.en/sysinfo/base/getting_the_system_version.htm
	@return E_OperationVersion.����Ĳ���ϵͳö��
	*/
	static E_OperationVersion GetOperationVersion();

	/** ɾ��ĳ��Ŀ¼��������Ϣ
	@return bool.
	*/
	static bool RemoveDir(std::wstring & strPath, const bool &bForce = false);

	/** ǿ��ɾ��ĳ��Ŀ¼��������Ϣ,����Ŀ¼
	@return bool.
	*/
	static bool ForceRemoveDir(const std::wstring &strPath);

	/** ǿ��ɾ��ĳ��Ŀ¼�������ļ�����������Ŀ¼
	@return bool.
	*/
	static bool RemoveDirAllFile(const std::wstring &strPath);

	/** ǿ��ɾ��ĳ��Ŀ¼��������Ŀ¼
	@return bool.
	*/
	static bool RemoveDirAllSubDir(const std::wstring &strPath);

#if 0
	/** 
	**������ο�sqlite3��shell.c�е�find_home_dir��������
	** Return a pathname which is the user's home directory.  A
	** 0 return indicates an error of some kind.  Space to hold the
	** resulting string is obtained from malloc().  The calling
	** function should free the result.
	*/
	static std::string GetHomeDir2(void);
#endif
	
	/** ת���ַ�����ʽΪSYSTEMTIME
	ʱ���ʽ:2008-09-01
	@return void.
	*/
	static void StrSysTime2Date(const std::wstring &strFormatDate, SYSTEMTIME &time);
	
	/** ת��SYSTEMTIMEΪtime_t
	ʱ���ʽ:2008-09-01
	@return void.
	*/
	static void SysTime2Time(const SYSTEMTIME &sysTime,  time_t &time);
	
	/** ת��time_tΪSYSTEMTIME
	ʱ���ʽ:2008-09-01
	@return void.
	*/
	static void Time2SysTime(const time_t &time, SYSTEMTIME &sysTime);
	
	/** ��ȡ��ǰʱ����ַ�����ʽ
	@return const char*.���ص�ǰʱ����ַ�����ʽ
	*/
	static const WCHAR *GetCurrentStrTime();

	/** ��ȡ��ǰʱ����ַ�����ʽ
	��ʾ�ĸ�ʽΪ YYmmDDHHMM
	@return const char*.���ص�ǰʱ����ַ�����ʽ
	*/
	static const WCHAR *GetStrTime2(const SYSTEMTIME &time);

	/** ��ȡ��ǰʱ����ַ�����ʽ
	��ʾ�ĸ�ʽΪ YYmmDDHHMM
	@return const char*.���ص�ǰʱ����ַ�����ʽ
	*/
	static const WCHAR *GetCurrentStrTime2();

	/** ����ʱ����ַ�����ʽת��Ϊ��Ӧʱ��
	�ַ����ĸ�ʽΪ YYmmDDHHMM(������������ʱʱ�ַ�)
	@return SYSTEMTIME.�����ַ�����ʽת�����ʱ��
	*/
	static const SYSTEMTIME &GetTimeFromStr2(const WCHAR *strTime);
	
	/** ��ȡö������E_WeekDay��Ӧ�ľ����ֵ
	e_MondayΪ1
	e_TuesdayΪ2....
	@return int.
	*/
	static int GetWeekDayValue(const E_WeekDay&eWeekDay);

	/** ��std::wstring����ltrim����
	@return void.
	*/
	static void StringUtils_ltrim(std::wstring& str);

	/** ��std::wstring����rtrim����
	@return void.
	*/
	static void StringUtils_rtrim(std::wstring& str);

	/** ��std::wstring����trim����
	@return void.
	*/
	static void StringUtils_trim(std::wstring& str);

private:
	PathHelper()
	{
	};
};



#endif // !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)








