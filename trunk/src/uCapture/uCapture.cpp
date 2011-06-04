// uCapture.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "uCapture.h"
#include <atlbase.h>
#include <Exdisp.h>
#include <Mshtmlc.h>
#include "ActiveControl.h"
#include <string>


#ifdef _MANAGED
#pragma managed(push, off)
#endif

HMODULE g_module;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_module = hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

HBITMAP CreateBitmap2(IDispatch *pApp, int x, int y)
{
	HBITMAP hBitmap = 0;
	IViewObject2*	pViewObject = NULL;
	HRESULT hRes =  pApp->QueryInterface(IID_IViewObject2, (void**)&pViewObject);
	if (SUCCEEDED(hRes))
	{
		// Create memory DC
		HDC hdcMem = CreateCompatibleDC(GetDC(NULL));
		// Create bitmap
		hBitmap = CreateCompatibleBitmap (GetDC(NULL), x, y);
		// Select bitmap into DC
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

		RECTL rcBounds = { 0, 0, x, y };
		hRes = pViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, GetDC(NULL), hdcMem, &rcBounds, NULL, NULL, 0);
		if (SUCCEEDED(hRes))
		{
			// Buil the DIB
			int imgsize = ((((x * 24)+31) & ~31) >> 3)*y;
			int size = imgsize+sizeof (BITMAPINFOHEADER);
			if (char *dib = new char[size])
			{

				BITMAPINFO	*pbmi = (BITMAPINFO *)dib;
				BITMAPINFOHEADER *lpBIH = &pbmi->bmiHeader;

				lpBIH->biSize			= sizeof (BITMAPINFOHEADER);
				lpBIH->biWidth			= x;
				lpBIH->biHeight			= y;
				lpBIH->biPlanes			= 1;
				lpBIH->biBitCount		= 24;
				lpBIH->biCompression	= BI_RGB;
				lpBIH->biSizeImage		= imgsize;
				lpBIH->biXPelsPerMeter  = 0;
				lpBIH->biYPelsPerMeter  = 0;
				lpBIH->biClrUsed        = 0;
				lpBIH->biClrImportant   = 0;

				SelectObject (hdcMem, hOldBitmap);
				hOldBitmap=NULL;

				int i = GetDIBits (hdcMem, hBitmap,	0, y, dib+sizeof(BITMAPINFOHEADER), pbmi, DIB_RGB_COLORS);
				delete[] dib;
			}
		}
		
		if (hOldBitmap)
			SelectObject (hdcMem, hOldBitmap);

		DeleteDC(hdcMem);

		pViewObject->Release();
		
	}
	return hBitmap;
}

