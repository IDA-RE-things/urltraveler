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

#if 1
//////////////////////////////////////////////////////////////////////////
///#if 1 ... #else 范围内的下面函数全部为Unicode版本的函数

/** 获取用户Application Data目录
采用windows api SHGetSpecialFolderPath来获取
*/
std::wstring PathHelper::GetAppDataDir(void)
{
	std::wstring strAppDataPath;

	///获取用户对应的 application data 目录,例:C:\Documents and Settings\username\Application Data
	WCHAR strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_APPDATA, FALSE);

	strAppDataPath = strPath;
	return strAppDataPath;
}

/** 获取用户MyDocument目录
采用windows api SHGetSpecialFolderPath来获取
*/
std::wstring PathHelper::GetMyDocumentDir(void)
{
	std::wstring strMyDocumentPath;

	///获取用户对应的 application data 目录,例:C:\Documents and Settings\wenhm\My Documents
	WCHAR strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath,  CSIDL_PERSONAL, FALSE);

	strMyDocumentPath = strPath;
	return strMyDocumentPath;
}

/** 获取所有用户的Application Data目录
采用windows api SHGetSpecialFolderPath来获取
*/
std::wstring PathHelper::GetCommonAppDataDir(void)
{
	std::wstring strAppDataPath;

	/**获取the file system directory containing application data for all users目录, 
	例:C:\Documents and Settings\All Users.WINDOWS\Application Data
	*/
	WCHAR strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_COMMON_APPDATA, FALSE);

	strAppDataPath = strPath;
	return strAppDataPath;
}

/** 获取用户主目录
采用Windows Api SHGetSpecialFolderPath来获取用户主目录
*/
std::wstring PathHelper::GetHomeDir(void)
{
	std::wstring strHomePath;

	///获取用户对应的 CSIDL_PROFILE 目录, 例:C:\Documents and Settings\username
	WCHAR strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPathW(NULL, strPath, CSIDL_PROFILE, FALSE);

	strHomePath = strPath;
	return strHomePath;
}


/** 获取用户临时目录
采用windows api SHGetSpecialFolderPath来获取用户临时目录
*/
std::wstring PathHelper::GetTempDir(void)
{
	std::wstring strTempPath;

	///获取用户对应的 CSIDL_PROFILE 目录, 例:C:\Documents and Settings\username
	WCHAR strPath[MAX_PATH] = {0};
	int iPathLen = ::GetTempPathW(MAX_PATH, strPath);

	strTempPath = strPath;
	return strTempPath;
}

bool PathHelper::CreateMultipleDirectory(const std::wstring& strPath)
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
	bool bSuccess = false;

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
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectoryW(iter->c_str(), NULL) ? true : false;    
	}

	return bSuccess;
}

