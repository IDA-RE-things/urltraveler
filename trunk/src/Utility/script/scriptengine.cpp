#include "stdafx.h"
#include "scriptengine.h"
#include <io.h>

lua_State* L;

bool InitLua()
{
	bool bRet = true;
	/* 初始化Lua */ 
	L = lua_open(); 
	/* 载入Lua基本库 */ 
	luaL_openlibs(L); 
	return bRet;
}

bool UninitLua()
{
	bool bRet = true;
	/* 清除Lua */ 
	lua_close(L); 
	return bRet;
}

int SCRIPTENGINE_FRONTLEE_API LuaFile(const char* pFileName)
{
	if (NULL != pFileName && 0 == _access(pFileName, 0))
	{
		/* 运行脚本 */ 
		luaL_dofile(L, pFileName); 
	}
	
	return 0;
}

int SCRIPTENGINE_FRONTLEE_API LuaBuffer(const char* pBuffer)
{
	if (NULL != pBuffer)
	{
		/* 运行脚本 */ 
		luaL_dostring(L, pBuffer); 
	}
	
	return 0;
}

bool SCRIPTENGINE_FRONTLEE_API RegisterFunction(const char* pFuncName, lua_CFunction pFuncAddr)
{
	bool bRet = false;

	if (NULL != pFuncName && NULL != pFuncAddr)
	{
		/* 注册函数 */
		lua_register(L, pFuncName, pFuncAddr);
		
		bRet = true;
	}
	return bRet ;
}
