#include "stdafx.h"
#include "common.h"

#ifdef _DEBUG    
void Monfile_err_log(LPCSTR str)   
{   
    printf("%s\n",str);   
}   
#else    
#define Monfile_err_log(x)     
#endif    




/*++

 Function Name: Monfile_NotiLog
 Abstract: 
 ReturnType: void __stdcall 
 Parameters:
		[in/out] - LPSTR pPath	--> 
		[in/out] - int iActionType	--> 
 Remarks:

*/
void __stdcall Monfile_NotiLog(LPSTR pPath,int iActionType)   
{   
    printf("[%d]%s\n",iActionType,pPath);   
} 


/*++

 Function Name: UnicodeToAnsi_Ptr
 Abstract: 
 ReturnType: void 
 Parameters:
		[in/out] -  LPWSTR Ptr	--> 
		[in/out] - int uLen	--> 
		[in/out] - LPSTR pAnsiBuffer	--> 
 Remarks:

*/
void UnicodeToAnsi_Ptr( LPWSTR Ptr,int uLen , LPSTR pAnsiBuffer)   
{   
    int iRetLen = WideCharToMultiByte(0,0,Ptr,uLen,NULL,0,0,0);   
    WideCharToMultiByte(0,0,Ptr,uLen,pAnsiBuffer,iRetLen,0,0);   
}   
