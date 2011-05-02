#pragma once

#include "SndaBase.h"
#include <string>
#include <vector>
#include <map>
#include <xstring>

#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assert.hpp>

using namespace boost::assign;

// 当前需要加载的所有的模块列表

#ifdef _DEBUG
#define MODULE_NAME(X) L#X##L"d.dll"
#else
#define MODULE_NAME(X) L#X##L".dll"
#endif

struct ModuleList
{
	ModuleList()
	{
		boost::assign::insert( m_stDllModuleList )
			( MODULE_NAME(MainFrame), list_of(MODULE_ID_MAINFRAME) )
			( MODULE_NAME(Setting), list_of(MODULE_ID_SETTING) )
			( MODULE_NAME(TrayIcon), list_of(MODULE_ID_TRAYICON) )
			;
	}

	typedef std::vector<ModuleId>    ModuleIdList;
	typedef std::map<std::wstring,ModuleIdList>   DllModuleList;
	DllModuleList m_stDllModuleList;
};

typedef ModuleList MODULELIST;
typedef MODULELIST* PMODULELIST;
