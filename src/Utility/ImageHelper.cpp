#include "stdafx.h"
#include "ImageHelper.h"


ImageHelper::ImageHelper()
{

}

ImageHelper::~ImageHelper()
{

}

HICON ImageHelper::Convert32x32IconTo16x16( HICON h32x32Icon )
{
	HDC   hMainDC,   hMemDC1,   hMemDC2; 
	HICON   h16x16Icon; 
	BITMAP   bmp; 
	HBITMAP   hOldBmp1,   hOldBmp2; 
	ICONINFO   IconInfo32x32,   IconInfo16x16; 

	GetIconInfo(h32x32Icon,   &IconInfo32x32); 

	hMainDC   =   ::GetDC(GetDesktopWindow()); 
	hMemDC1   =   CreateCompatibleDC(hMainDC); 
	hMemDC2   =   CreateCompatibleDC(hMainDC); 

	GetObject(IconInfo32x32.hbmColor,   sizeof(BITMAP),   &bmp); 

	IconInfo16x16.hbmColor   =   CreateBitmap(   16,   16,   
		bmp.bmPlanes, 
		bmp.bmBitsPixel, 
		NULL); 

	hOldBmp1   =   (HBITMAP)   SelectObject(   hMemDC1,   
		IconInfo32x32.hbmColor); 
	hOldBmp2   =   (HBITMAP)   SelectObject(   hMemDC2, 
		IconInfo16x16.hbmColor); 

	StretchBlt(hMemDC2, 
		0,   0, 
		16,   16, 
		hMemDC1, 
		0,   0, 
		32,   32, 
		SRCCOPY 
		); 

	GetObject(IconInfo32x32.hbmMask,   sizeof(BITMAP),   &bmp); 

	IconInfo16x16.hbmMask   =   CreateBitmap(   16,   16,   
		bmp.bmPlanes,   
		bmp.bmBitsPixel, 
		NULL); 

	SelectObject(hMemDC1,   IconInfo32x32.hbmMask); 
	SelectObject(hMemDC2,   IconInfo16x16.hbmMask); 

	StretchBlt(hMemDC2, 
		0,   0, 
		16,   16, 
		hMemDC1, 
		0,   0, 
		32,   32, 
		SRCCOPY 
		); 

	SelectObject(hMemDC1,   hOldBmp1); 
	SelectObject(hMemDC2,   hOldBmp2); 

	IconInfo16x16.fIcon   =   TRUE; 
	h16x16Icon   =   CreateIconIndirect(&IconInfo16x16); 

	DeleteObject(IconInfo32x32.hbmColor); 
	DeleteObject(IconInfo16x16.hbmColor); 
	DeleteObject(IconInfo32x32.hbmMask); 
	DeleteObject(IconInfo16x16.hbmMask); 
	DeleteDC(hMemDC1); 
	DeleteDC(hMemDC2); 
	::ReleaseDC(GetDesktopWindow(),   hMainDC); 

	return   h16x16Icon; 
}

HICON ImageHelper::CreateIconFromBuffer( const LPBYTE lpBuf, unsigned int nSize, int nIconSize)
{
	HICON hIcon = NULL;
	if (!lpBuf || nSize < sizeof(WORD)) {
		return NULL;
	}

	UINT bufferIndex = 0;

	if ((WORD)(lpBuf[bufferIndex]) != 0) {
		return NULL;
	}

	bufferIndex += sizeof(WORD);

	if (nSize - bufferIndex < sizeof(WORD)) {
		return NULL;
	}

	if ((WORD)(lpBuf[bufferIndex]) != 1) {
		return NULL;
	}

	bufferIndex += sizeof(WORD);

	if (nSize - bufferIndex < sizeof(WORD)) {
		return NULL;
	}

	UINT imageNumber = (WORD)(lpBuf[bufferIndex]);

	if (imageNumber < 1) {
		return NULL;
	}

	bufferIndex += sizeof(WORD);
	/* Is there enough space for the icon directory entries? */
	if ((bufferIndex + imageNumber * sizeof(ICONDIRENTRY)) > nSize) {
		return NULL;
	}

	int i = 0;
	int j = 0;
	/* Assign icon directory entries from buffer */
	LPICONDIRENTRY lpIDE = (LPICONDIRENTRY)(&lpBuf[bufferIndex]);
	LPICONDIRENTRY lpSelectEntry = NULL;
	//select the best qulify
	while (i < imageNumber)
	{
		if (lpIDE->bWidth != nIconSize || lpIDE->bWidth != nIconSize)
		{
			i++;
			lpIDE++;
			continue;
		}

		if (lpSelectEntry == NULL)
		{
			lpSelectEntry = lpIDE;
		}
		else
		{
			if (lpSelectEntry->wBitCount < lpIDE->wBitCount)
			{
				lpSelectEntry = lpIDE;
			}
		}

		i++;
		lpIDE++;
	}

	if (lpSelectEntry)
	{
		ICONIMAGE IconImage;
		IconImage.dwNumBytes = lpSelectEntry->dwBytesInRes;

		/* Seek to beginning of this image */
		if (lpSelectEntry->dwImageOffset > nSize) {
			return NULL;
		}

		bufferIndex = lpSelectEntry->dwImageOffset;

		/* Read it in */
		if ((bufferIndex + lpSelectEntry->dwBytesInRes) > nSize) {
			return NULL;
		}

		IconImage.lpBits = &lpBuf[bufferIndex];

		/* It failed, odds are good we're on NT so try the non-Ex way */
		/* We would break on NT if we try with a 16bpp image */
		if (((LPBITMAPINFO)IconImage.lpBits)->bmiHeader.biBitCount != 16) {
			hIcon = CreateIconFromResourceEx(IconImage.lpBits, IconImage.dwNumBytes, TRUE, 0x00030000, lpIDE->bWidth, lpIDE->bHeight, 0);
		}
	}

	return hIcon;
}
