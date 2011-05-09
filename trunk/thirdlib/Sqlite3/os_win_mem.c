/*
** 2004 May 22
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
** This file contains code that is specific to windows.
*/
#include "sqliteInt.h"
#if OS_WIN               /* This file is used for windows only */

static WCHAR *utf8ToUnicode(const char *zFilename){
  int nChar;
  WCHAR *zWideFilename;

  nChar = MultiByteToWideChar(CP_UTF8, 0, zFilename, -1, NULL, 0);
  zWideFilename = malloc( nChar*sizeof(zWideFilename[0]) );
  if( zWideFilename==0 ){
    return 0;
  }
  nChar = MultiByteToWideChar(CP_UTF8, 0, zFilename, -1, zWideFilename, nChar);
  if( nChar==0 ){
    free(zWideFilename);
    zWideFilename = 0;
  }
  return zWideFilename;
}

/*
** Convert microsoft unicode to UTF-8.  Space to hold the returned string is
** obtained from malloc().
*/
static char *unicodeToUtf8(const WCHAR *zWideFilename){
  int nByte;
  char *zFilename;

  nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, 0, 0, 0, 0);
  zFilename = malloc( nByte );
  if( zFilename==0 ){
    return 0;
  }
  nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, zFilename, nByte,
                              0, 0);
  if( nByte == 0 ){
    free(zFilename);
    zFilename = 0;
  }
  return zFilename;
}

/*
** Convert an ansi string to microsoft unicode, based on the
** current codepage settings for file apis.
** 
** Space to hold the returned string is obtained
** from malloc.
*/
static WCHAR *mbcsToUnicode(const char *zFilename){
  int nByte;
  WCHAR *zMbcsFilename;
  int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;

  nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, NULL,0)*sizeof(WCHAR);
  zMbcsFilename = malloc( nByte*sizeof(zMbcsFilename[0]) );
  if( zMbcsFilename==0 ){
    return 0;
  }
  nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, zMbcsFilename, nByte);
  if( nByte==0 ){
    free(zMbcsFilename);
    zMbcsFilename = 0;
  }
  return zMbcsFilename;
}

/*
** Convert microsoft unicode to multibyte character string, based on the
** user's Ansi codepage.
**
** Space to hold the returned string is obtained from
** malloc().
*/
static char *unicodeToMbcs(const WCHAR *zWideFilename){
  int nByte;
  char *zFilename;
  int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;

  nByte = WideCharToMultiByte(codepage, 0, zWideFilename, -1, 0, 0, 0, 0);
  zFilename = malloc( nByte );
  if( zFilename==0 ){
    return 0;
  }
  nByte = WideCharToMultiByte(codepage, 0, zWideFilename, -1, zFilename, nByte,
                              0, 0);
  if( nByte == 0 ){
    free(zFilename);
    zFilename = 0;
  }
  return zFilename;
}

/*
** Convert multibyte character string to UTF-8.  Space to hold the
** returned string is obtained from malloc().
*/
static char *mbcsToUtf8(const char *zFilename){
  char *zFilenameUtf8;
  WCHAR *zTmpWide;

  zTmpWide = mbcsToUnicode(zFilename);
  if( zTmpWide==0 ){
    return 0;
  }
  zFilenameUtf8 = unicodeToUtf8(zTmpWide);
  free(zTmpWide);
  return zFilenameUtf8;
}

/*
** Convert UTF-8 to multibyte character string.  Space to hold the 
** returned string is obtained from malloc().
*/
static char *utf8ToMbcs(const char *zFilename){
  char *zFilenameMbcs;
  WCHAR *zTmpWide;

  zTmpWide = utf8ToUnicode(zFilename);
  if( zTmpWide==0 ){
    return 0;
  }
  zFilenameMbcs = unicodeToMbcs(zTmpWide);
  free(zTmpWide);
  return zFilenameMbcs;
}



/*
** A Note About Memory Allocation:
**
** This driver uses malloc()/free() directly rather than going through
** the SQLite-wrappers sqlite3_malloc()/sqlite3_free().  Those wrappers
** are designed for use on embedded systems where memory is scarce and
** malloc failures happen frequently.  Win32 does not typically run on
** embedded systems, and when it does the developers normally have bigger
** problems to worry about than running out of memory.  So there is not
** a compelling need to use the wrappers.
**
** But there is a good reason to not use the wrappers.  If we use the
** wrappers then we will get simulated malloc() failures within this
** driver.  And that causes all kinds of problems for our tests.  We
** could enhance SQLite to deal with simulated malloc failures within
** the OS driver, but the code to deal with those failure would not
** be exercised on Linux (which does not need to malloc() in the driver)
** and so we would have difficulty writing coverage tests for that
** code.  Better to leave the code out, we think.
**
** The point of this discussion is as follows:  When creating a new
** OS layer for an embedded system, if you use this file as an example,
** avoid the use of malloc()/free().  Those routines work ok on windows
** desktops but not so well in embedded systems.
*/

