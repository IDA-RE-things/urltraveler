/*
** 2006 Oct 10
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This header file is used by programs that want to link against the
** FTS3 library.  All it does is declare the sqlite3Fts3Init() interface.
*/
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

int sqlite3Fts3Init(sqlite3 *db);


/** 从给定内存块中查找给定子块
@return:找到返回指向相应子块的指针，否则返回NULL
[7/8/2008 温辉敏]
*/
extern void *memmem (void *buf, void *sub, unsigned int n);

/** 从给定内存块中查找给定子块
本函数为memmem的安全版本,对于源内存块和要查找的内存子块都定义了长度,防止内存非法访问
@return:找到返回指向相应子块的指针，否则返回NULL
[7/8/2008 温辉敏]
*/
extern void *memmem_s (void *buf, unsigned int iBufSize, void *sub, unsigned int iSubSize);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */
