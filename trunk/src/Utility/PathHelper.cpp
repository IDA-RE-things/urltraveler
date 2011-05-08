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
///#if 1 ... #else ��Χ�ڵ����溯��ȫ��ΪUnicode�汾�ĺ���
/**��ȡ��ǰģ���·��*/
std::wstring PathHelper::GetModulePath()
{
	static wstring strModulePath = L"";
	if(strModulePath.empty())
	{
		WCHAR szModulePath[MAX_PATH  + 1];
		::GetModuleFileName(NULL, szModulePath, MAX_PATH);

		String	strPath = szModulePath;
		int index = strPath.ReverseFind((const WCHAR*)L'\\');
		if(index > 0)
		{
			strPath = strPath.Left(index + 1);
		}
		else
		{
			ASSERT(FALSE);
			strPath = L"\\";
		}

		strPath.Replace((const WCHAR*)L'\\',(const WCHAR*)L'/');
		strModulePath = strPath.GetData();
	}

	return strModulePath;
}


/** ��ȡ�û�Application DataĿ¼
����windows api SHGetSpecialFolderPath����ȡ
*/
std::wstring PathHelper::GetAppDataDir(void)
{
	std::wstring strAppDataPath;

	///��ȡ�û���Ӧ�� application data Ŀ¼,��:C:\Documents and Settings\username\Application Data
	WCHAR strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_APPDATA, FALSE);

	strAppDataPath = strPath;
	return strAppDataPath;
}

/** ��ȡ�û�MyDocumentĿ¼
����windows api SHGetSpecialFolderPath����ȡ
*/
std::wstring PathHelper::GetMyDocumentDir(void)
{
	std::wstring strMyDocumentPath;

	///��ȡ�û���Ӧ�� application data Ŀ¼,��:C:\Documents and Settings\wenhm\My Documents
	WCHAR strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath,  CSIDL_PERSONAL, FALSE);

	strMyDocumentPath = strPath;
	return strMyDocumentPath;
}

/** ��ȡ�����û���Application DataĿ¼
����windows api SHGetSpecialFolderPath����ȡ
*/
std::wstring PathHelper::GetCommonAppDataDir(void)
{
	std::wstring strAppDataPath;

	/**��ȡthe file system directory containing application data for all usersĿ¼, 
	��:C:\Documents and Settings\All Users.WINDOWS\Application Data
	*/
	WCHAR strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_COMMON_APPDATA, FALSE);

	strAppDataPath = strPath;
	return strAppDataPath;
}

/** ��ȡ�û���Ŀ¼
����Windows Api SHGetSpecialFolderPath����ȡ�û���Ŀ¼
*/
std::wstring PathHelper::GetHomeDir(void)
{
	std::wstring strHomePath;

	///��ȡ�û���Ӧ�� CSIDL_PROFILE Ŀ¼, ��:C:\Documents and Settings\username
	WCHAR strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_PROFILE, FALSE);

	strHomePath = strPath;
	return strHomePath;
}


/** ��ȡ�û���ʱĿ¼
����windows api SHGetSpecialFolderPath����ȡ�û���ʱĿ¼
*/
std::wstring PathHelper::GetTempDir(void)
{
	std::wstring strTempPath;

	///��ȡ�û���Ӧ�� CSIDL_PROFILE Ŀ¼, ��:C:\Documents and Settings\username
	WCHAR strPath[MAX_PATH] = {0};
	int iPathLen = ::GetTempPathW(MAX_PATH, strPath);

	strTempPath = strPath;
	return strTempPath;
}

bool PathHelper::CreateMultipleDirectory(const std::wstring& strPath)
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
	bool bSuccess = false;

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
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bSuccess = CreateDirectoryW(iter->c_str(), NULL) ? true : false;    
	}

	return bSuccess;
}

bool PathHelper::RemoveMultipleDirectory(const std::wstring& strPath)
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
	bool bSuccess = false;

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
		//���RemoveDirectoryִ�гɹ�,����true,���򷵻�false
		///bool bResult = iter->find('\\');
		std::wstring::size_type iFindPos = iter->find(L'\\');		
		if (std::wstring::npos != iFindPos)
		{
			bSuccess = RemoveDirectoryW(iter->c_str()) ? true : false;
		}    
	}

	return bSuccess;
}

bool PathHelper::GetAllSubDir(
							const std::wstring & strDir, 
							CStdStringVector &vecSubDir, 
							const std::wstring &strDirMask
							)
{
	bool bResult = false ;
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
		if ((fileinfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == false) 
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
			if (true != bResult) //��ΪҪ���˵���.��..Ŀ¼��ѹ��Ŀ¼������
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
		if (false == bResult)
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

bool PathHelper::GetDirFile(
						  const std::wstring &strDir,  
						  CStdStringVector &vecFile,
						  const std::wstring &strFileMask
						  )
{
	bool bResult = false;
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
		if ((fileinfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == false) 
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

		if (false == bResult)
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

bool PathHelper::GetAllFile(
						  const std::wstring & strDir, 
						  CStdStringVector &vecFile,
						  const std::wstring &strDirMask, 
						  const std::wstring &strFileMask 
						  )
{
	bool bResult = false;
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

bool PathHelper::DirFilter(const std::wstring &strDirName)
{
	bool bReturn = false;

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
@return bool.
*/
bool PathHelper::RemoveDir(std::wstring & strPath, const bool &bForce)
{
	bool bResult = false;

	if (true == bForce)
	{
		bResult = ForceRemoveDir(strPath);
	}
	else
	{
		bResult = RemoveDirectoryW(strPath.c_str());
	}

	return bResult;
}

bool PathHelper::RemoveDirAllFile(const std::wstring &strPath)
{
	bool bResult = false;

	///��ȡ���е�*.*�ļ�
	PathHelper::CStdStringVector vecFile;
	bResult = PathHelper::GetAllFile(strPath, vecFile, DIR_MASK_ALL, L"*.*");	

	///ɾ������*.*�ļ�
	PathHelper::CStdStringVector::iterator iter = vecFile.begin();
	while (iter != vecFile.end())
	{
		const std::wstring &strTmpFile = *iter;

		bResult = ::DeleteFileW(strTmpFile.c_str());

		iter ++;
	}

	return bResult;
}

bool PathHelper::RemoveDirAllSubDir(const std::wstring &strPath)
{
	///��ȡ������Ŀ¼
	CStdStringVector vecSubDir;
	bool bResult = GetAllSubDir(strPath, vecSubDir);

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

bool PathHelper::ForceRemoveDir(const std::wstring &strPath)
{
	///ɾ������*.*�ļ�
	bool bResult = RemoveDirAllFile(strPath);
	
	///ɾ��������Ŀ¼
	bResult = RemoveDirAllSubDir(strPath);
	
	///ɾ���Լ�
	bResult = RemoveDirectoryW(strPath.c_str());
		
	return bResult; 
}