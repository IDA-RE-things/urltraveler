#include "UpdateThread.h"
#include "XUnzip.h"
#include "StringHelper.h"
#include "MiscHelper.h"
#include "FileHelper.h"
#include "PathHelper.h"
#include "atlconv.h"
#include <string>
#include "json/json.h"
#include "shellapi.h"
#include "UpdateThread.h"
#include "UpdateProcessWnd.h"

extern CUpdateProcessWnd* pUpdateExeWnd;

void CUpdateThread::OnThreadEntry()
{

}

int CUpdateThread::Run()
{
	// ��ֹUrlTraveler����
	BOOL bRet = MiscHelper::KillProcess(L"UrlTravelerd.exe", L"EverFav");
	while(MiscHelper::ExistProcess(L"UrlTravelerd.exe", L"EverFav"))
	{
		Sleep(100);
	}

	Sleep(2000);

   	wstring strUpdatePackage = MiscHelper::GetUpdatePath();
	strUpdatePackage += L"\\update_copy.zip";

	int nRet = UnzipPackage((wchar_t*)strUpdatePackage.c_str());
	if( nRet == -1)
	{
		pUpdateExeWnd->SendMessage(WM_UNPACKAGE,0,0);
		return -1;
	}
	pUpdateExeWnd->SendMessage(WM_UNPACKAGE,1,0);
	
	nRet = CopyUnPackageFile();
	if( nRet == -1)
	{
		pUpdateExeWnd->SendMessage(WM_COPYPACKAGE,0,0);
		return -1;
	}
	pUpdateExeWnd->SendMessage(WM_COPYPACKAGE,1,0);
	pUpdateExeWnd->SendMessage(WM_UPDATESUCCESS,0,0);

	return 0;
}

void CUpdateThread::OnThreadExit()
{
}

int CUpdateThread::UnzipPackage(wchar_t* pPackageName)
{
	if( pPackageName == NULL)
		return -1;

	// �����̣߳����и���
	HZIP hz = OpenZip((void*)pPackageName, 0, 2);

	unsigned int nZipItemNum;
	ZRESULT zResult = GetZipItemNum(hz, &nZipItemNum);
	if( zResult != ZR_OK)
		return -1;

	size_t i=0;
	for( ; i<nZipItemNum; i++)
	{
		ZIPENTRY ze; 
		zResult = GetZipItemA(hz, i, &ze);
		if( zResult != ZR_OK)
			continue;

		USES_CONVERSION;
		wchar_t* pName = wcsdup(A2W(ze.name));

		wchar_t szName[MAX_PATH];
		memset(szName, 0x0, MAX_PATH);
		swprintf(szName, L"%s%s", MiscHelper::GetUnpackagePath(),pName);
		zResult = UnzipItem(hz, i, (void*)szName, wcslen(szName),ZIP_FILENAME);

		// ��ѹʧ��
		if( zResult != ZR_OK)
			break;

		FileHelper::ModifyFileAttribute(szName, 0, FILE_ATTRIBUTE_READONLY);
	}
	CloseZip(hz);

	// ����ʧ��
	if( i < nZipItemNum)
	{
		::MessageBox(NULL, L"��װ����ѹʧ�ܣ��޷���ȷ���и���", L"������ʾ", MB_OK);
		return -1;
	}

	return 0;
}

int CUpdateThread::CopyUnPackageFile()
{
	// �ҵ�update.json
	wstring	strUpdateJson  = MiscHelper::GetUnpackagePath();
	strUpdateJson += L"update.json";
	if( PathHelper::IsFileExist(strUpdateJson.c_str()) == FALSE)
		return -1;

	char* pOutBuffer = NULL;
	int nBufLen = 0;
	BOOL bRet = FileHelper::File2Buffer(strUpdateJson.c_str(), (BYTE**)&pOutBuffer, nBufLen);
	if(bRet == FALSE)
		return -1;

	Json::Reader reader;
	Json::Value root;
	if(! reader.parse(pOutBuffer, root)) 
		return -1;

	Json::Value& fileList = root["filelist"];
	ASSERT(fileList.isArray() == TRUE);

	for( size_t i=0; i<fileList.size(); i++)
	{
		Json::Value& fileNode = fileList[i];

		string strAction = fileNode["action"].asString();
		string strFileName = fileNode["filename"].asString();
		string strMD5 = fileNode["md5"].asString();
		string strPath = fileNode["locatepath"].asString();

		// �����ļ�
		wstring wstrFileName = MiscHelper::GetUnpackagePath() + StringHelper::ANSIToUnicode(strFileName);
		if( PathHelper::IsFileExist(wstrFileName.c_str()) == FALSE)
		{
			::MessageBox(NULL, L"��װ����ѹʧ�ܣ���װ���ļ�����ʧ��", L"������ʾ", MB_OK);
			break;
		}

		if( strAction == "copy")
		{
			wstring wstrDestPath = PathHelper::GetModuleDir() + StringHelper::ANSIToUnicode(strPath) 
				+ StringHelper::ANSIToUnicode(strFileName);

			BOOL bRet = ::CopyFileW(wstrFileName.c_str(), wstrDestPath.c_str(), FALSE);
			if( bRet == FALSE)
			{
				int n = GetLastError();
				::MessageBox(NULL, L"��װ����ѹʧ�ܣ��޷����������ļ�", L"������ʾ", MB_OK);
				return -1;
			}
		}
		// ִ��exe�ļ�
		else if( strAction == "install")
		{
			ShellExecuteW(NULL, _T("open"), wstrFileName.c_str(), NULL, NULL, SW_SHOWNORMAL);
		}
	}

	return 0;
}
