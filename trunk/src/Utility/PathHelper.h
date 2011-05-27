#if !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)
#define AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_

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
class UTILITY_API PathHelper
{
public:
	///���ڲ�ʹ�����Ͷ���
	///std::string vector
	typedef std::vector<std::wstring> CStdStringVector;

	/**��ȡ��ǰģ�����ڵ�Ŀ¼������Ϊģ���hInstance��Ĭ��ΪNULL����ʾ��ȡ�������·��*/
	static std::wstring GetModuleDir(HINSTANCE hInstance = NULL);

	/**��ȡ��ǰģ�������·��������Ϊģ���hInstance��Ĭ��ΪNULL����ʾ��ȡ�������·��*/
	static std::wstring GetModulePath(HINSTANCE hInstance = NULL);

	/** ��ȡ�û�Application DataĿ¼
	����windows api SHGetSpecialFolderPath����ȡ
	*/
	static std::wstring GetAppDataDir(void);

	/** ��ȡ�û�����Application DataĿ¼
	����windows api SHGetSpecialFolderPath����ȡ
	*/
	static std::wstring GetLocalAppDataDir(void);

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
	static BOOL CreateMultipleDirectory(const std::wstring& strPath);

	/** ɾ���༶Ŀ¼
	*/
	static BOOL RemoveMultipleDirectory(const std::wstring& strPath);

#define DIR_MASK_ALL L"*"
#define FILE_MASK_ALL L"*"
	/** ��ȡstrDirĿ¼�µ�������Ŀ¼
	��.��..����ϵͳȱʡĿ¼���˵���
	�������в�����ģ��ݹ�ķ�ʽ�����ݹ��㷨�ǵݹ黯
	@return BOOL,true:��ʾ��ȡ������Ŀ¼��false��ʾû�л�ȡ���κ���Ŀ¼
	*/
	static BOOL GetAllSubDir(
		const std::wstring & strDir, 
		CStdStringVector &subDirQueue,
		const std::wstring &strDirMask = DIR_MASK_ALL		///Ŀ¼ƥ������԰���*,?��
		);

	/** ��ȡstrDirĿ¼�µ������ļ�����������Ŀ¼���ļ�
	��.��..����ϵͳȱʡĿ¼���˵���
	@return BOOL,true:��ʾ��ȡ������Ŀ¼��false��ʾû�л�ȡ���κ���Ŀ¼
	*/
	static BOOL GetDirFile(
		const std::wstring & strDir, 
		CStdStringVector &vecFile,
		const std::wstring &strFileMask = FILE_MASK_ALL		///�ļ�ƥ������԰���*,?��
		);

	/** ��ȡstrDirĿ¼�µ������ļ���������Ŀ¼���ļ�
	@return BOOL,true:��ʾ��ȡ�����ļ���false��ʾû�л�ȡ���κ��ļ�
	*/
	static BOOL GetAllFile(
		const std::wstring & strDir, 
		CStdStringVector &vecFile,
		const std::wstring &strDirMask = DIR_MASK_ALL	,	///Ŀ¼ƥ������԰���*,?��
		const std::wstring &strFileMask = FILE_MASK_ALL		///�ļ�ƥ������԰���*,?�� 
		);

	/** �ж��Ƿ��Ǹù��˵���Ŀ¼
	��.��..����ϵͳȱʡĿ¼���˵���
	@return BOOL,true:�ǣ�false����
	*/
	static BOOL DirFilter(const std::wstring &strDirName);

	/** �ж���ӦĿ¼�Ƿ����	
	@return BOOL,TRUE:����,FALSE:������.
	*/
	static BOOL IsDirExist(const std::wstring &strDirPath);

	/** �ж���Ӧ�ļ��Ƿ����
	@return BOOL,TRUE:����,FALSE:������.
	*/
	static BOOL IsFileExist(const std::wstring &strFilePath);

	/** ɾ��ĳ��Ŀ¼��������Ϣ
	@return BOOL.
	*/
	static BOOL RemoveDir(std::wstring & strPath, const BOOL &bForce = false);

	/** ǿ��ɾ��ĳ��Ŀ¼��������Ϣ,����Ŀ¼
	@return BOOL.
	*/
	static BOOL ForceRemoveDir(const std::wstring &strPath);

	/** ǿ��ɾ��ĳ��Ŀ¼�������ļ�����������Ŀ¼
	@return BOOL.
	*/
	static BOOL RemoveDirAllFile(const std::wstring &strPath);

	/** ǿ��ɾ��ĳ��Ŀ¼��������Ŀ¼
	@return BOOL.
	*/
	static BOOL RemoveDirAllSubDir(const std::wstring &strPath);
};

#endif // !defined(AFX_UTILITY_H__9A1932AD_2109_48B4_8FDF_C2C33D76A15C__INCLUDED_)








