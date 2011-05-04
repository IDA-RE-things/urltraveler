#pragma once

#include "SndaBase.h"
#include <string>
#include <vector>
#include <map>
#include <xstring>

#include <boost/assign/list_of.hpp>
#include <boost/assign/list_inserter.hpp>
#include <boost/assert.hpp>
#include "tinyxml/tinyxml.h"
#include "StringHelper.h"

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
		//boost::assign::insert( m_stDllModuleList )
		//	( MODULE_NAME(MainFrame), list_of(MODULE_ID_MAINFRAME) )
		//	( MODULE_NAME(Setting), list_of(MODULE_ID_SETTING) )
		//	( MODULE_NAME(TrayIcon), list_of(MODULE_ID_TRAYICON) )
		//	;

		TiXmlDocument xmlDoc;
		TiXmlNode*    pXmlNode;
		TiXmlElement* pXmlElementModule = NULL;
		TiXmlElement* pXmlElementIModule = NULL;

		bool bRet = xmlDoc.LoadFile("Module.xml");

		if (bRet)
		{
			pXmlNode = xmlDoc.FirstChild("ModuleManager");

			for (pXmlElementModule = pXmlNode->FirstChildElement("Module");
				pXmlElementModule;
				pXmlElementModule = pXmlElementModule->NextSiblingElement())
			{
				std::string strModuleName = pXmlElementModule->Attribute("Name");
				ModuleIdList listId;

				listId.clear();

				for (pXmlElementIModule = pXmlElementModule->FirstChildElement("IModule");
					pXmlElementIModule;
					pXmlElementIModule = pXmlElementIModule->NextSiblingElement())
				{
					std::string strModuleId = pXmlElementIModule->Attribute("Id");

					listId.push_back((ModuleId)atol(strModuleId.c_str()));
				}

				m_stDllModuleList[StringHelper::ANSIToUnicode(strModuleName)] = listId;
			}
		}
	}

	typedef std::vector<ModuleId>    ModuleIdList;
	typedef std::map<std::wstring,ModuleIdList>   DllModuleList;
	DllModuleList m_stDllModuleList;
};

typedef ModuleList MODULELIST;
typedef MODULELIST* PMODULELIST;
