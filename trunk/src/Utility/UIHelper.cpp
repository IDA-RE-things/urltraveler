#include <stdafx.h>
#include "UIHelper.h"
#include "XString.h"

/**
* Function		��ȡ�����ַ�����ռ�ݵ����ؿ��
* @strText	`	��Ҫ������ַ���
* @pDC			������豸����
* @return		���ַ���ռ�ݵľ���������
*/
UINT UIHelper::GetCharWidthInfo( HDC hDC, HFONT hFont ,LPCWSTR pszText )
{
	if ( pszText == NULL)
		return 0;

	String strText = pszText;
	if( strText == L"")
		return 0;

	HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);

	UINT nWidth = 0;

	SIZE szRet;
	if (GetTextExtentPoint32(hDC, pszText, (int)strText.GetLength(), &szRet))
		nWidth = szRet.cx;
	else
		nWidth = 0;

	::SelectObject(hDC,hOldFont);
	return nWidth;
}


LPCWSTR UIHelper::GetFittableStringFromFront( HDC hDC, HFONT	hFont, LPCWSTR pszText, 
									   UINT nWidth,  UINT nBeginIndex, UINT& nEndIndex)
{
	if( pszText == NULL)
		return L"";

	String strText = pszText;

	if( nBeginIndex >=strText.GetLength())
	{
		nEndIndex = strText.GetLength() + 1;
		return L"";
	}

	int i = 0;
	for( ; i <= (int)strText.GetLength(); i++)
	{
		String strTemp = strText.SubStr( nBeginIndex, i);
		UINT nTextWidth = GetCharWidthInfo( hDC, hFont, strTemp);
		if( nTextWidth >= nWidth )
		{
			i--;
			break;
		}
	}

	String strShowText = strText.SubStr( nBeginIndex, i);
	nEndIndex = nBeginIndex + i - 1 ;

	return wcsdup(strShowText.GetData());
}