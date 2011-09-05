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
#include "PathHelper.h"
#include "time.h"
#include <tchar.h>
#include "SndaBase.h"
#include "XString.h"

//////////////////////////////////////////////////////////////////////////
const wchar_t* PathHelper::GetModuleDir(HINSTANCE hInstance)
{
	WCHAR szModulePath[MAX_PATH  + 1];
	::GetModuleFileName(hInstance, szModulePath, MAX_PATH);

	String	strPath = szModulePath;
	int index = strPath.ReverseFind((const WCHAR*)L"\\");
	if(index > 0)
	{
		strPath = strPath.Left(index + 1);
	}
	else
	{
		ASSERT(FALSE);
		strPath += L"\\";
	}

	strPath.Replace((const WCHAR*)L"/",(const WCHAR*)L"\\");
	return _wcsdup(strPath.GetData());
}

/**��ȡ��ǰģ�����ڵ�����·��*/
const wchar_t* PathHelper::GetModulePath(HINSTANCE hInstance)
{
	WCHAR szModulePath[MAX_PATH  + 1];
	::GetModuleFileName(hInstance, szModulePath, MAX_PATH);

	return _wcsdup(szModulePath);
}

/** ��ȡ�û�Application DataĿ¼
����windows api SHGetSpecialFolderPath����ȡ
*/
const wchar_t* PathHelper::GetAppDataDir(void)
{
	///��ȡ�û���Ӧ�� application data Ŀ¼,��:C:\Documents and Settings\username\Application Data
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_APPDATA, FALSE);

	return _wcsdup(szPath);
}

/** ��ȡ�û�Application DataĿ¼
����windows api SHGetSpecialFolderPath����ȡ
*/
const wchar_t* PathHelper::GetLocalAppDataDir(void)
{
	///��ȡ�û���Ӧ�� application data Ŀ¼,��:C:\Documents and Settings\username\Application Data
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);

	return _wcsdup(szPath);
}

/** ��ȡ�û�MyDocumentĿ¼
����windows api SHGetSpecialFolderPath����ȡ
*/
const wchar_t* PathHelper::GetMyDocumentDir(void)
{
	///��ȡ�û���Ӧ�� application data Ŀ¼,��:C:\Documents and Settings\wenhm\My Documents
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath,  CSIDL_PERSONAL, FALSE);

	return _wcsdup(szPath);
}

/** ��ȡ�����û���Application DataĿ¼
����windows api SHGetSpecialFolderPath����ȡ
*/
const wchar_t* PathHelper::GetCommonAppDataDir(void)
{
	/**��ȡthe file system directory containing application data for all usersĿ¼, 
	��:C:\Documents and Settings\All Users.WINDOWS\Application Data
	*/
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_COMMON_APPDATA, FALSE);

	return _wcsdup(szPath);
}

/** ��ȡ�û���Ŀ¼
����Windows Api SHGetSpecialFolderPath����ȡ�û���Ŀ¼
*/
const wchar_t* PathHelper::GetHomeDir(void)
{
	///��ȡ�û���Ӧ�� CSIDL_PROFILE Ŀ¼, ��:C:\Documents and Settings\username
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_PROFILE, FALSE);

	return _wcsdup(szPath);
}

/** ��ȡ�û���ʱĿ¼
����windows api SHGetSpecialFolderPath����ȡ�û���ʱĿ¼
*/
const wchar_t* PathHelper::GetTempDir(void)
{
	///��ȡ�û���Ӧ�� CSIDL_PROFILE Ŀ¼, ��:C:\Documents and Settings\username
	WCHAR szPath[MAX_PATH] = {0};
	int iPathLen = ::GetTempPathW(MAX_PATH, szPath);

	return _wcsdup(szPath);
}