#include <winbase.h>

/*
** Macros used to determine whether or not to use threads.
*/
#if defined(THREADSAFE) && THREADSAFE
# define SQLITE_W32_THREADS 1
#endif

/*
** Include code that is common to all os_*.c files
*/
#include "os_common.h"

/*
** Determine if we are dealing with WindowsCE - which has a much
** reduced API.
*/
#if defined(_WIN32_WCE)
# define OS_WINCE 1
# define AreFileApisANSI() 1
#else
# define OS_WINCE 0
#endif
/*
** The winFile structure is a subclass of sqlite3_file* specific to the win32
** portability layer.
*/
typedef struct winFile winFile;
struct winFile {
  const sqlite3_io_methods *pMethod;/* Must be first */
  HANDLE h;               /* Handle for accessing the file */
  unsigned char locktype; /* Type of lock currently held on this file */
  short sharedLockByte;   /* Randomly chosen byte used as a shared lock */
#if OS_WINCE
  WCHAR *zDeleteOnClose;  /* Name of file to delete when closing */
  HANDLE hMutex;          /* Mutex used to control access to shared lock */  
  HANDLE hShared;         /* Shared memory segment used for locking */
  winceLock local;        /* Locks obtained by this instance of winFile */
  winceLock *shared;      /* Global shared lock memory for the file  */
#endif
};


/*
** The following variable is (normally) set once and never changes
** thereafter.  It records whether the operating system is Win95
** or WinNT.
**
** 0:   Operating system unknown.
** 1:   Operating system is Win95.
** 2:   Operating system is WinNT.
**
** In order to facilitate testing on a WinNT system, the test fixture
** can manually set this value to 1 to emulate Win98 behavior.
*/
#ifdef SQLITE_TEST
int sqlite3_os_type = 0;
#else
static int sqlite3_os_type = 0;
#endif


static int isNT(void){
	if( sqlite3_os_type==0 ){
		OSVERSIONINFO sInfo;
		sInfo.dwOSVersionInfoSize = sizeof(sInfo);
		GetVersionEx(&sInfo);
		sqlite3_os_type = sInfo.dwPlatformId==VER_PLATFORM_WIN32_NT ? 2 : 1;
	}
	return sqlite3_os_type==2;
}


/*
** Return true (non-zero) if we are running under WinNT, Win2K, WinXP,
** or WinCE.  Return false (zero) for Win95, Win98, or WinME.
**
** Here is an interesting observation:  Win95, Win98, and WinME lack
** the LockFileEx() API.  But we can still statically link against that
** API as long as we don't call it win running Win95/98/ME.  A call to
** this routine is used to determine if the host is Win95/98/ME or
** WinNT/2K/XP so that we will know whether or not we can safely call
** the LockFileEx() API.
*/
/*****************************************************************************
** The next group of routines implement the I/O methods specified
** by the sqlite3_io_methods object.
******************************************************************************/

/*
** Close a file.
**
** It is reported that an attempt to close a handle might sometimes
** fail.  This is a very unreasonable result, but windows is notorious
** for being unreasonable so I do not doubt that it might happen.  If
** the close fails, we pause for 100 milliseconds and try again.  As
** many as MX_CLOSE_ATTEMPT attempts to close the handle are made before
** giving up and returning an error.
*/
typedef struct _winFileMem 
{
	unsigned char *pMemPointer;
	unsigned long  ulMemSize;
}winFileMem;

#define MX_CLOSE_ATTEMPT 3
static int winCloseMem(sqlite3_file *id){
	winFile *pFile = (winFile*)id;
	winFileMem *pMem = (winFileMem *)pFile->h;

	if (strstr(pMem->pMemPointer, "SQLite") == NULL)
	{
		free(pMem->pMemPointer);
		free(pMem);
	}
	else
	{
		int i = 0;
	}

  OpenCounter(-1);
  return SQLITE_OK;
}

