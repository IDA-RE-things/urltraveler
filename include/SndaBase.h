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
#define SNDADLLAPI	_declspec(dllexport)
#endif
#define DLLEXPORT		SNDADLLAPI

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


/* 函数返回基本状态信息  */
#ifdef OK
#undef OK
#endif /* #ifdef OK */
#ifndef OK
#define OK               (0)    /* 成功返回	*/
#endif /* #ifndef OK */

/* 函数返回基本状态信息  */
#ifdef FAILURE
#undef FAILURE
#endif /* #ifdef FAILURE */
#ifndef FAILURE
#define FAILURE           (-1)    /* 失败返回	*/
#endif /* #ifndef FAILURE */


#ifndef STRNCPY
#define STRNCPY(dstPtr,srcPtr) \
	memset(dstPtr, 0x0, sizeof(dstPtr)/sizeof(wchar_t)); \
	wcsncpy(dstPtr, srcPtr, sizeof(dstPtr)/sizeof(wchar_t)-1); 
#endif

#ifndef ZEROMEM
#define ZEROMEM(ptr) \
	memset(ptr, 0x0,sizeof(ptr));
#endif

// 获取数组的维数 [7/1/2008 温辉敏]
#ifndef PARRAYSIZE
#define PARRAYSIZE(array) ((sizeof(array)/sizeof(array[0])))
#endif

