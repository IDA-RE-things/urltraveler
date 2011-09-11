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
	wstring	m_strFileExtName;				//�ļ�����չ����
	wstring	m_strFilePrefix;				//�ļ����޳���չ����֮��Ĳ���
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
	
	/** ��std::wstring����·���ַ�����������
	1.��/�滻��\
	2.�����������ķָ���\\�滻��\
	@return void.
	*/
	void AdjustPath(std::wstring & strPath);

	/**
	 * Function ��ȡ����·���ĸ�·�����֣�
	 * @return ����c:\cc\aa\a.txt��c:\����
	 **/
	const wchar_t* GetRootPath();

	/**
	 * Function ��ȡ·�����֣�
	 * @return ��c:\1\1\a.txt��c:\1\1\����
	 **/
	const wchar_t* GetFilePath();

	/**
	 * Function ��ȡ·���е��ļ�����
	 * @return ����c:\1\1\a.txt������a.txt
	 **/
	const wchar_t* GetFileName();

	/**
	 * Function ��ȡ����·�����ļ������޳���չ����Ĳ��֣�
	 * @return ����c:\1\1\a.txt������a
	 **/
	const wchar_t* GetFileTitle();

	/**
	 * Function ��ȡ����·�����ļ����е���չ������
	 * @return	����c:\1\1\a.txt������txt
	 **/
	const wchar_t* GetFileExtName();

private:
	wstring	m_strFileFullPath;					//���ڼ�¼������ļ�����
	wstring	m_strFileName;						//���ڱ����ļ����ƣ�������׺���ƣ�������·������
												//����c:\a.txt��m_strFileName��Ϊa.txt����
	wstring	m_strFilePath;						//·�����֣���c:\a.txt��c:\����
	wstring	m_strRootPath;						//��·�����֣���c:\cc\aa\a.txt��c:\����
	wstring	m_strFileExtName;					//·�����ļ��ĺ�׺��
	wstring	m_strFileTitle;						//·�����ļ���ǰ׺
};


#endif