static int winReadMem(
  sqlite3_file *id,          /* File to read from */
  void *pBuf,                /* Write content into this buffer */
  int amt,                   /* Number of bytes to read */
  sqlite3_int64 offset       /* Begin reading at this offset */
){
  LONG upperBits = (offset>>32) & 0x7fffffff;
  LONG lowerBits = offset & 0xffffffff;
  winFile *pFile = (winFile*)id;

  winFileMem *pMem = (winFileMem *)pFile->h;

  unsigned long ulRemain = pMem->ulMemSize - lowerBits;

  if (ulRemain < amt)
  {
	  memcpy(pBuf, &pMem->pMemPointer[lowerBits], ulRemain);
	  memset(&((char*)pBuf)[ulRemain], 0, amt-ulRemain);
	  return SQLITE_IOERR_SHORT_READ;
  }
  else
  {
	 memcpy(pBuf, &pMem->pMemPointer[lowerBits], amt);
	 return SQLITE_OK;
  }
}

/*
** Write data from a buffer into a file.  Return SQLITE_OK on success
** or some other error code on failure.
*/
static int winWriteMem(
  sqlite3_file *id,         /* File to write into */
  const void *pBuf,         /* The bytes to be written */
  int amt,                  /* Number of bytes to write */
  sqlite3_int64 offset      /* Offset into the file to begin writing at */
){
  LONG upperBits = (offset>>32) & 0x7fffffff;
  LONG lowerBits = offset & 0xffffffff;
  winFile *pFile = (winFile*)id;
  winFileMem *pMem = (winFileMem *)pFile->h;

  unsigned long ulRemain = pMem->ulMemSize - lowerBits;

  if (ulRemain < amt)
  {
	  pMem->pMemPointer = realloc(pMem->pMemPointer, pMem->ulMemSize + amt - ulRemain);
	  assert(pMem->pMemPointer != NULL);
	  pMem->ulMemSize = pMem->ulMemSize + amt - ulRemain;
  }

  memcpy(&pMem->pMemPointer[lowerBits], pBuf, amt);
  return SQLITE_OK;
}

/*
** Truncate an open file to a specified size
*/
static int winTruncateMem(sqlite3_file *id, sqlite3_int64 nByte){
  LONG upperBits = (nByte>>32) & 0x7fffffff;
  LONG lowerBits = nByte & 0xffffffff;
  winFile *pFile = (winFile*)id;

  SetFilePointer(pFile->h, lowerBits, &upperBits, FILE_BEGIN);
  SetEndOfFile(pFile->h);
  return SQLITE_OK;
}
/*
** Make sure all writes to a particular file are committed to disk.
*/
static int winSyncMem(sqlite3_file *id, int flags){
    return SQLITE_OK;
}

/*
** Determine the current size of a file in bytes
*/
static int winFileSizeMem(sqlite3_file *id, sqlite3_int64 *pSize){
  winFile *pFile = (winFile*)id;
  DWORD upperBits = 0, lowerBits;
  winFileMem *pMem = (winFileMem *)pFile->h;
  
  *pSize = (((sqlite3_int64)upperBits)<<32) + pMem->ulMemSize;
  return SQLITE_OK;
}

/*
** LOCKFILE_FAIL_IMMEDIATELY is undefined on some Windows systems.
*/
#ifndef LOCKFILE_FAIL_IMMEDIATELY
# define LOCKFILE_FAIL_IMMEDIATELY 1
#endif

/*
** Acquire a reader lock.
** Different API routines are called depending on whether or not this
** is Win95 or WinNT.
*/
static int getReadLockMem(winFile *pFile){
  return 1;
}

/*
** Undo a readlock
*/
static int unlockReadLockMem(winFile *pFile){
  return 1;
}

/*
** Lock the file with the lock specified by parameter locktype - one
** of the following:
**
**     (1) SHARED_LOCK
**     (2) RESERVED_LOCK
**     (3) PENDING_LOCK
**     (4) EXCLUSIVE_LOCK
**
** Sometimes when requesting one lock state, additional lock states
** are inserted in between.  The locking might fail on one of the later
** transitions leaving the lock state different from what it started but
** still short of its goal.  The following chart shows the allowed
** transitions and the inserted intermediate states:
**
**    UNLOCKED -> SHARED
**    SHARED -> RESERVED
**    SHARED -> (PENDING) -> EXCLUSIVE
**    RESERVED -> (PENDING) -> EXCLUSIVE
**    PENDING -> EXCLUSIVE
**
** This routine will only increase a lock.  The winUnlock() routine
** erases all locks at once and returns us immediately to locking level 0.
** It is not possible to lower the locking level one step at a time.  You
** must go straight to locking level 0.
*/
static int winLockMem(sqlite3_file *id, int locktype){
  int rc = SQLITE_OK;    /* Return code from subroutines */

  return rc;
}

