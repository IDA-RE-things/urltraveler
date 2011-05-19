#include "stdafx.h"
#include "Base.h"
#include <Shlwapi.h>
#include <tchar.h>

void __Trace(LPCTSTR pstrFormat, ...)
{
#ifdef _DEBUG
	TCHAR szBuffer[300] = { 0 };
	va_list args;
	va_start(args, pstrFormat);
	::wvnsprintf(szBuffer, lengthof(szBuffer) - 2, pstrFormat, args);
	_tcscat(szBuffer, _T("\n"));
	va_end(args);
	::OutputDebugString(szBuffer);
#endif
}

CRect::CRect()
{
	left = top = right = bottom = 0;
}

CRect::CRect(const RECT& src)
{
	left = src.left;
	top = src.top;
	right = src.right;
	bottom = src.bottom;
}

CRect::CRect(int iLeft, int iTop, int iRight, int iBottom)
{
	left = iLeft;
	top = iTop;
	right = iRight;
	bottom = iBottom;
}

int CRect::GetWidth() const
{
	return right - left;
}

int CRect::GetHeight() const
{
	return bottom - top;
}

void CRect::Empty()
{
	left = top = right = bottom = 0;
}

bool CRect::IsNull() const
{
	return (left == 0 && right == 0 && top == 0 && bottom == 0); 
}

void CRect::Join(const RECT& rc)
{
	if( rc.left < left ) left = rc.left;
	if( rc.top < top ) top = rc.top;
	if( rc.right > right ) right = rc.right;
	if( rc.bottom > bottom ) bottom = rc.bottom;
}

void CRect::ResetOffset()
{
	::OffsetRect(this, -left, -top);
}

void CRect::Normalize()
{
	if( left > right ) { int iTemp = left; left = right; right = iTemp; }
	if( top > bottom ) { int iTemp = top; top = bottom; bottom = iTemp; }
}

void CRect::Offset(int cx, int cy)
{
	::OffsetRect(this, cx, cy);
}

void CRect::Inflate(int cx, int cy)
{
	::InflateRect(this, cx, cy);
}

void CRect::Deflate(int cx, int cy)
{
	::InflateRect(this, -cx, -cy);
}

void CRect::Union(CRect& rc)
{
	::UnionRect(this, this, &rc);
}