bool SaveBitmapToFile(HBITMAP   hBitmap,   std::wstring szfilename)   
{   
	HDC hDC; //   设备描述表        
	int iBits; //   当前显示分辨率下每个像素所占字节数  
	WORD wBitCount; //   位图中每个像素所占字节数    
	DWORD dwPaletteSize   =   0   ; //   定义调色板大小，  位图中像素字节大小  ，  
	DWORD dwBmBitsSize   ;   
	DWORD dwDIBSize,   dwWritten   ;   
	BITMAP Bitmap;                   
	BITMAPFILEHEADER bmfHdr; //位图属性结构  
	BITMAPINFOHEADER bi; //位图文件头结构   
	LPBITMAPINFOHEADER lpbi;   //位图信息头结构    
	HANDLE                     fh,   hDib,   hPal,hOldPal   =   NULL   ; //指向位图信息头结构,定义文件，分配内存句柄，调色板句柄
	//计算位图文件每个像素所占字节数  

	hDC   =   CreateDC(   L"DISPLAY"   ,   NULL   ,   NULL   ,   NULL   )   ;   
	iBits   =   GetDeviceCaps(   hDC   ,   BITSPIXEL   )   *   GetDeviceCaps(   hDC   ,   PLANES   )   ;   

	DeleteDC(   hDC   )   ;   

	if   (   iBits   <=   1   )   
	{ 
		wBitCount   =   1;   
	}  

	else   if   (   iBits   <=   4   )   
	{ 
		wBitCount   =   4;   
	} 

	else   if   (   iBits   <=   8   )   

	{

		wBitCount   =   8;   

	}   

	else   if   (   iBits   <=   24   )   

	{ 

		wBitCount   =   24;   

	}  

	else   if   (   iBits   <=   32   )   

	{   

		wBitCount   =   32;   

	}   

	//计算调色板大小  

	if   (   wBitCount   <=   8   )   

	{

		dwPaletteSize   =   (   1   <<   wBitCount   )   *   sizeof(   RGBQUAD   )   ;   

	}  

	//设置位图信息头结构  

	GetObject(   hBitmap   ,   sizeof(   BITMAP   )   ,   (   LPSTR   )   &   Bitmap   )   ;   

	bi.biSize                         =   sizeof(   BITMAPINFOHEADER   );   

	bi.biWidth                       =   Bitmap.bmWidth;      

	bi.biHeight                     =   Bitmap.bmHeight;    

	bi.biPlanes                     =   1;      

	bi.biBitCount                 =   wBitCount;      

	bi.biCompression           =   BI_RGB;   

	bi.biSizeImage               =   0;   

	bi.biXPelsPerMeter       =   0;   

	bi.biYPelsPerMeter       =   0;   

	bi.biClrUsed                   =   0;   

	bi.biClrImportant         =   0;   

	dwBmBitsSize   =   (   (   Bitmap.bmWidth   *   wBitCount   +   31   )   /   32   )   *   4   *   Bitmap.bmHeight   ;   

	//为位图内容分配内存  
	hDib     =   GlobalAlloc(   GHND   ,dwBmBitsSize   +   dwPaletteSize   +   sizeof(   BITMAPINFOHEADER   )   )   ;   

	lpbi   =   (   LPBITMAPINFOHEADER   )GlobalLock(   hDib   )   ; 

	*lpbi   =   bi;   

	//   处理调色板      

	hPal   =   GetStockObject(   DEFAULT_PALETTE   );   

	if   (   hPal   )   
	{   
		hDC =   ::GetDC(   NULL   );   
		hOldPal   =   SelectPalette(   hDC,   (HPALETTE   )   hPal   ,   FALSE   )   ;   
		RealizePalette(   hDC   )   ;   

	}  

	//   获取该调色板下新的像素值  

	GetDIBits(   hDC,   hBitmap,   0,   (   UINT   )   Bitmap.bmHeight,    
		(   LPSTR   )lpbi   +   sizeof(   BITMAPINFOHEADER   )   +   dwPaletteSize,   
		(   LPBITMAPINFO   )lpbi,   DIB_RGB_COLORS   );   

	//恢复调色板    
	if   (   hOldPal   )   
	{   
		SelectPalette(   hDC,   (   HPALETTE   )hOldPal,   TRUE   );   
		RealizePalette(   hDC   )   ;    
		::ReleaseDC(   NULL,   hDC   )   ;   
	}  

	//创建位图文件
	fh   =   CreateFile(   szfilename.c_str()   ,   GENERIC_WRITE,     
		0   ,   NULL   ,   CREATE_ALWAYS   ,   
		FILE_ATTRIBUTE_NORMAL   |   FILE_FLAG_SEQUENTIAL_SCAN   ,   NULL   )   ;  

	if   (   fh   ==   INVALID_HANDLE_VALUE   )   

	{   
		return   false;   
	}    

	//   设置位图文件头  
	bmfHdr.bfType   =   0x4D42;     //   "BM"     
	dwDIBSize =   sizeof(   BITMAPFILEHEADER   )   +   sizeof(   BITMAPINFOHEADER   )   +   dwPaletteSize   +   dwBmBitsSize;        
	bmfHdr.bfSize =   dwDIBSize;   
	bmfHdr.bfReserved1 =   0;   
	bmfHdr.bfReserved2 =   0;   
	bmfHdr.bfOffBits =   (   DWORD   )sizeof(   BITMAPFILEHEADER   )     
		+   (   DWORD   )sizeof(   BITMAPINFOHEADER   )   
		+   dwPaletteSize;   
	//   写入位图文件头  
	WriteFile(   fh,   (   LPSTR   )&bmfHdr,   sizeof    
		(   BITMAPFILEHEADER   ),   &dwWritten,   NULL);   
	//   写入位图文件其余内容  
	WriteFile(   fh,   (   LPSTR   )lpbi,   dwDIBSize,     

		&dwWritten   ,   NULL   );   

	GlobalUnlock(   hDib   );    
	GlobalFree(   hDib   );   
	CloseHandle(   fh   );   

	return   true;   

}

