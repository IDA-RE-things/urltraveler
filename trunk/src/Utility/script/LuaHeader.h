/**
 * @file LuaHeader.h
 * @brief 
 *
 * @author linjinming
 * @date 2010-3-14 13:22:24
 */

#ifndef __LUAHEADER_H__
#define __LUAHEADER_H__

extern "C" { 
#include "../lua/lua.h" 
#include "../lua/lualib.h" 
#include "../lua/lauxlib.h"
#pragma comment(lib,"lua5.1.lib")
}

#endif //__LUAHEADER_H__