bool PathHelper::RemoveMultipleDirectory(const std::wstring& strPath)
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
	bool bSuccess = false;

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
		//如果RemoveDirectory执行成功,返回true,否则返回false
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
		if ((fileinfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == false) 
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
			if (true != bResult) //不为要过滤掉的.和..目录则压入目录队列中
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
		if ((fileinfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == false) 
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

#else 
//////////////////////////////////////////////////////////////////////////
///#else  ... #endif 范围内的下面函数全部为非Unicode版本的函数

/** 获取用户Application Data目录
采用windows api SHGetSpecialFolderPath来获取
*/
std::string PathHelper::GetAppDataDir(void)
{
	std::string strAppDataPath;

	///获取用户对应的 application data 目录,例:C:\Documents and Settings\username\Application Data
	char strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPath(NULL, strPath, CSIDL_APPDATA, FALSE);

	strAppDataPath = strPath;

	return strAppDataPath;
}

/** 获取所有用户的Application Data目录
采用windows api SHGetSpecialFolderPath来获取
*/
std::string PathHelper::GetCommonAppDataDir(void)
{
	std::string strAppDataPath;
	
	/**获取the file system directory containing application data for all users目录, 
	例:C:\Documents and Settings\All Users.WINDOWS\Application Data
	*/
	char strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPath(NULL, strPath, CSIDL_COMMON_APPDATA, FALSE);

	strAppDataPath = strPath;

	return strAppDataPath;
}

/** 获取用户主目录
采用Windows Api SHGetSpecialFolderPath来获取用户主目录
*/
std::string PathHelper::GetHomeDir(void)
{
	std::string strHomePath;

	///获取用户对应的 CSIDL_PROFILE 目录, 例:C:\Documents and Settings\username
	char strPath[MAX_PATH] = {0};
	BOOL bResult = SHGetSpecialFolderPath(NULL, strPath, CSIDL_PROFILE, FALSE);

	strHomePath = strPath;

	return strHomePath;
}


/** 获取用户临时目录
采用windows api SHGetSpecialFolderPath来获取用户临时目录
*/
std::string PathHelper::GetTempDir(void)
{
	std::string strTempPath;

	///获取用户对应的 CSIDL_PROFILE 目录, 例:C:\Documents and Settings\username
	char strPath[MAX_PATH] = {0};
	int iPathLen = ::GetTempPath(MAX_PATH, strPath);

	strTempPath = strPath;

	return strTempPath;
}

bool PathHelper::CreateMultipleDirectory(const std::string& strPath)
{
	//存放要创建的目录字符串
	std::string strDir(strPath);

	//确保以'\'结尾以创建最后一个目录
	if (strDir[strDir.length() - 1] != ('\\'))
	{
		strDir += '\\';
	}

	//存放每一层目录字符串
	std::vector<std::string> vecPath;
	//一个临时变量,存放目录字符串
	std::string strTemp;
	//成功标志
	bool bSuccess = false;

	//遍历要创建的字符串
	for (UINT i=0; i<strDir.length(); ++i)
	{
		if (strDir[i] != '\\') 
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
	std::vector<std::string>::const_iterator iter;
	for (iter = vecPath.begin(); iter != vecPath.end(); iter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(iter->c_str(), NULL) ? true : false;    
	}

	return bSuccess;
}

bool PathHelper::RemoveMultipleDirectory(const std::string& strPath)
{
	//存放要删除的目录字符串
	std::string strDir(strPath);

	//确保以'\'结尾以删除最后一个目录
	if (strDir[strDir.length() - 1] != ('\\'))
	{
		strDir += '\\';
	}

	//存放每一层目录字符串
	std::vector<std::string> vecPath;
	//一个临时变量,存放目录字符串
	std::string strTemp;
	//成功标志
	bool bSuccess = false;

	//遍历要删除的字符串
	for (UINT i=0; i<strDir.length(); ++i)
	{
		if (strDir[i] != '\\') 
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
	std::vector<std::string>::const_reverse_iterator iter;
	for (iter = vecPath.rbegin(); iter != vecPath.rend(); iter++) 
	{
		//如果RemoveDirectory执行成功,返回true,否则返回false
		///bool bResult = iter->find('\\');
		std::string::size_type iFindPos = iter->find('\\');		
		if (std::string::npos != iFindPos)
		{
			bSuccess = RemoveDirectory(iter->c_str()) ? true : false;
		}    
	}

	return bSuccess;
}

bool PathHelper::GetAllSubDir(
		const std::string & strDir, 
		CStdStringVector &vecSubDir, 
		const std::string &strDirMask
		)
{
	bool bResult = false ;
	/*
	下面的代码为访问目录，和操作系统相关，目前为windows操作系统的相关代码
	*/

	std::string strFileName;
	//存放查找的当前目录
	std::string strCurDirName; 
	//存放查找中碰到的中间目录
	std::string strSubDirName; 
	//用来模拟递归用的存储中间遇到的子目录
	std::queue<std::string> recursiveDirQueue; 

	strCurDirName = strDir;
	strFileName = strDir;
	strFileName += "\\";
	strFileName += strDirMask;

	HANDLE hFindFile;
	WIN32_FIND_DATA fileinfo;	
	hFindFile = FindFirstFile(strFileName.c_str(), &fileinfo);

	while (INVALID_HANDLE_VALUE != hFindFile)
	{
		if ((fileinfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == false) 
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
			if (true != bResult) //不为要过滤掉的.和..目录则压入目录队列中
			{
				strSubDirName = strCurDirName;
				strSubDirName += "\\";
				strSubDirName += fileinfo.cFileName;
				///sprintf(strSubDirName,"%s\\%s" , strCurDirName, fileinfo.cFileName);
				recursiveDirQueue.push(strSubDirName);
				vecSubDir.push_back(strSubDirName);
				//printf ("The  subdir found is %s\n", strSubDirName.c_str());
			}
		}

		bResult = FindNextFile(hFindFile, &fileinfo);
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
				strFileName += "\\";
				strFileName += strDirMask;
				hFindFile = FindFirstFile(strFileName.c_str(), &fileinfo);
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
		  const std::string &strDir,  
		  CStdStringVector &vecFile,
		  const std::string &strFileMask
		  )
{
	bool bResult = false;
	vecFile.clear();
	/*
	下面的代码为访问目录，和操作系统相关，目前为windows操作系统的相关代码
	*/

	std::string strFileName;
	//存放查找的当前目录
	std::string strCurDirName; 
	//存放查找中碰到的中间目录
	std::string strSubDirName; 
	//用来模拟递归用的存储中间遇到的子目录
	std::queue<std::string> recursiveDirQueue; 

	strCurDirName = strDir;
	strFileName = strDir;
	strFileName += "\\";
	strFileName += strFileMask;

	HANDLE hFindFile;
	WIN32_FIND_DATA fileinfo;	
	hFindFile = FindFirstFile(strFileName.c_str(), &fileinfo);

	while (INVALID_HANDLE_VALUE != hFindFile)
	{
		if ((fileinfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == false) 
		{
			std::string strTmpFile = strCurDirName;
			strTmpFile += "\\";
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

		bResult = FindNextFile(hFindFile, &fileinfo);

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
	  const std::string & strDir, 
	  CStdStringVector &vecFile,
	  const std::string &strDirMask, 
	  const std::string &strFileMask 
	  )
{
	bool bResult = false;
	vecFile.clear();

	CStdStringVector vecSubDir;

	///获取所有子目录
	bResult = GetAllSubDir(strDir, vecSubDir, strDirMask);

	///将目录自己也压入进去以查找文件
	vecSubDir.push_back(strDir);

	if (true == bResult)
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

bool PathHelper::DirFilter(const std::string &strDirName)
{
	bool bReturn = false;

	if ( (strDirName == ".") || ( strDirName == ".."))
	{
		bReturn = true ;
	}

	return bReturn;
}


BOOL PathHelper::IsDirExist(const std::string &strDirPath)
{
	WIN32_FIND_DATA fd;
	BOOL bIsDirExist = FALSE;

	std::string strDirPathTemp = strDirPath;

	///确保删除掉后尾的'\'或是'/',不然FindFirstFile查找结尾带/的目录会失败的
	while (		strDirPathTemp.length() > 0 
		&&	(
		strDirPathTemp[strDirPathTemp.length() - 1] == '\\'
		||  strDirPathTemp[strDirPathTemp.length() - 1] == '/'
		)
		)
	{
		strDirPathTemp.erase(strDirPathTemp.length()-1);
	}

	HANDLE hFind = ::FindFirstFile(strDirPathTemp.c_str(), &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		//目录存在
		bIsDirExist = TRUE;
	}
	FindClose(hFind);

	return bIsDirExist;
}

BOOL PathHelper::IsFileExist(const std::string &strFilePath)
{
	WIN32_FIND_DATA fd;
	BOOL bIsFileExist = FALSE;

	std::string strDirPathTemp = strFilePath;

	///确保删除掉后尾的'\'或是'/',不然FindFirstFile查找结尾带/的目录会失败的
	while (		strDirPathTemp.length() > 0 
		&&	(
		strDirPathTemp[strDirPathTemp.length() - 1] == '\\'
		||  strDirPathTemp[strDirPathTemp.length() - 1] == '/'
		)
		)
	{
		strDirPathTemp.erase(strDirPathTemp.length()-1);
	}

	HANDLE hFind = ::FindFirstFile(strDirPathTemp.c_str(), &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		//文件存在
		bIsFileExist = TRUE;
	}
	FindClose(hFind);

	return bIsFileExist;
}

#endif

PathHelper::E_OperationVersion PathHelper::GetOperationVersion()
{
	const int iBufLen = 80;
	E_OperationVersion eOperationVersion = e_UnKnown;
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		{
			return eOperationVersion;
		}
	}

	switch (osvi.dwPlatformId)
	{
		// Test for the Windows NT product family.
	case VER_PLATFORM_WIN32_NT:

		if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
		{
			if( osvi.wProductType == VER_NT_WORKSTATION )
			{
				eOperationVersion = e_WinVista;
				///StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
			}
			else 
			{
				eOperationVersion = e_Win2008;
				///StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
			}
		}


		// Test for the specific product.
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			eOperationVersion = e_Win2003;
			printf ("Microsoft Windows Server 2003, ");
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			eOperationVersion = e_WinXP;
			///printf ("Microsoft Windows XP ");
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			eOperationVersion = e_Win2000;
			printf ("Microsoft Windows 2000 ");
		}

		if ( osvi.dwMajorVersion <= 4 )
		{
			eOperationVersion = e_WinNT;
			printf("Microsoft Windows NT ");
		}

		// Test for specific product on Windows NT 4.0 SP6 and later.
		if( bOsVersionInfoEx )
		{
			// Test for the workstation type.
			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				if( osvi.dwMajorVersion == 4 )
				{
					///printf ( "Workstation 4.0 " );
				}
				else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				{
					///printf ( "Home Edition " );
				}
				else
				{
					///printf ( "Professional " );
				}
			}

			// Test for the server type.
			else if ( osvi.wProductType == VER_NT_SERVER || 
				osvi.wProductType == VER_NT_DOMAIN_CONTROLLER )
			{
				if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2)
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						printf ( "Datacenter Edition " );
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						printf ( "Enterprise Edition " );
					else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
						printf ( "Web Edition " );
					else printf ( "Standard Edition " );
				}
				else if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0)
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						printf ( "Datacenter Server " );
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						printf ( "Advanced Server " );
					else printf ( "Server " );
				}
				else  // Windows NT 4.0 
				{
					if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						printf ("Server 4.0, Enterprise Edition " );
					else printf ( "Server 4.0 " );
				}
			}
		}
		// Test for specific product on Windows NT 4.0 SP5 and earlier
		else  
		{
			HKEY hKey;
			TCHAR szProductType[iBufLen];
			DWORD dwBufLen = iBufLen;
			LONG lRet;

			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
				0, KEY_QUERY_VALUE, &hKey );
			if( lRet != ERROR_SUCCESS )
			{
				return eOperationVersion;
			}

			lRet = RegQueryValueEx( hKey, _T("ProductType"), NULL, NULL,
				(LPBYTE) szProductType, &dwBufLen);
			if( (lRet != ERROR_SUCCESS) || (dwBufLen > iBufLen) )
			{
				return eOperationVersion;
			}

			RegCloseKey( hKey );

			if ( lstrcmpi( _T("WINNT"), szProductType) == 0 )
				printf( "Workstation " );
			if ( lstrcmpi( _T("LANMANNT"), szProductType) == 0 )
				printf( "Server " );
			if ( lstrcmpi( _T("SERVERNT"), szProductType) == 0 )
				printf( "Advanced Server " );
			printf( "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
		}

		// Display service pack (if any) and build number.

		if( osvi.dwMajorVersion == 4 && 
			lstrcmpi( osvi.szCSDVersion, _T("Service Pack 6") ) == 0 )
		{ 
			HKEY hKey;
			LONG lRet;

			// Test for SP6 versus SP6a.
			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),
				0, KEY_QUERY_VALUE, &hKey );
			if( lRet == ERROR_SUCCESS )
				printf( "Service Pack 6a (Build %d)\n", 
				osvi.dwBuildNumber & 0xFFFF );         
			else // Windows NT 4.0 prior to SP6a
			{
				/*printf( "%s (Build %d)\n",
					osvi.szCSDVersion,
					osvi.dwBuildNumber & 0xFFFF);
					*/
			}

			RegCloseKey( hKey );
		}
		else // not Windows NT 4.0 
		{
			/*
			printf( "%s (Build %d)\n",
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
			*/
		}

		break;

		// Test for the Windows Me/98/95.
	case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			eOperationVersion = e_Win95;
			printf ("Microsoft Windows 95 ");
			if (osvi.szCSDVersion[1]=='C' || osvi.szCSDVersion[1]=='B')
			{
				///printf("OSR2 " );
			}
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			eOperationVersion = e_Win98;
			///printf ("Microsoft Windows 98 ");
			if ( osvi.szCSDVersion[1] == 'A' )
			{
				///printf("SE " );
			}
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			eOperationVersion = e_WinME;
			///printf ("Microsoft Windows Millennium Edition\n");
		} 
		break;

	case VER_PLATFORM_WIN32s:
		{
			eOperationVersion = e_Win32;
			///printf ("Microsoft Win32s\n");
		}		
		break;
	}
	return eOperationVersion; 
}