/*
** This routine checks if there is a RESERVED lock held on the specified
** file by this or any other process. If such a lock is held, return
** non-zero, otherwise zero.
*/
static int winCheckReservedLockMem(sqlite3_file *id){
 
  return 1;
}

/*
** Lower the locking level on file descriptor id to locktype.  locktype
** must be either NO_LOCK or SHARED_LOCK.
**
** If the locking level of the file descriptor is already at or below
** the requested locking level, this routine is a no-op.
**
** It is not possible for this routine to fail if the second argument
** is NO_LOCK.  If the second argument is SHARED_LOCK then this routine
** might return SQLITE_IOERR;
*/
static int winUnlockMem(sqlite3_file *id, int locktype){
  int rc = SQLITE_OK;
 
  return rc;
}

/*
** Control and query of the open file handle.
*/
static int winFileControlMem(sqlite3_file *id, int op, void *pArg){
  switch( op ){
    case SQLITE_FCNTL_LOCKSTATE: {
      *(int*)pArg = ((winFile*)id)->locktype;
      return SQLITE_OK;
    }
  }
  return SQLITE_ERROR;
}

/*
** Return the sector size in bytes of the underlying block device for
** the specified file. This is almost always 512 bytes, but may be
** larger for some devices.
**
** SQLite code assumes this function cannot fail. It also assumes that
** if two files are created in the same file-system directory (i.e.
** a database and its journal file) that the sector size will be the
** same for both.
*/
static int winSectorSizeMem(sqlite3_file *id){
  return SQLITE_DEFAULT_SECTOR_SIZE;
}

/*
** Return a vector of device characteristics.
*/
static int winDeviceCharacteristicsMem(sqlite3_file *id){
  return 0;
}

/*
** This vector defines all the methods that can operate on an
** sqlite3_file for win32.
*/
static const sqlite3_io_methods winIoMethod = {
  1,                        /* iVersion */
  winCloseMem,
  winReadMem,
  winWriteMem,
  winTruncateMem,
  winSyncMem,
  winFileSizeMem,
  winLockMem,
  winUnlockMem,
  winCheckReservedLockMem,
  winFileControlMem,
  winSectorSizeMem,
  winDeviceCharacteristicsMem
};

/***************************************************************************
** Here ends the I/O methods that form the sqlite3_io_methods object.
**
** The next block of code implements the VFS methods.
****************************************************************************/
/*
** Open a file.
*/
static int winOpenMem(
  sqlite3_vfs *pVfs,        /* Not used */
  const char *zName,        /* Name of the file (UTF-8) */
  sqlite3_file *id,         /* Write the SQLite file handle here */
  int flags,                /* Open mode flags */
  int *pOutFlags            /* Status return flags */
){
  winFile *pFile = (winFile*)id;
  memset(pFile, 0, sizeof(*pFile));
  pFile->pMethod = &winIoMethod;

  if (strstr(zName, "journal"))
  {
	  winFileMem *pJournal = (winFileMem *)malloc(sizeof(winFileMem));

	  pJournal->pMemPointer = (unsigned char *)malloc(0x2000);
	  pJournal->ulMemSize = 0x2000;
	  pFile->h = (HANDLE)pJournal;
  }
  else
  {
	  sscanf(zName, "c:\\%08X", &pFile->h);
  }
  return SQLITE_OK;
}

/*
** Delete the named file.
**
** Note that windows does not allow a file to be deleted if some other
** process has it open.  Sometimes a virus scanner or indexing program
** will open a journal file shortly after it is created in order to do
** whatever does.  While this other process is holding the
** file open, we will be unable to delete it.  To work around this
** problem, we delay 100 milliseconds and try to delete again.  Up
** to MX_DELETION_ATTEMPTs deletion attempts are run before giving
** up and returning an error.
*/
#define MX_DELETION_ATTEMPTS 5
static int winDeleteMem(
  sqlite3_vfs *pVfs,          /* Not used on win32 */
  const char *zFilename,      /* Name of file to delete */
  int syncDir                 /* Not used on win32 */
){
  
  return SQLITE_OK;
}

