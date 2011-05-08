#pragma once

#include "SndaBase.h"
#include <string>

using namespace std;

/** 
星期几
星期几 = [星期一 | 星期二| 星期三| 星期四| 星期五 | 星期六| 星期天]
以上几项可以进行或操作
*/

///通用宏定义
#ifndef LEFT_SHIFT
///左移宏定义
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

	/** 转换字符串格式为SYSTEMTIME
	时间格式:2008-09-01
	@return void.
	*/
	static void StrSysTime2Date(const std::wstring &strFormatDate, SYSTEMTIME &time);

	/** 转换SYSTEMTIME为time_t
	时间格式:2008-09-01
	@return void.
	*/
	static void SysTime2Time(const SYSTEMTIME &sysTime,  time_t &time);

	/** 转换time_t为SYSTEMTIME
	时间格式:2008-09-01
	@return void.
	*/
	static void Time2SysTime(const time_t &time, SYSTEMTIME &sysTime);

	/** 获取当前时间的字符串形式
	@return const char*.返回当前时间的字符串形式
	*/
	static const WCHAR *GetCurrentStrTime();

	/** 获取当前时间的字符串形式
	显示的格式为 YYmmDDHHMM
	@return const char*.返回当前时间的字符串形式
	*/
	static const WCHAR *GetStrTime2(const SYSTEMTIME &time);

	/** 获取当前时间的字符串形式
	显示的格式为 YYmmDDHHMM
	@return const char*.返回当前时间的字符串形式
	*/
	static const WCHAR *GetCurrentStrTime2();

	/** 根据时间的字符串形式转换为相应时间
	字符串的格式为 YYmmDDHHMM(年年月月日日时时分分)
	@return SYSTEMTIME.返回字符串形式转换后的时间
	*/
	static const SYSTEMTIME &GetTimeFromStr2(const WCHAR *strTime);

	/** 获取枚举类型E_WeekDay对应的具体的值
	e_Monday为1
	e_Tuesday为2....
	@return int.
	*/
	static int GetWeekDayValue(const E_WeekDay&eWeekDay);
};