/** 删除某个目录下所有信息
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

	///获取所有的*.*文件
	PathHelper::CStdStringVector vecFile;
	bResult = PathHelper::GetAllFile(strPath, vecFile, DIR_MASK_ALL, L"*.*");	

	///删除所有*.*文件
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
	///获取所有子目录
	CStdStringVector vecSubDir;
	bool bResult = GetAllSubDir(strPath, vecSubDir);

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

bool PathHelper::ForceRemoveDir(const std::wstring &strPath)
{
	///删除所有*.*文件
	bool bResult = RemoveDirAllFile(strPath);
	
	///删除所有子目录
	bResult = RemoveDirAllSubDir(strPath);
	
	///删除自己
	bResult = RemoveDirectoryW(strPath.c_str());
		
	return bResult; 
}


#if 0
/** 
**本代码参考sqlite3中shell.c中的find_home_dir()函数而来
** Return a pathname which is the user's home directory.  A
** 0 return indicates an error of some kind.  Space to hold the
** resulting string is obtained from malloc().  The calling
** function should free the result.
*/
std::string PathHelper::GetHomeDir2(void)
{
	std::string strHomePath;
	char *home_dir = NULL;

#if !defined(_WIN32) && !defined(WIN32) && !defined(__OS2__) && !defined(_WIN32_WCE)
	struct passwd *pwent;
	uid_t uid = getuid();
	if( (pwent=getpwuid(uid)) != NULL) {
		home_dir = pwent->pw_dir;
	}
#endif

#if defined(_WIN32_WCE)
	/* Windows CE (arm-wince-mingw32ce-gcc) does not provide getenv()
	*/
	home_dir = strdup("/");
#else

#if defined(_WIN32) || defined(WIN32) || defined(__OS2__)
	if (!home_dir) {
		home_dir = getenv("USERPROFILE");
	}
#endif

	if (!home_dir) {
		home_dir = getenv("HOME");
	}

#if defined(_WIN32) || defined(WIN32) || defined(__OS2__)
	if (!home_dir) {
		char *zDrive, *zPath;
		int n;
		zDrive = getenv("HOMEDRIVE");
		zPath = getenv("HOMEPATH");
		if( zDrive && zPath ){
			n = strlen(zDrive) + strlen(zPath) + 1;
			// 修改下面代码增加(char*)强制类型转换以使能编译过 [7/15/2008 温辉敏]
			home_dir = (char *)malloc( n );
			if( home_dir==0 ) return 0;
			// 修改下面代码用sprintf_s来替代qlite3中的sqlite3_snprintf函数 [7/15/2008 温辉敏]
			///sqlite3_snprintf(n, home_dir, "%s%s", zDrive, zPath);
			sprintf_s(home_dir, n, "%s%s", zDrive, zPath);
			return home_dir;
		}
		home_dir = "c:\\";
	}
#endif

#endif /* !_WIN32_WCE */

	if( home_dir ){
		int n = strlen(home_dir) + 1;
		// 修改下面代码增加(char*)强制类型转换以使能编译过 [7/15/2008 温辉敏]
		char *z = (char *)malloc( n );
		if( z ) memcpy(z, home_dir, n);
		home_dir = z;
	}
	
	// 设置std::string 类型的返回值 [7/15/2008 温辉敏]
	///return home_dir;
	strHomePath = home_dir;
	free(home_dir);

	return strHomePath;
}
#endif