/** Declare all the standard PWlib class information.
This macro is used to provide the basic run-time typing capability needed
by the library. All descendent classes from the #PObject# class require
these functions for correct operation. Either use this macro or the
#PDECLARE_CLASS# macro.

The use of the #PDECLARE_CLASS# macro is no longer recommended for reasons
of compatibility with documentation systems.
*/
#define CLASSINFO(cls, par) \
  public: \
  static const char * Class() \
{ return #cls; } \
	virtual const char * GetClass(unsigned ancestor = 0) const \
{ return ancestor > 0 ? par::GetClass(ancestor-1) : cls::Class(); } \
	virtual BOOL IsClass(const char * clsName) const \
{ return strcmp(clsName, cls::Class()) == 0; } \
	virtual BOOL IsDescendant(const char * clsName) const \
{ return strcmp(clsName, cls::Class()) == 0 || \
	par::IsDescendant(clsName); } 


/** Declare all the standard PWlib class information.
This macro is used to provide the basic run-time typing capability needed
by the library. All descendent classes from the #PObject# class require
these functions for correct operation. Either use this macro or the
#PDECLARE_CLASS# macro.

The use of the #PDECLARE_CLASS# macro is no longer recommended for reasons
of compatibility with documentation systems.
*/
#define CLASSINFO_BASE(cls) \
  public: \
  static const char * Class() \
{ return #cls; } \
	virtual const char * GetClass(unsigned ancestor = 0) const \
{ return cls::Class(); } \
	virtual BOOL IsClass(const char * clsName) const \
{ return strcmp(clsName, cls::Class()) == 0; } \
	virtual BOOL IsDescendant(const char * clsName) const \
{ return strcmp(clsName, cls::Class()) == 0; } 


///删除一个数组指针的宏定义
//lint -emacro(774, DELETEA)
#define DELETEA(ptr)	\
	if(NULL != ptr)		\
{					\
	delete[] ptr;	\
	ptr = NULL;		\
}

///删除一个指针的宏定义
//lint -emacro(774, DELETEP)
#define DELETEP(ptr)	\
	if(NULL != (ptr))		\
{					\
	delete (ptr);		\
	(ptr) = NULL;		\
}

///删除一个GDI对象的宏定义
//lint -emacro(774, DELETEOBJECT)
#define DELETEOBJECT(ptr)	\
	if(NULL != (ptr))		\
{					\
	::DeleteObject (ptr);		\
	(ptr) = NULL;		\
}

///Destroy一个Window
//lint -emacro(774, DESTROYWINDOW)
#define DESTROYWINDOW(hWnd)	\
	if (IsWindow(hWnd)) \
{ \
	DestroyWindow(hWnd); \
}

///删除一个指针的宏定义
//lint -emacro(774, FREEP)
#undef  FREEP
#define FREEP(ptr)		\
	if(NULL != ptr)		\
{					\
	free(ptr) ;		\
	ptr = NULL;		\
}


/** 定义的根据输入类型来删除不同类型的指针的宏定义
*/
#define DELETE_TYPE_P(Type, ptrEvent) \
{\
	Type *ptrEventLocal = (Type *)ptrEvent; \
	DELETEP(ptrEventLocal); \
	ptrEvent = NULL; \
}


/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后以return_value值return
*/
#define PAssert_ReturnWithValue(condition, return_value) \
{  \
	if (!(condition)) \
{ \
	return (return_value); \
} \
}

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后return
*/
#define PAssert_Return(condition) \
{  \
	if (!(condition)) \
{ \
	return ; \
} \
}

#ifndef VOS_DELETE_SEM
#define VOS_DELETE_SEM(semId)	\
	if (NULL != semId)			\
{							\
	VOS_DeleteSem(semId);	\
	semId = NULL;			\
}
#endif

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后以return_value值return
*/
#define PAssert_Statement_ReturnWithValue(condition, statement, return_value) \
{  \
	if (!(condition)) \
{ \
	statement; \
	return (return_value); \
} \
}

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后return
*/
#define PAssert_Statement_Return(condition, statement) \
{  \
	if (!(condition)) \
{ \
	statement; \
	return ; \
} \
}

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement语句，然后以return_value值return
*/
#define PAssertNotNull_Statement_ReturnWithValue(ptr, statement, return_value) \
{ \
	if( (ptr) == NULL) \
{ \
	statement; \
	return (return_value); \
} \
}


/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement语句，然后return
*/
#define PAssertNotNull_Statement_Return(ptr, statement) \
{ \
	if( (ptr) == NULL) \
{ \
	statement; \
	return ; \
} \
}

/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement语句，然后以return_value值return
*/
#define PAssertNotNull_ReturnWithValue(ptr, return_value) \
{ \
	if( (ptr) == NULL) \
{ \
	return (return_value); \
} \
}


/** This macro is used to assert that a pointer must be non-null.
若指针ptr为NULL则执行statement语句，然后return
*/
#define PAssertNotNull_Return(ptr) \
{ \
	if( (ptr) == NULL) \
{ \
	return ; \
} \
}

/** This macro is used to do something and return 
执行一个语句statement，然后return return_value
*/
#define PStatement_Return(statement, return_value) \
{ \
	statement; \
	return return_value; \
}

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行break语句
*/
#define PAssert_Break(condition) \
{  \
	if (!(condition)) \
{ \
	break ; \
} \
}

/** This macro is used to do something and break 
执行一个语句statement，然后break
*/
#define PStatement_Break(statement) \
{ \
	statement; \
	break; \
}

/** This macro is used to assert that a condition must be TRUE.
若condition条件不成立则执行statement语句，然后执行break语句
*/
#define PAssert_Statement_Break(condition, statement) \
{  \
	if (!(condition)) \
{ \
	statement; \
	break ; \
} \
}

/** 空操作
*/
#define NULL_OPERATION

// 获取数组的维数 
#define PARRAYSIZE(array) ((sizeof(array)/sizeof(array[0])))

/** Declare all the standard RTTI class information.
This macro is used to provide the basic run-time typing capability needed
by the library. All descendent classes from the #PObject# class require
these functions for correct operation. Either use this macro or the
#PDECLARE_CLASS# macro.

The use of the #PDECLARE_CLASS# macro is no longer recommended for reasons
of compatibility with documentation systems.
*/
#define CLASSINFO(cls, par) \
  public: \
  static const char * Class() \
{ return #cls; } \
	virtual const char * GetClass(unsigned ancestor = 0) const \
{ return ancestor > 0 ? par::GetClass(ancestor-1) : cls::Class(); } \
	virtual BOOL IsClass(const char * clsName) const \
{ return strcmp(clsName, cls::Class()) == 0; } \
	virtual BOOL IsDescendant(const char * clsName) const \
{ return strcmp(clsName, cls::Class()) == 0 || \
	par::IsDescendant(clsName); } 


///memset缺省构造函数
#ifndef MEMSET_CONSTRUCTOR
#define MEMSET_CONSTRUCTOR(ClassType) \
	ClassType() \
{ \
	memset(this, 0, sizeof(ClassType)); \
}
#endif


/** 
按照特定数据类型删除该数据类型的数组指针
*/
#ifndef DELETE_ARRAY_TEMPLATE
#define DELETE_ARRAY_TEMPLATE
template <class classType>
void DeleteArray(void *&ptr)
{
	classType *ptrClassType = (classType *)ptr;
	DELETEA(ptrClassType);
	ptr = NULL;
}
#endif

/** 
按照特定数据类型删除该数据类型的指针
*/
#ifndef DELETE_TEMPLATE
#define DELETE_TEMPLATE
template <class classType>
void Delete(void *&ptr)
{
	classType *ptrClassType = (classType *)ptr;
	DELETEP(ptrClassType);
	ptr = NULL;
}
#endif