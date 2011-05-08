#pragma once

#include "SndaBase.h"
#include <string>
#include <vector>
#include <map>
#include <xstring>

// ��ǰ��Ҫ���ص����е�ģ���б�

#ifdef _DEBUG
#define PLUGIN_NAME(X) L#X##L"d.dll"
#else
#define PLUGIN_NAME(X) L#X##L".dll"
#endif

struct PlugInList
{
	PlugInList()
	{
		m_stDllPlugInList.push_back(PLUGIN_NAME(IEPlugIn));
		m_stDllPlugInList.push_back(PLUGIN_NAME(SogouPlugIn));
	}

	typedef std::vector<wstring>    DllPlugInList;
	DllPlugInList m_stDllPlugInList;
};

typedef PlugInList PLUGINLIST;
typedef PLUGINLIST* PPLUGINLIST;
