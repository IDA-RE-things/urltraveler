#include "stdafx.h"
#include "scriptfunctions.h"

int LuaExampleAvg(lua_State *L)
{
	/* �õ��������� */
	int n = lua_gettop(L);
	int i;
	double sum = 0;
	// ��ȡ����
	for (i = 1; i <= n; i++)
	{
		/* ��� */
		sum += lua_tonumber(L, i);
	}
	
	/* ѹ��ƽ��ֵ */
	lua_pushnumber(L, sum / n); // ����ֵ
	
	/* ���ط���ֵ�ĸ��� */
	return 1;
}

int LuaCallDllFunc(lua_State *L)
{
	/* �õ��������� */
	int n = lua_gettop(L);
	// ��ȡ����
	if (n == 2)
	{
		const char *pszDllName = luaL_checkstring(L, 1);
		const char *pszFuncName = luaL_checkstring(L, 2);
		HANDLE hDll = LoadLibraryA(pszDllName);
		lua_CFunction pFunc = NULL;
		
		if (NULL != pszDllName )
		{
			pFunc = (lua_CFunction)GetProcAddress((HMODULE )hDll,pszFuncName);
			pFunc(L);
		}
	}
	/* ���ط���ֵ�ĸ��� */
	return 0;
}

int LuaRegisterDllFuntion(lua_State *L)
{
	/* �õ��������� */
	int n = lua_gettop(L);
	// ��ȡ����
	if (n == 2)
	{
		const char *pszDllName = luaL_checkstring(L, 1);
		const char *pszFuncName = luaL_checkstring(L, 2);
		HANDLE hDll = LoadLibraryA(pszDllName);
		lua_CFunction pFunc = NULL;
		
		if (NULL != pszDllName )
		{
			pFunc = (lua_CFunction)GetProcAddress((HMODULE )hDll,pszFuncName);
			RegisterFunction(pszFuncName, pFunc);
		}
	}
	/* ���ط���ֵ�ĸ��� */
	return 0;
}

int LuaOutputDebugString(lua_State *L)
{
    /* �õ��������� */
    int n = lua_gettop(L);
    // ��ȡ����
    if (n == 1)
    {
        const char *pszText = luaL_checkstring(L, 1);
        
        if (NULL != pszText )
        {
            OutputDebugStringA((char *)pszText);
        }
	}
    /* ���ط���ֵ�ĸ��� */
    return 0;
}

int LuaGetModuleHandle(lua_State *L)
{
    int n = lua_gettop(L);

    if (n == 1)
    {
        const char *pszModuleName = luaL_checkstring(L, 1);

        lua_pushnumber(L, (DWORD)GetModuleHandleA(pszModuleName));
    }
    else
    {
        lua_pushinteger(L, 0);
    }
  
    return 1;
}

int LuaLoadLibrary(lua_State *L)
{
    /* �õ��������� */
    int n = lua_gettop(L);
    
    if (n == 1)
    {
        const char *pszModuleName = luaL_checkstring(L, 1);

        lua_pushinteger(L, (DWORD)LoadLibraryA(pszModuleName));
    }
    else
    {
        lua_pushinteger(L, 0);
    }
    
    return 1;
}


int LuaGetProcAddress(lua_State *L)
{
    /* �õ��������� */
    int n = lua_gettop(L);
    
    if (n == 2)
    {
        const char *pszModuleName = luaL_checkstring(L, 1);
        const char *pszProcName = luaL_checkstring(L, 2);
        
        lua_pushinteger(L, (DWORD)GetProcAddress(GetModuleHandleA(pszModuleName), pszProcName));
    }
    else
    {
        lua_pushinteger(L, 0);
    }
    
    return 1;
}

int  LuaGetCrc32(lua_State *L)
{
	//�õ���������
	int n = lua_gettop(L);
	if(n!=2)
	{
		lua_pushnumber(L,0);
		return 0;
	}
	
	//��ȡ����
	DWORD addr=(DWORD)lua_tointeger(L,1);
	BYTE* ptr=(BYTE*)addr;
	DWORD Size=(DWORD)lua_tonumber(L,2);

	//Crc32����
	DWORD crcTable[256],crcTmp1,crc32;
		
	//��̬����CRC-32��
	for (int i=0; i<256; i++)
	{
		crcTmp1 = i;
		for (int j=8; j>0; j--)
		{
			if (crcTmp1&1) crcTmp1 = (crcTmp1 >> 1) ^ 0xEDB88320L;
			else crcTmp1 >>= 1;
		}
		
		crcTable[i] = crcTmp1;
	}
	//����CRC32ֵ
	DWORD crcTmp2= 0xFFFFFFFF;
	while(Size--)
	{
		crcTmp2 = ((crcTmp2>>8) & 0x00FFFFFF) ^ crcTable[ (crcTmp2^(*ptr)) & 0xFF ];
		ptr++;
	}
	crc32=(crcTmp2^0xFFFFFFFF);
	
	//���ؼ�����
	lua_pushnumber(L,crc32);
	/* ���ط���ֵ�ĸ��� */
	return 1;
}

bool RegisterScripts()
{
    bool bRet = true;
    
    RegisterFunction("LuaOutputDebugString", LuaOutputDebugString);
    RegisterFunction("LuaRegDllFunc",LuaRegisterDllFuntion);
    RegisterFunction("LuaCallDllFunc",LuaCallDllFunc);
    
    RegisterFunction("LuaGetModuleHandle", LuaGetModuleHandle);
    RegisterFunction("LuaLoadLibrary", LuaLoadLibrary);
    RegisterFunction("LuaGetProcAddress", LuaGetProcAddress);
	RegisterFunction("LuaGetCrc32", LuaGetCrc32);
    
    return bRet;
}

