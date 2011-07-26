#pragma once

#include "stdafx.h"
#include "SndaBase.h"


class UTILITY_API UIHelper
{
public:

	/**
	* Function		��ȡ�����ַ�����ռ�ݵ����ؿ��
	* @strText	`	��Ҫ������ַ���
	* @pDC			������豸����
	* @return		���ַ���ռ�ݵľ���������
	*/
	static UINT GetCharWidthInfo( HDC hDC, HFONT hFont ,LPCWSTR pszText );

	/**
	* Function		���ڸ����ľ������򣬼���������ַ����ӿ�ʼλ�õ��������ľ������������
	* @wstrText	`	��Ҫ������ַ���
	* @nWidth		�����ľ�������
	* @nBeginIdex	��ʼ������
	* @nEndIndex	���ؽ�������
	*/
	static LPCWSTR GetFittableStringFromFront( HDC hDC,HFONT hFont,LPCWSTR pszText, 
													UINT nWidth, UINT nBeginIndex, UINT& nEndIndex);

};