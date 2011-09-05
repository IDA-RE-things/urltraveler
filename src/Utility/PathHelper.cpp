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

/**获取当前模块所在的完整路径*/
const wchar_t* PathHelper::GetModulePath(HINSTANCE hInstance)
{
	WCHAR szModulePath[MAX_PATH  + 1];
	::GetModuleFileName(hInstance, szModulePath, MAX_PATH);

	return _wcsdup(szModulePath);
}

/** 获取用户Application Data目录
采用windows api SHGetSpecialFolderPath来获取
*/
const wchar_t* PathHelper::GetAppDataDir(void)
{
	///获取用户对应的 application data 目录,例:C:\Documents and Settings\username\Application Data
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_APPDATA, FALSE);

	return _wcsdup(szPath);
}

/** 获取用户Application Data目录
采用windows api SHGetSpecialFolderPath来获取
*/
const wchar_t* PathHelper::GetLocalAppDataDir(void)
{
	///获取用户对应的 application data 目录,例:C:\Documents and Settings\username\Application Data
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);

	return _wcsdup(szPath);
}

/** 获取用户MyDocument目录
采用windows api SHGetSpecialFolderPath来获取
*/
const wchar_t* PathHelper::GetMyDocumentDir(void)
{
	///获取用户对应的 application data 目录,例:C:\Documents and Settings\wenhm\My Documents
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath,  CSIDL_PERSONAL, FALSE);

	return _wcsdup(szPath);
}

/** 获取所有用户的Application Data目录
采用windows api SHGetSpecialFolderPath来获取
*/
const wchar_t* PathHelper::GetCommonAppDataDir(void)
{
	/**获取the file system directory containing application data for all users目录, 
	例:C:\Documents and Settings\All Users.WINDOWS\Application Data
	*/
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_COMMON_APPDATA, FALSE);

	return _wcsdup(szPath);
}

/** 获取用户主目录
采用Windows Api SHGetSpecialFolderPath来获取用户主目录
*/
const wchar_t* PathHelper::GetHomeDir(void)
{
	///获取用户对应的 CSIDL_PROFILE 目录, 例:C:\Documents and Settings\username
	WCHAR szPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, szPath, CSIDL_PROFILE, FALSE);

	return _wcsdup(szPath);
}

/** 获取用户临时目录
采用windows api SHGetSpecialFolderPath来获取用户临时目录
*/
const wchar_t* PathHelper::GetTempDir(void)
{
	///获取用户对应的 CSIDL_PROFILE 目录, 例:C:\Documents and Settings\username
	WCHAR szPath[MAX_PATH] = {0};
	int iPathLen = ::GetTempPathW(MAX_PATH, szPath);

	return _wcsdup(szPath);
}

