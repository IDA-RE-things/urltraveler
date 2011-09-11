#pragma once

#include "SndaBase.h"
#include <string>
#include <vector>
#include <map>
#include <xstring>

// 当前需要加载的所有的模块列表

#ifdef _DEBUG
#define PLUGIN_NAME(X) L#X##L"d.dll"
#else
#define PLUGIN_NAME(X) L#X##L".dll"
#endif

struct PlugInList
{
	PlugInList()
	{
		m_stDllPlugInList.push_back(PLUGIN_NAME(SogouPlugIn));
		//m_stDllPlugInList.push_back(PLUGIN_NAME(MaxthonPlugIn));
		//m_stDllPlugInList.push_back(PLUGIN_NAME(360PlugIn));
		//m_stDllPlugInList.push_back(PLUGIN_NAME(FireFoxPlugIn));
		m_stDllPlugInList.push_back(PLUGIN_NAME(IEPlugIn));
		m_stDllPlugInList.push_back(PLUGIN_NAME(ChromePlugIn));
		m_stDllPlugInList.push_back(PLUGIN_NAME(QQPlugIn));
	}

	typedef std::vector<wstring>    DllPlugInList;
	DllPlugInList m_stDllPlugInList;
};

typedef PlugInList PLUGINLIST;
typedef PLUGINLIST* PPLUGINLIST;