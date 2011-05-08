#pragma once

#include "SndaBase.h"
#include <string>

using namespace std;

/** 
���ڼ�
���ڼ� = [����һ | ���ڶ�| ������| ������| ������ | ������| ������]
���ϼ�����Խ��л����
*/

///ͨ�ú궨��
#ifndef LEFT_SHIFT
///���ƺ궨��
#define LEFT_SHIFT(uiData) \
	( (uiData) * 2)
#endif

enum E_WeekDay
{
	e_Monday	= 1,
	e_Tuesday	= LEFT_SHIFT(e_Monday),
	e_Wednesday = LEFT_SHIFT(e_Tuesday),
	e_Thursday	= LEFT_SHIFT(e_Wednesday),
	e_Friday	= LEFT_SHIFT(e_Thursday),
	e_Saturday	= LEFT_SHIFT(e_Friday),
	e_Sunday	= LEFT_SHIFT(e_Saturday),
};

class UTILITY_API TimeHelper
{
public:

	/** ת���ַ�����ʽΪSYSTEMTIME
	ʱ���ʽ:2008-09-01
	@return void.
	*/
	static void StrSysTime2Date(const std::wstring &strFormatDate, SYSTEMTIME &time);

	/** ת��SYSTEMTIMEΪtime_t
	ʱ���ʽ:2008-09-01
	@return void.
	*/
	static void SysTime2Time(const SYSTEMTIME &sysTime,  time_t &time);

	/** ת��time_tΪSYSTEMTIME
	ʱ���ʽ:2008-09-01
	@return void.
	*/
	static void Time2SysTime(const time_t &time, SYSTEMTIME &sysTime);

	/** ��ȡ��ǰʱ����ַ�����ʽ
	@return const char*.���ص�ǰʱ����ַ�����ʽ
	*/
	static const WCHAR *GetCurrentStrTime();

	/** ��ȡ��ǰʱ����ַ�����ʽ
	��ʾ�ĸ�ʽΪ YYmmDDHHMM
	@return const char*.���ص�ǰʱ����ַ�����ʽ
	*/
	static const WCHAR *GetStrTime2(const SYSTEMTIME &time);

	/** ��ȡ��ǰʱ����ַ�����ʽ
	��ʾ�ĸ�ʽΪ YYmmDDHHMM
	@return const char*.���ص�ǰʱ����ַ�����ʽ
	*/
	static const WCHAR *GetCurrentStrTime2();

	/** ����ʱ����ַ�����ʽת��Ϊ��Ӧʱ��
	�ַ����ĸ�ʽΪ YYmmDDHHMM(������������ʱʱ�ַ�)
	@return SYSTEMTIME.�����ַ�����ʽת�����ʱ��
	*/
	static const SYSTEMTIME &GetTimeFromStr2(const WCHAR *strTime);

	/** ��ȡö������E_WeekDay��Ӧ�ľ����ֵ
	e_MondayΪ1
	e_TuesdayΪ2....
	@return int.
	*/
	static int GetWeekDayValue(const E_WeekDay&eWeekDay);
};