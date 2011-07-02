#include "stdafx.h"
#include "scriptengine.h"
#include <io.h>

lua_State* L;

bool InitLua()
{
	bool bRet = true;
	/* ��ʼ��Lua */ 
	L = lua_open(); 
	/* ����Lua������ */ 
	luaL_openlibs(L); 
	return bRet;
}

bool UninitLua()
{
	bool bRet = true;
	/* ���Lua */ 
	lua_close(L); 
	return bRet;
}

int SCRIPTENGINE_FRONTLEE_API LuaFile(const char* pFileName)
{
	if (NULL != pFileName && 0 == _access(pFileName, 0))
	{
		/* ���нű� */ 
		luaL_dofile(L, pFileName); 
	}
	
	return 0;
}

int SCRIPTENGINE_FRONTLEE_API LuaBuffer(const char* pBuffer)
{
	if (NULL != pBuffer)
	{
		/* ���нű� */ 
		luaL_dostring(L, pBuffer); 
	}
	
	return 0;
}

bool SCRIPTENGINE_FRONTLEE_API RegisterFunction(const char* pFuncName, lua_CFunction pFuncAddr)
{
	bool bRet = false;

	if (NULL != pFuncName && NULL != pFuncAddr)
	{
		/* ע�ắ�� */
		lua_register(L, pFuncName, pFuncAddr);
		
		bRet = true;
	}
	return bRet ;
}
