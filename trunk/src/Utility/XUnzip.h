#pragma once

#include <stdafx.h>

DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{ 
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;

typedef struct
{ 
	int index;                 // index of this file within the zip
	CHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime,ctime,mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)

} ZIPENTRYW;

#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern UTILITY_API HZIP OpenZipU(void *z,unsigned int len,DWORD flags);
extern UTILITY_API ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif

extern UTILITY_API ZRESULT GetZipItemNum(HZIP hz, unsigned int* nItemNum);
extern UTILITY_API ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern UTILITY_API ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern UTILITY_API ZRESULT FindZipItemA(HZIP hz, const CHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern UTILITY_API ZRESULT FindZipItemW(HZIP hz, const CHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern UTILITY_API ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);