/*
** Check the existance and status of a file.
*/
static int winAccessMem(
  sqlite3_vfs *pVfs,         /* Not used on win32 */
  const char *zFilename,     /* Name of file to check */
  int flags                  /* Type of test to make on this file */
){
  return 1;
}


/*
** Create a temporary file name in zBuf.  zBuf must be big enough to
** hold at pVfs->mxPathname characters.
*/
static int winGetTempnameMem(sqlite3_vfs *pVfs, int nBuf, char *zBuf){
	static char zChars[] =
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789";
	int i, j;
	char zTempPath[MAX_PATH+1];
	if( sqlite3_temp_directory ){
		sqlite3_snprintf(MAX_PATH-30, zTempPath, "%s", sqlite3_temp_directory);
	}else if( isNT() ){
		char *zMulti;
		WCHAR zWidePath[MAX_PATH];
		GetTempPathW(MAX_PATH-30, zWidePath);
		zMulti = unicodeToUtf8(zWidePath);
		if( zMulti ){
			sqlite3_snprintf(MAX_PATH-30, zTempPath, "%s", zMulti);
			free(zMulti);
		}else{
			return SQLITE_NOMEM;
		}
	}else{
		char *zUtf8;
		char zMbcsPath[MAX_PATH];
		GetTempPathA(MAX_PATH-30, zMbcsPath);
		zUtf8 = mbcsToUtf8(zMbcsPath);
		if( zUtf8 ){
			sqlite3_snprintf(MAX_PATH-30, zTempPath, "%s", zUtf8);
			free(zUtf8);
		}else{
			return SQLITE_NOMEM;
		}
	}
	for(i=strlen(zTempPath); i>0 && zTempPath[i-1]=='\\'; i--){}
	zTempPath[i] = 0;
	sqlite3_snprintf(nBuf-30, zBuf,
		"%s\\"SQLITE_TEMP_FILE_PREFIX, zTempPath);
	j = strlen(zBuf);
	sqlite3_randomness(20, &zBuf[j]);
	for(i=0; i<20; i++, j++){
		zBuf[j] = (char)zChars[ ((unsigned char)zBuf[j])%(sizeof(zChars)-1) ];
	}
	zBuf[j] = 0;
	OSTRACE2("TEMP FILENAME: %s\n", zBuf);
	return SQLITE_OK; 
}

/*
** Turn a relative pathname into a full pathname.  Write the full
** pathname into zOut[].  zOut[] will be at least pVfs->mxPathname
** bytes in size.
*/
static int winFullPathnameMem(
  sqlite3_vfs *pVfs,            /* Pointer to vfs object */
  const char *zRelative,        /* Possibly relative input path */
  int nFull,                    /* Size of output buffer in bytes */
  char *zFull                   /* Output buffer */
){
	sprintf(zFull, "c:\\%08X", zRelative);
    return SQLITE_OK;
}

#ifndef SQLITE_OMIT_LOAD_EXTENSION
/*
** Interfaces for opening a shared library, finding entry points
** within the shared library, and closing the shared library.
*/
/*
** Interfaces for opening a shared library, finding entry points
** within the shared library, and closing the shared library.
*/
static void *winDlOpenMem(sqlite3_vfs *pVfs, const char *zFilename){
  HANDLE h;
  void *zConverted = zFilename;
  if( zConverted==0 ){
    return 0;
  }
  if( isNT() ){
    h = LoadLibraryW((WCHAR*)zConverted);
  }else{
#if OS_WINCE
    return 0;
#else
    h = LoadLibraryA((char*)zConverted);
#endif
  }
  free(zConverted);
  return (void*)h;
}
static void winDlErrorMem(sqlite3_vfs *pVfs, int nBuf, char *zBufOut){
#if OS_WINCE
  int error = GetLastError();
  if( error>0x7FFFFFF ){
    sqlite3_snprintf(nBuf, zBufOut, "OsError 0x%x", error);
  }else{
    sqlite3_snprintf(nBuf, zBufOut, "OsError %d", error);
  }
#else
  FormatMessageA(
    FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    GetLastError(),
    0,
    zBufOut,
    nBuf-1,
    0
  );
#endif
}
void *winDlSymMem(sqlite3_vfs *pVfs, void *pHandle, const char *zSymbol){
#if OS_WINCE
  /* The GetProcAddressA() routine is only available on wince. */
  return GetProcAddressA((HANDLE)pHandle, zSymbol);
#else
  /* All other windows platforms expect GetProcAddress() to take
  ** an Ansi string regardless of the _UNICODE setting */
  return GetProcAddress((HANDLE)pHandle, zSymbol);
#endif
}
void winDlCloseMem(sqlite3_vfs *pVfs, void *pHandle){
  FreeLibrary((HANDLE)pHandle);
}
#else /* if SQLITE_OMIT_LOAD_EXTENSION is defined: */
  #define winDlOpen  0
  #define winDlError 0
  #define winDlSym   0
  #define winDlClose 0
