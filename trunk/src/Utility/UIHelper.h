#pragma once

#include "stdafx.h"
#include "SndaBase.h"


class UTILITY_API UIHelper
{
public:

	/**
	* Function		获取给定字符串所占据的像素宽度
	* @strText	`	需要计算的字符串
	* @pDC			计算的设备环境
	* @return		该字符所占据的矩形区域宽度
	*/
	static UINT GetCharWidthInfo( HDC hDC, HFONT hFont ,LPCWSTR pszText );

	/**
	* Function		对于给定的矩形区域，计算给定的字符串从开始位置到正好填充改矩形区域的文字
	* @wstrText	`	需要计算的字符串
	* @nWidth		给定的矩形区域
	* @nBeginIdex	开始的索引
	* @nEndIndex	返回结束索引
	*/
	static LPCWSTR GetFittableStringFromFront( HDC hDC,HFONT hFont,LPCWSTR pszText, 
													UINT nWidth, UINT nBeginIndex, UINT& nEndIndex);

};