const WCHAR *PathHelper::GetCurrentStrTime()
{
	static WCHAR strTime[64] = {0};
	SYSTEMTIME time;	
	GetLocalTime(&time);

	wsprintfW(strTime, L"%u/%u/%u  %u:%u:%u:%u", 
		time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	return strTime;
}

void PathHelper::StrSysTime2Date(const std::wstring &strFormatDate, SYSTEMTIME &time)
{
	///时间格式:2008-09-01
	std::wstring strDate = strFormatDate;
	int iYear = 0;
	int iMonth = 0;
	int iDay = 0;

	swscanf(strDate.c_str(), L"%4d", &iYear);

	strDate = strDate.substr(5);
	swscanf(strDate.c_str(), L"%2d", &iMonth);

	strDate = strDate.substr(3);
	swscanf(strDate.c_str(), L"%2d", &iDay);

	time.wYear = iYear;
	time.wMonth = iMonth;
	time.wDay = iDay;
}

void PathHelper::SysTime2Time(const SYSTEMTIME &sysTime,  time_t &time)
{
	struct tm tblock = {0};

	tblock.tm_year		= sysTime.wYear - 1900		;   
	tblock.tm_mon		= sysTime.wMonth	- 1	;   
	tblock.tm_mday		= sysTime.wDay		;   
	tblock.tm_hour		= sysTime.wHour		;   
	tblock.tm_min		= sysTime.wMinute		;   
	tblock.tm_sec		= sysTime.wSecond		;   
	tblock.tm_wday		= sysTime.wDayOfWeek	; 

	time = mktime(&tblock);	
}

void PathHelper::Time2SysTime(const time_t &time, SYSTEMTIME &sysTime)
{
	/** time_t有时间限制
	Before midnight, January 1, 1970.
	After 03:14:07, January 19, 2038, UTC (using _time32 and time32_t).
	After 23:59:59, December 31, 3000, UTC (using _time64 and __time64_t).
	*/
	memset(&sysTime,  0, sizeof(sysTime));
	struct tm *tblock = localtime(&time);   

	if (NULL != tblock)
	{
		sysTime.wYear		= tblock->tm_year + 1900;   
		sysTime.wMonth		= tblock->tm_mon + 1;   
		sysTime.wDay		= tblock->tm_mday;   
		sysTime.wHour		= tblock->tm_hour;   
		sysTime.wMinute		= tblock->tm_min;   
		sysTime.wSecond		= tblock->tm_sec;   
		sysTime.wDayOfWeek	= tblock->tm_wday;  
	}
}

const WCHAR *PathHelper::GetStrTime2(const SYSTEMTIME &time)
{
	static WCHAR strTime[64] = {0};
	memset(strTime, 0, sizeof(strTime));

	WCHAR strTmp[8] = {0};
	swprintf(strTmp, L"%d", time.wYear%100);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wYear%100);
	}
	wcscat(strTime, strTmp);

	swprintf(strTmp, L"%d", time.wMonth);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wMonth);
	}
	wcscat(strTime, strTmp);

	swprintf(strTmp, L"%d", time.wDay);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wDay);
	}
	wcscat(strTime, strTmp);

	swprintf(strTmp, L"%d", time.wHour);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wHour);
	}
	wcscat(strTime, strTmp);

	swprintf(strTmp, L"%d", time.wMinute);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wMinute);
	}
	wcscat(strTime, strTmp);

	return strTime;
}