#endif


/*
** Write up to nBuf bytes of randomness into zBuf.
*/
static int winRandomnessMem(sqlite3_vfs *pVfs, int nBuf, char *zBuf){
  int n = 0;
  if( sizeof(SYSTEMTIME)<=nBuf-n ){
    SYSTEMTIME x;
    GetSystemTime(&x);
    memcpy(&zBuf[n], &x, sizeof(x));
    n += sizeof(x);
  }
  if( sizeof(DWORD)<=nBuf-n ){
    DWORD pid = GetCurrentProcessId();
    memcpy(&zBuf[n], &pid, sizeof(pid));
    n += sizeof(pid);
  }
  if( sizeof(DWORD)<=nBuf-n ){
    DWORD cnt = GetTickCount();
    memcpy(&zBuf[n], &cnt, sizeof(cnt));
    n += sizeof(cnt);
  }
  if( sizeof(LARGE_INTEGER)<=nBuf-n ){
    LARGE_INTEGER i;
    QueryPerformanceCounter(&i);
    memcpy(&zBuf[n], &i, sizeof(i));
    n += sizeof(i);
  }
  return n;
}


/*
** Sleep for a little while.  Return the amount of time slept.
*/
static int winSleepMem(sqlite3_vfs *pVfs, int microsec){
  Sleep((microsec+999)/1000);
  return ((microsec+999)/1000)*1000;
}

/*
** The following variable, if set to a non-zero value, becomes the result
** returned from sqlite3OsCurrentTime().  This is used for testing.
*/
#ifdef SQLITE_TEST
int sqlite3_current_time = 0;
#endif

/*
** Find the current time (in Universal Coordinated Time).  Write the
** current time and date as a Julian Day number into *prNow and
** return 0.  Return 1 if the time and date cannot be found.
*/
int winCurrentTimeMem(sqlite3_vfs *pVfs, double *prNow){
  FILETIME ft;
  /* FILETIME structure is a 64-bit value representing the number of 
     100-nanosecond intervals since January 1, 1601 (= JD 2305813.5). 
  */
  double now;
#if OS_WINCE
  SYSTEMTIME time;
  GetSystemTime(&time);
  SystemTimeToFileTime(&time,&ft);
#else
  GetSystemTimeAsFileTime( &ft );
#endif
  now = ((double)ft.dwHighDateTime) * 4294967296.0; 
  *prNow = (now + ft.dwLowDateTime)/864000000000.0 + 2305813.5;
#ifdef SQLITE_TEST
  if( sqlite3_current_time ){
    *prNow = sqlite3_current_time/86400.0 + 2440587.5;
  }
#endif
  return 0;
}


/*
** Return a pointer to the sqlite3DefaultVfs structure.   We use
** a function rather than give the structure global scope because
** some compilers (MSVC) do not allow forward declarations of
** initialized structures.
*/
sqlite3_vfs *sqlite3OsDefaultVfs_(void){
  static sqlite3_vfs winVfs = {
    1,                 /* iVersion */
    sizeof(winFile),   /* szOsFile */
    MAX_PATH,          /* mxPathname */
    0,                 /* pNext */
    "win32mem",           /* zName */
    0,                 /* pAppData */
  
    winOpenMem,           /* xOpen */
    winDeleteMem,         /* xDelete */
    winAccessMem,         /* xAccess */
    winGetTempnameMem,    /* xGetTempName */
    winFullPathnameMem,   /* xFullPathname */
    winDlOpenMem,         /* xDlOpen */
    winDlErrorMem,        /* xDlError */
    winDlSymMem,          /* xDlSym */
    winDlCloseMem,        /* xDlClose */
    winRandomnessMem,     /* xRandomness */
    winSleepMem,          /* xSleep */
    winCurrentTimeMem     /* xCurrentTime */
  };
  
  return &winVfs;
}

#endif /* OS_WIN */
