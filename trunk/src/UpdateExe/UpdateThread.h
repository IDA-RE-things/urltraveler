#pragma once

#include "ThreadObject_i.h"
#include <string>

using namespace std;

class CUpdateThread : public IThreadEvent
{
public:

	//! 进入线程虚函数
	virtual void OnThreadEntry();

	virtual int Run();

	//! 线程退出虚函数
	virtual void OnThreadExit();

	int UnzipPackage(wchar_t* pPackageName);	

	int CopyUnPackageFile();
};