BOOL PathHelper::CreateMultipleDirectory(const std::wstring& strPath)
{
	//���Ҫ������Ŀ¼�ַ���
	std::wstring strDir(strPath);

	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir[strDir.length() - 1] != (L'\\'))
	{
		strDir += L'\\';
	}

	//���ÿһ��Ŀ¼�ַ���
	std::vector<std::wstring> vecPath;
	//һ����ʱ����,���Ŀ¼�ַ���
	std::wstring strTemp;
	//�ɹ���־
	BOOL bSuccess = FALSE;

	//����Ҫ�������ַ���
	for (UINT i=0; i<strDir.length(); ++i)
	{
		if (strDir[i] != L'\\') 
		{	
			//�����ǰ�ַ�����'\\'
			strTemp += strDir[i];
		}
		else 
		{
			//�����ǰ�ַ���'\\',����ǰ����ַ�����ӵ�������
			vecPath.push_back(strTemp);
			strTemp += strDir[i];
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<std::wstring>::const_iterator iter;
	for (iter = vecPath.begin(); iter != vecPath.end(); iter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�FALSE
		bSuccess = CreateDirectoryW(iter->c_str(), NULL) ? true : FALSE;    
	}

	return bSuccess;
}

BOOL PathHelper::RemoveMultipleDirectory(const std::wstring& strPath)
{
	//���Ҫɾ����Ŀ¼�ַ���
	std::wstring strDir(strPath);

	//ȷ����'\'��β��ɾ�����һ��Ŀ¼
	if (strDir[strDir.length() - 1] != (L'\\'))
	{
		strDir += L'\\';
	}

	//���ÿһ��Ŀ¼�ַ���
	std::vector<std::wstring> vecPath;
	//һ����ʱ����,���Ŀ¼�ַ���
	std::wstring strTemp;
	//�ɹ���־
	BOOL bSuccess = FALSE;

	//����Ҫɾ�����ַ���
	for (UINT i=0; i<strDir.length(); ++i)
	{
		if (strDir[i] != L'\\') 
		{	
			//�����ǰ�ַ�����'\\'
			strTemp += strDir[i];
		}
		else 
		{
			//�����ǰ�ַ���'\\',����ǰ����ַ�����ӵ�������
			vecPath.push_back(strTemp);
			strTemp += strDir[i];
		}
	}

	//�������Ŀ¼������,ɾ��ÿ��Ŀ¼,�������Ŀ¼��ʼɾ��,�����������
	std::vector<std::wstring>::const_reverse_iterator iter;
	for (iter = vecPath.rbegin(); iter != vecPath.rend(); iter++) 
	{
		//���RemoveDirectoryִ�гɹ�,����true,���򷵻�FALSE
		///BOOL bResult = iter->find('\\');
		std::wstring::size_type iFindPos = iter->find(L'\\');		
		if (std::wstring::npos != iFindPos)
		{
			bSuccess = RemoveDirectoryW(iter->c_str()) ? true : FALSE;
		}    
	}

	return bSuccess;
}

BOOL PathHelper::GetAllSubDir(
							const std::wstring & strDir, 
							CStdStringVector &vecSubDir, 
							const std::wstring &strDirMask
							)
{
	BOOL bResult = FALSE ;
	/*
	����Ĵ���Ϊ����Ŀ¼���Ͳ���ϵͳ��أ�ĿǰΪwindows����ϵͳ����ش���
	*/

	std::wstring strFileName;
	//��Ų��ҵĵ�ǰĿ¼
	std::wstring strCurDirName; 
	//��Ų������������м�Ŀ¼
	std::wstring strSubDirName; 
	//����ģ��ݹ��õĴ洢�м���������Ŀ¼
	std::queue<std::wstring> recursiveDirQueue; 

	strCurDirName = strDir;
	strFileName = strDir;
	strFileName += L"\\";
	strFileName += strDirMask;

	HANDLE hFindFile;
	WIN32_FIND_DATAW fileinfo;	
	hFindFile = FindFirstFileW(strFileName.c_str(), &fileinfo);

	while (INVALID_HANDLE_VALUE != hFindFile)
	{
		if ((fileinfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == FALSE) 
		{
			//�ļ���Ϊ��Ŀ¼
			/**
			push(fileinfo.cFileName);
			printf ("The  file found is %s\n", fileinfo.cFileName);
			*/
		}
		else  
		{
			//�������м���������Ŀ¼ȫ������dirQueue�����У��Ժ�Ҫ�Ը�Ŀ¼���в���
			bResult = DirFilter(fileinfo.cFileName);
			if (TRUE != bResult) //��ΪҪ���˵���.��..Ŀ¼��ѹ��Ŀ¼������
			{
				strSubDirName = strCurDirName;
				strSubDirName += L"\\";
				strSubDirName += fileinfo.cFileName;
				///sprintf(strSubDirName,"%s\\%s" , strCurDirName, fileinfo.cFileName);
				recursiveDirQueue.push(strSubDirName);
				vecSubDir.push_back(strSubDirName);
				//printf ("The  subdir found is %s\n", strSubDirName.c_str());
			}
		}

		bResult = FindNextFileW(hFindFile, &fileinfo);
		if (FALSE == bResult)
		{
			/*
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
			printf("No more %s files.", m_StrFileName);
			} 
			else 
			{ 
			printf("Couldn't find next file."); 
			}
			*/

			FindClose(hFindFile);

			if (recursiveDirQueue.size() > 0)
			{
				strCurDirName  = recursiveDirQueue.front( );
				recursiveDirQueue.pop();

				//sprintf(strFileName, "%s\\*", strCurDirName);
				strFileName = strCurDirName;
				strFileName += L"\\";
				strFileName += strDirMask;
				hFindFile = FindFirstFileW(strFileName.c_str(), &fileinfo);
			}
			else
			{
				break;
			}			
		}
	}

	bResult = (vecSubDir.size() > 0) ;
	return bResult;
}

BOOL PathHelper::GetDirFile(
						  const std::wstring &strDir,  
						  CStdStringVector &vecFile,
						  const std::wstring &strFileMask
						  )
{
	BOOL bResult = FALSE;
	vecFile.clear();
	/*
	����Ĵ���Ϊ����Ŀ¼���Ͳ���ϵͳ��أ�ĿǰΪwindows����ϵͳ����ش���
	*/

	std::wstring strFileName;
	//��Ų��ҵĵ�ǰĿ¼
	std::wstring strCurDirName; 
	//��Ų������������м�Ŀ¼
	std::wstring strSubDirName; 
	//����ģ��ݹ��õĴ洢�м���������Ŀ¼
	std::queue<std::wstring> recursiveDirQueue; 

	strCurDirName = strDir;
	strFileName = strDir;
	strFileName += L"\\";
	strFileName += strFileMask;

	HANDLE hFindFile;
	WIN32_FIND_DATAW fileinfo;	
	hFindFile = FindFirstFileW(strFileName.c_str(), &fileinfo);

	while (INVALID_HANDLE_VALUE != hFindFile)
	{
		if ((fileinfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == FALSE) 
		{
			std::wstring strTmpFile = strCurDirName;
			strTmpFile += L"\\";
			strTmpFile += fileinfo.cFileName;
			//�ļ���Ϊ��Ŀ¼
			vecFile.push_back(strTmpFile);
			/**
			push(fileinfo.cFileName);
			printf ("The  file found is %s\n", fileinfo.cFileName);
			*/
		}
		else  
		{
#if 0
			//�������м���������Ŀ¼ȫ������dirQueue�����У��Ժ�Ҫ�Ը�Ŀ¼���в���
			bResult = DirFilter(fileinfo.cFileName);
			if (true != bResult) //��ΪҪ���˵���.��..Ŀ¼��ѹ��Ŀ¼������
			{
				strSubDirName += strCurDirName;
				strSubDirName += "\\";
				strSubDirName += fileinfo.cFileName;
				///sprintf(strSubDirName,"%s\\%s" , strCurDirName, fileinfo.cFileName);
				recursiveDirQueue.push(strSubDirName);
				vecSubDir.push_back(strSubDirName);
				printf ("The  subdir found is %s\n", strSubDirName.c_str());
			}
#endif
		}

		bResult = FindNextFileW(hFindFile, &fileinfo);

		if (FALSE == bResult)
		{
			/*
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
			printf("No more %s files.", m_StrFileName);
			} 
			else 
			{ 
			printf("Couldn't find next file."); 
			}
			*/

			FindClose(hFindFile);
			break;
		}
	}

	bResult = (vecFile.size() > 0) ;
	return bResult;
}

BOOL PathHelper::GetAllFile(
						  const std::wstring & strDir, 
						  CStdStringVector &vecFile,
						  const std::wstring &strDirMask, 
						  const std::wstring &strFileMask 
						  )
{
	BOOL bResult = FALSE;
	vecFile.clear();

	CStdStringVector vecSubDir;

	///��ȡ������Ŀ¼
	bResult = GetAllSubDir(strDir, vecSubDir, strDirMask);

	///��Ŀ¼�Լ�Ҳѹ���ȥ�Բ����ļ�
	vecSubDir.push_back(strDir);

	if (vecSubDir.size() > 0)
	{
		CStdStringVector::iterator iter = vecSubDir.begin();
		while (iter != vecSubDir.end())
		{
			CStdStringVector vecTmpFile;
			GetDirFile(*iter, vecTmpFile, strFileMask);

			vecFile.insert(vecFile.end(), vecTmpFile.begin(), vecTmpFile.end());

			iter ++;
		}
	}

	bResult = (vecFile.size() > 0) ;
	return bResult;
}

BOOL PathHelper::DirFilter(const std::wstring &strDirName)
{
	BOOL bReturn = FALSE;

	if ( (strDirName == L".") || ( strDirName == L".."))
	{
		bReturn = true ;
	}

	return bReturn;
}

BOOL PathHelper::IsDirExist(const std::wstring &strDirPath)
{
	WIN32_FIND_DATAW fd;
	BOOL bIsDirExist = FALSE;

	std::wstring strDirPathTemp = strDirPath;

	///ȷ��ɾ������β��'\'����'/',��ȻFindFirstFile���ҽ�β��/��Ŀ¼��ʧ�ܵ�
	while (		strDirPathTemp.length() > 0 
		&&	(
		strDirPathTemp[strDirPathTemp.length() - 1] == L'\\'
		||  strDirPathTemp[strDirPathTemp.length() - 1] == L'/'
		)
		)
	{
		strDirPathTemp.erase(strDirPathTemp.length()-1);
	}

	HANDLE hFind = ::FindFirstFileW(strDirPathTemp.c_str(), &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		//Ŀ¼����
		bIsDirExist = TRUE;
	}
	FindClose(hFind);

	return bIsDirExist;
}

BOOL PathHelper::IsFileExist(const std::wstring &strFilePath)
{
	WIN32_FIND_DATAW fd;
	BOOL bIsFileExist = FALSE;

	std::wstring strDirPathTemp = strFilePath;

	///ȷ��ɾ������β��'\'����'/',��ȻFindFirstFile���ҽ�β��/��Ŀ¼��ʧ�ܵ�
	while (		strDirPathTemp.length() > 0 
		&&	(
		strDirPathTemp[strDirPathTemp.length() - 1] == L'\\'
		||  strDirPathTemp[strDirPathTemp.length() - 1] == L'/'
		)
		)
	{
		strDirPathTemp.erase(strDirPathTemp.length()-1);
	}

	HANDLE hFind = ::FindFirstFileW(strDirPathTemp.c_str(), &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		//�ļ�����
		bIsFileExist = TRUE;
	}
	FindClose(hFind);

	return bIsFileExist;
}

/** ɾ��ĳ��Ŀ¼��������Ϣ
@return BOOL.
*/
BOOL PathHelper::RemoveDir(std::wstring & strPath, const BOOL &bForce)
{
	BOOL bResult = FALSE;

	if (TRUE == bForce)
	{
		bResult = ForceRemoveDir(strPath);
	}
	else
	{
		bResult = RemoveDirectoryW(strPath.c_str());
	}

	return bResult;
}

BOOL PathHelper::RemoveDirAllFile(const std::wstring &strPath)
{
	BOOL bResult = FALSE;

	///��ȡ���е�*.*�ļ�
	PathHelper::CStdStringVector vecFile;
	bResult = PathHelper::GetAllFile(strPath, vecFile, DIR_MASK_ALL, L"*.*");	

	///ɾ������*.*�ļ�
	PathHelper::CStdStringVector::iterator iter = vecFile.begin();
	while (iter != vecFile.end())
	{
		const std::wstring &strTmpFile = *iter;

		bResult = ::DeleteFileW(strTmpFile.c_str());
		int n = GetLastError();
		iter ++;
	}

	return bResult;
}

BOOL PathHelper::RemoveDirAllSubDir(const std::wstring &strPath)
{
	///��ȡ������Ŀ¼
	CStdStringVector vecSubDir;
	BOOL bResult = GetAllSubDir(strPath, vecSubDir);

	///ɾ�����е���Ŀ¼,�������Ŀ¼������,ɾ��ÿ��Ŀ¼,�������Ŀ¼��ʼɾ��,�����������
	if (vecSubDir.size() > 0)
	{
		CStdStringVector::const_reverse_iterator iter = vecSubDir.rbegin();
		while (iter != vecSubDir.rend())
		{
			bResult = RemoveDirectoryW(iter->c_str());

			iter ++;
		}
	}
	else
	{
		bResult = true;
	}

	return bResult;
}

BOOL PathHelper::ForceRemoveDir(const std::wstring &strPath)
{
	///ɾ������*.*�ļ�
	BOOL bResult = RemoveDirAllFile(strPath);
	
	///ɾ��������Ŀ¼
	bResult = RemoveDirAllSubDir(strPath);
	
	///ɾ���Լ�
	bResult = RemoveDirectoryW(strPath.c_str());
		
	return bResult; 
}