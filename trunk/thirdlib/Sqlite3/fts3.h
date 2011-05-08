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


/** �Ӹ����ڴ���в��Ҹ����ӿ�
@return:�ҵ�����ָ����Ӧ�ӿ��ָ�룬���򷵻�NULL
[7/8/2008 �»���]
*/
extern void *memmem (void *buf, void *sub, unsigned int n);

/** �Ӹ����ڴ���в��Ҹ����ӿ�
������Ϊmemmem�İ�ȫ�汾,����Դ�ڴ���Ҫ���ҵ��ڴ��ӿ鶼�����˳���,��ֹ�ڴ�Ƿ�����
@return:�ҵ�����ָ����Ӧ�ӿ��ָ�룬���򷵻�NULL
[7/8/2008 �»���]
*/
extern void *memmem_s (void *buf, unsigned int iBufSize, void *sub, unsigned int iSubSize);

#ifdef __cplusplus
}  /* extern "C" */
#endif  /* __cplusplus */
