//
// SndaBase.h
#pragma once

#include "windows.h"
#include "string.h"
#include "assert.h"

#if defined(_WIN32) || defined(_WIN64)
#define SNDACALL			__stdcall
#else
#define SNDACALL
#endif

#if !defined(interface)
#define interface	struct
#endif

#if !defined(PURE)
#define PURE		= 0
#endif

#define SNDACALLBACK		SNDACALL
#define SNDAMETHOD(Type)	virtual Type SNDACALL
#define SNDAAPI(Type)		extern "C" Type SNDACALL

#ifdef _USE_LINUX
#define SNDADLLAPI 
#else
#define SNDADLLAPI _declspec(dllexport)
#endif

#ifndef PARRAYSIZE
#define PARRAYSIZE(array) ((sizeof(array)/sizeof(array[0])))
#endif

#ifndef DELETEP
#define DELETEP(ptr) \
	if(NULL != (ptr)) \
{ \
	delete (ptr); \
	(ptr) = NULL; \
}
#endif

#ifndef STRNCPY
#define STRNCPY(dstPtr,srcPtr) \
memset(dstPtr, 0x0, sizeof(dstPtr)/sizeof(wchar_t)); \
wcsncpy(dstPtr, srcPtr, sizeof(dstPtr)/sizeof(wchar_t)); 
#endif

#ifndef ZEROMEM
#define ZEROMEM(ptr) \
memset(ptr, 0x0,sizeof(ptr));
#endif

typedef signed __int64 int64;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;

typedef unsigned __int64 uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char byte;
typedef unsigned long ulong;


#if defined(_WIN64)
typedef uint64 uintptr;
typedef uint32 EventValue;
typedef uint32 MessageValue;
typedef uint32 ServiceValue;
typedef uint32 uin;
typedef uint32 tid;
typedef uint64 param;
#else
typedef uint32 uintptr;
typedef uint32 EventValue;
typedef uint32 MessageValue;
typedef uint32 ServiceValue;
typedef uint32 uin;
typedef uint32 tid;
typedef uint32 param;
#endif

typedef uint32 Cookie;

#ifndef MAX_PATH
#define MAX_PATH          260
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef IN
#define IN	
#endif

#ifndef OUT
#define OUT
#endif

#ifndef ASSERT
#define ASSERT assert
#endif


#ifndef TRUE
#define  TRUE	1
#endif

#ifndef FALSE
#define FALSE	1
#endif