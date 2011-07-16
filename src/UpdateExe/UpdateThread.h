#pragma once

#include "ThreadObject_i.h"
#include <string>

using namespace std;

class CUpdateThread : public IThreadEvent
{
public:

	//! �����߳��麯��
	virtual void OnThreadEntry();

	virtual int Run();

	//! �߳��˳��麯��
	virtual void OnThreadExit();

	int UnzipPackage(wchar_t* pPackageName);	

	int CopyUnPackageFile();
};