BOOL PathHelper::CreateMultipleDirectory(const std::wstring& strPath)
{
	//存放要创建的目录字符串
	std::wstring strDir(strPath);

	//确保以'\'结尾以创建最后一个目录
	if (strDir[strDir.length() - 1] != (L'\\'))
	{
		strDir += L'\\';
	}

	//存放每一层目录字符串
	std::vector<std::wstring> vecPath;
	//一个临时变量,存放目录字符串
	std::wstring strTemp;
	//成功标志
	BOOL bSuccess = FALSE;

	//遍历要创建的字符串
	for (UINT i=0; i<strDir.length(); ++i)
	{
		if (strDir[i] != L'\\') 
		{	
			//如果当前字符不是'\\'
			strTemp += strDir[i];
		}
		else 
		{
			//如果当前字符是'\\',将当前层的字符串添加到数组中
			vecPath.push_back(strTemp);
			strTemp += strDir[i];
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<std::wstring>::const_iterator iter;
	for (iter = vecPath.begin(); iter != vecPath.end(); iter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回FALSE
		bSuccess = CreateDirectoryW(iter->c_str(), NULL) ? true : FALSE;    
	}

	return bSuccess;
}

BOOL PathHelper::RemoveMultipleDirectory(const std::wstring& strPath)
{
	//存放要删除的目录字符串
	std::wstring strDir(strPath);

	//确保以'\'结尾以删除最后一个目录
	if (strDir[strDir.length() - 1] != (L'\\'))
	{
		strDir += L'\\';
	}

	//存放每一层目录字符串
	std::vector<std::wstring> vecPath;
	//一个临时变量,存放目录字符串
	std::wstring strTemp;
	//成功标志
	BOOL bSuccess = FALSE;

	//遍历要删除的字符串
	for (UINT i=0; i<strDir.length(); ++i)
	{
		if (strDir[i] != L'\\') 
		{	
			//如果当前字符不是'\\'
			strTemp += strDir[i];
		}
		else 
		{
			//如果当前字符是'\\',将当前层的字符串添加到数组中
			vecPath.push_back(strTemp);
			strTemp += strDir[i];
		}
	}

	//遍历存放目录的数组,删除每层目录,从最深的目录开始删除,进行逆向访问
	std::vector<std::wstring>::const_reverse_iterator iter;
	for (iter = vecPath.rbegin(); iter != vecPath.rend(); iter++) 
	{
		//如果RemoveDirectory执行成功,返回true,否则返回FALSE
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
	下面的代码为访问目录，和操作系统相关，目前为windows操作系统的相关代码
	*/

	std::wstring strFileName;
	//存放查找的当前目录
	std::wstring strCurDirName; 
	//存放查找中碰到的中间目录
	std::wstring strSubDirName; 
	//用来模拟递归用的存储中间遇到的子目录
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
			//文件不为子目录
			/**
			push(fileinfo.cFileName);
			printf ("The  file found is %s\n", fileinfo.cFileName);
			*/
		}
		else  
		{
			//将查找中间碰到的子目录全部存入dirQueue队列中，以后还要对该目录进行查找
			bResult = DirFilter(fileinfo.cFileName);
			if (TRUE != bResult) //不为要过滤掉的.和..目录则压入目录队列中
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
	下面的代码为访问目录，和操作系统相关，目前为windows操作系统的相关代码
	*/

	std::wstring strFileName;
	//存放查找的当前目录
	std::wstring strCurDirName; 
	//存放查找中碰到的中间目录
	std::wstring strSubDirName; 
	//用来模拟递归用的存储中间遇到的子目录
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
			//文件不为子目录
			vecFile.push_back(strTmpFile);
			/**
			push(fileinfo.cFileName);
			printf ("The  file found is %s\n", fileinfo.cFileName);
			*/
		}
		else  
		{
#if 0
			//将查找中间碰到的子目录全部存入dirQueue队列中，以后还要对该目录进行查找
			bResult = DirFilter(fileinfo.cFileName);
			if (true != bResult) //不为要过滤掉的.和..目录则压入目录队列中
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

	///获取所有子目录
	bResult = GetAllSubDir(strDir, vecSubDir, strDirMask);

	///将目录自己也压入进去以查找文件
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

	///确保删除掉后尾的'\'或是'/',不然FindFirstFile查找结尾带/的目录会失败的
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
		//目录存在
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

	///确保删除掉后尾的'\'或是'/',不然FindFirstFile查找结尾带/的目录会失败的
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
		//文件存在
		bIsFileExist = TRUE;
	}
	FindClose(hFind);

	return bIsFileExist;
}

/** 删除某个目录下所有信息
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

	///获取所有的*.*文件
	PathHelper::CStdStringVector vecFile;
	bResult = PathHelper::GetAllFile(strPath, vecFile, DIR_MASK_ALL, L"*.*");	

	///删除所有*.*文件
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
	///获取所有子目录
	CStdStringVector vecSubDir;
	BOOL bResult = GetAllSubDir(strPath, vecSubDir);

	///删除所有的子目录,遍历存放目录的数组,删除每层目录,从最深的目录开始删除,进行逆向访问
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
	///删除所有*.*文件
	BOOL bResult = RemoveDirAllFile(strPath);
	
	///删除所有子目录
	bResult = RemoveDirAllSubDir(strPath);
	
	///删除自己
	bResult = RemoveDirectoryW(strPath.c_str());
		
	return bResult; 
}