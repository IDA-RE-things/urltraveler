#ifndef __DEF_H__
#define __DEF_H__

#include <WINDOWS.H>    
#include <STDIO.H>    
#include <TIME.H>    

void UnicodeToAnsi_Ptr( LPWSTR Ptr,int uLen , LPSTR pAnsiBuffer) ;  

#ifdef _DEBUG    
void Monfile_err_log(LPCSTR str) ;
#else    
#define Monfile_err_log(x)     
#endif    

void __stdcall Monfile_NotiLog(LPSTR pPath,int iActionType) ; 

typedef void (__stdcall *USER_NOTIFY_FUNC)(LPSTR pPath,int iActionType); 

typedef bool (WINAPI *_ReadDirectoryChangesW)(HANDLE    hDirectory,   
											  PVOID           lpBuffer,   
											  int             nBufferLength,   
											  bool            bWatchSubtree,   
											  int             dwNotifyFilter,   
											  int*            lpBytesReturned,   
											  OVERLAPPED*     lpOverlapped,   
											  int             lpCompletionRoutine);

typedef HANDLE  (__stdcall *_CreateFileW)(
									  LPCWSTR lpFileName,                         // file name
									  DWORD dwDesiredAccess,                      // access mode
									  DWORD dwShareMode,                          // share mode
									  LPSECURITY_ATTRIBUTES lpSecurityAttributes, // SD
									  DWORD dwCreationDisposition,                // how to create
									  DWORD dwFlagsAndAttributes,                 // file attributes
									  HANDLE hTemplateFile                        // handle to template file
									  );



#endif //__DEF_H__