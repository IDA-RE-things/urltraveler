/*++

 REVISION LOG ENTRY
 Revision By: linjinming
 Revised on 2010-3-17 23:52:55
 Comments: 修改为导出函数

--*/

/**
 * @file scriptengine.h
 * @brief 脚本引擎注册，销毁，执行函数
 *
 * @author FrontLee
 * @date 2010-3-14 13:30:04
 */

#ifndef __SCRIPTENGINE_H__
#define __SCRIPTENGINE_H__

#include "LuaHeader.h"

#define SCRIPTENGINE_FRONTLEE_API __declspec(dllexport)__stdcall

extern lua_State* L;

bool	InitLua();
bool	UninitLua();

extern "C"
{
int		SCRIPTENGINE_FRONTLEE_API LuaBuffer(const char* pBuffer);
int		SCRIPTENGINE_FRONTLEE_API LuaFile(const char* pFileName);
bool	SCRIPTENGINE_FRONTLEE_API RegisterFunction(const char* pFuncName, lua_CFunction pFuncAddr);
}

#endif //__SCRIPTENGINE_H__