const WCHAR *PathHelper::GetCurrentStrTime2()
{
	static WCHAR strTime[64] = {0};
	memset(strTime, 0, sizeof(strTime));

	SYSTEMTIME time;	
	GetLocalTime(&time);

	const WCHAR * strTimeTmp = GetStrTime2(time);
	wcsncpy_s(strTime, PARRAYSIZE(strTime), strTimeTmp, PARRAYSIZE(strTime)-1);

	return strTime;
}

const SYSTEMTIME &PathHelper::GetTimeFromStr2(const WCHAR *strTime)
{
	static SYSTEMTIME time = {0};	
	WCHAR strTmp[64] = {0};
	
	int iValue = 0;
	swscanf(strTime, L"%d", &iValue);

	time.wYear = iValue / 100000000;
	time.wYear += 2000;

	iValue %= 100000000;
	time.wMonth = iValue/ 1000000;

	iValue %= 1000000;
	time.wDay = iValue/ 10000;

	iValue %= 10000;
	time.wHour = iValue/ 100;

	iValue %= 100;
	time.wMinute = iValue;

	return time;
}

int PathHelper::GetWeekDayValue(const E_WeekDay&eWeekDay)
{
	struct ST_WeekDayAndValue
	{
		E_WeekDay m_eWeekDay;
		int m_iWeekDayValue;
	};

	struct ST_WeekDayAndValue st_aWeekDayAndValue[] = 
	{
		{e_Monday,		1},
		{e_Tuesday,		2},
		{e_Wednesday,	3},
		{e_Thursday,	4},
		{e_Friday,		5},
		{e_Saturday,	6},
		{e_Sunday,		7},
	};

	int iWeekDayAndValueArraySize = PARRAYSIZE(st_aWeekDayAndValue);

	int iWeekDayValue = 1;

	for (int i = 0; i < iWeekDayAndValueArraySize; i++)
	{
		struct ST_WeekDayAndValue &st_WeekDayAndValue = st_aWeekDayAndValue[i];
		if (eWeekDay == st_WeekDayAndValue.m_eWeekDay)
		{
			iWeekDayValue = st_WeekDayAndValue.m_iWeekDayValue;
			break;
		}
	}

	return iWeekDayValue;
}

void PathHelper::StringUtils_ltrim(std::wstring& str)
{
	int iPos = 0;
	while(iPos < str.size() && iswspace(str[iPos]))
	{
		iPos ++;
	}	

	if (iPos < str.size())
	{
		str.erase(0, iPos);
	}
	else
	{
		str.erase(str.begin(), str.end());
	}
}

void PathHelper::StringUtils_rtrim(std::wstring& str)
{
	int iPos = str.size() - 1;
	while(iPos >=0 && iswspace(str[iPos]))
	{
		iPos --;
	}	


	if(iPos >= 0)
	{
		str.erase(iPos+1, str.size() - iPos - 1);
	}
	else
	{
		str.erase(str.begin(), str.end());
	}
}

void PathHelper::StringUtils_trim(std::wstring& str)
{	
	StringUtils_ltrim(str);
	StringUtils_rtrim(str);
}