::ATL::CComPtr<IWebBrowser2> g_pBrowserApp;
CActiveXUI g_WebBroswerActive;
int g_nTimerId = 0;
CAPTURECALLBACK g_cb;

VOID CALLBACK TimerProc(HWND hwnd,
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime
						)
{
	::ATL::CComPtr<IHTMLDocument2> pDocument = NULL;
	::ATL::CComPtr<IHTMLElement>   pBody = NULL;
	::ATL::CComPtr<IDispatch>      pDispature = NULL;

	VARIANT_BOOL bBusy = VARIANT_FALSE;
	HRESULT hr;

	hr = g_pBrowserApp->get_Busy(&bBusy);

	if (bBusy == VARIANT_TRUE)
	{
		return;
	}

	g_pBrowserApp->get_Document(&pDispature);

	if (pDispature == NULL)
	{
		return;
	}

	pDispature->QueryInterface(IID_IHTMLDocument2, (void **)&pDocument);

	if (pDocument == NULL)
	{
		return;
	}

	pDocument->get_body(&pBody);
	CComQIPtr<IHTMLElement2> pBodyElem2(pBody);


	if (pBodyElem2 == NULL)
	{
		return;
	}

	long nWidth = 0;
	long nHeight = 0;

	pBodyElem2->get_scrollWidth(&nWidth);
	pBodyElem2->get_scrollHeight(&nHeight);

	g_pBrowserApp->put_Width(nWidth);
	g_pBrowserApp->put_Height(nHeight);

	g_cb.fn2(&nWidth, &nHeight);

	HBITMAP hBitmap = CreateBitmap2(pDispature, nWidth, nHeight);
	g_cb.fn1(hBitmap);
	SaveBitmapToFile(hBitmap, L"c:\\b.bmp");

	if (g_nTimerId)
	{
		KillTimer(NULL, g_nTimerId);
		g_nTimerId = 0;
	}
}

void DefaultOnCaptureFinish(HBITMAP hBitmap)
{
	SaveBitmapToFile(hBitmap, L"c:\\b.bmp");
}

void DefaultQueryCaptureSize(long *pnWidth, long *pnHeight)
{

}


UCAPTURE_API HRESULT Capture(HWND hWnd, LPTSTR szUrl, CAPTURECALLBACK *cb)
{
	HRESULT hr;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (g_nTimerId != 0)
	{
		return E_FAIL;
	}

	if (cb == NULL)
	{
		g_cb.fn1 = DefaultOnCaptureFinish;
		g_cb.fn2 = DefaultQueryCaptureSize;
	}
	else
	{
		memcpy(&g_cb, cb, sizeof(CAPTURECALLBACK));
	}

	if (g_pBrowserApp == NULL)
	{
		g_WebBroswerActive.SetMainFrame(hWnd);
		g_WebBroswerActive.SetDelayCreate(false);
		g_WebBroswerActive.SetAttribute(L"clsid", L"{8856F961-340A-11D0-A96B-00C04FD705A2}");

		g_WebBroswerActive.GetControl(IID_IWebBrowser2, (void **)&g_pBrowserApp);
		g_WebBroswerActive.SetPos(CRect(0, 0, 0, 0));
	}

	CComVariant Url(szUrl);
	CComVariant Flag = 0;
	CComVariant lp = NULL;
	CComVariant Empty;
	VARIANT_BOOL bBusy = FALSE;

	hr = g_pBrowserApp->Navigate2(&Url, &Flag, &lp, &Empty, &Empty);

	if (S_OK == hr)
	{
		g_nTimerId = SetTimer(NULL, 1, 1000, TimerProc);
	}

	return hr;
}