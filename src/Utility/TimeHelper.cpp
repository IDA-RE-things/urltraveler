#include "stdafx.h"
#include "TimeHelper.h"
#include "stdio.h"
#include "time.h"

const WCHAR *TimeHelper::GetCurrentStrTime()
{
	static WCHAR strTime[64] = {0};
	SYSTEMTIME time;	
	GetLocalTime(&time);

	wsprintfW(strTime, L"%u/%u/%u  %u:%u:%u:%u", 
		time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	return strTime;
}

void TimeHelper::StrSysTime2Date(const std::wstring &strFormatDate, SYSTEMTIME &time)
{
	///时间格式:2008-09-01
	std::wstring strDate = strFormatDate;
	int iYear = 0;
	int iMonth = 0;
	int iDay = 0;

	swscanf(strDate.c_str(), L"%4d", &iYear);

	strDate = strDate.substr(5);
	swscanf(strDate.c_str(), L"%2d", &iMonth);

	strDate = strDate.substr(3);
	swscanf(strDate.c_str(), L"%2d", &iDay);

	time.wYear = iYear;
	time.wMonth = iMonth;
	time.wDay = iDay;
}

void TimeHelper::SysTime2Time(const SYSTEMTIME &sysTime,  time_t &time)
{
	struct tm tblock = {0};

	tblock.tm_year		= sysTime.wYear - 1900		;   
	tblock.tm_mon		= sysTime.wMonth	- 1	;   
	tblock.tm_mday		= sysTime.wDay		;   
	tblock.tm_hour		= sysTime.wHour		;   
	tblock.tm_min		= sysTime.wMinute		;   
	tblock.tm_sec		= sysTime.wSecond		;   
	tblock.tm_wday		= sysTime.wDayOfWeek	; 

	time = mktime(&tblock);	
}

void TimeHelper::Time2SysTime(const time_t &time, SYSTEMTIME &sysTime)
{
	/** time_t有时间限制
	Before midnight, January 1, 1970.
	After 03:14:07, January 19, 2038, UTC (using _time32 and time32_t).
	After 23:59:59, December 31, 3000, UTC (using _time64 and __time64_t).
	*/
	memset(&sysTime,  0, sizeof(sysTime));
	struct tm *tblock = localtime(&time);   

	if (NULL != tblock)
	{
		sysTime.wYear		= tblock->tm_year + 1900;   
		sysTime.wMonth		= tblock->tm_mon + 1;   
		sysTime.wDay		= tblock->tm_mday;   
		sysTime.wHour		= tblock->tm_hour;   
		sysTime.wMinute		= tblock->tm_min;   
		sysTime.wSecond		= tblock->tm_sec;   
		sysTime.wDayOfWeek	= tblock->tm_wday;  
	}
}

const WCHAR *TimeHelper::GetStrTime2(const SYSTEMTIME &time)
{
	static WCHAR strTime[64] = {0};
	memset(strTime, 0, sizeof(strTime));

	WCHAR strTmp[8] = {0};
	swprintf(strTmp, L"%d", time.wYear%100);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wYear%100);
	}
	wcscat(strTime, strTmp);

	swprintf(strTmp, L"%d", time.wMonth);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wMonth);
	}
	wcscat(strTime, strTmp);

	swprintf(strTmp, L"%d", time.wDay);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wDay);
	}
	wcscat(strTime, strTmp);

	swprintf(strTmp, L"%d", time.wHour);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wHour);
	}
	wcscat(strTime, strTmp);

	swprintf(strTmp, L"%d", time.wMinute);
	if (wcslen(strTmp) < 2)
	{
		swprintf(strTmp, L"0%d", time.wMinute);
	}
	wcscat(strTime, strTmp);

	return strTime;
}

const WCHAR *TimeHelper::GetCurrentStrTime2()
{
	static WCHAR strTime[64] = {0};
	memset(strTime, 0, sizeof(strTime));

	SYSTEMTIME time;	
	GetLocalTime(&time);

	const WCHAR * strTimeTmp = GetStrTime2(time);
	wcsncpy_s(strTime, PARRAYSIZE(strTime), strTimeTmp, PARRAYSIZE(strTime)-1);

	return strTime;
}

const SYSTEMTIME &TimeHelper::GetTimeFromStr2(const WCHAR *strTime)
{
	static SYSTEMTIME time = {0};	
	WCHAR strTmp[64] = {0};

	int iValue = 0;
	swscanf(strTime, L"%d", &iValue);

	time.wYear = iValue / 100000000;
	time.wYear += 2000;

	iValue %= 100000000;
	time.wMonth = iValue/ 1000000;

	iValue %= 1000000;
	time.wDay = iValue/ 10000;

	iValue %= 10000;
	time.wHour = iValue/ 100;

	iValue %= 100;
	time.wMinute = iValue;

	return time;
}

int TimeHelper::GetWeekDayValue(const E_WeekDay&eWeekDay)
{
	struct ST_WeekDayAndValue
	{
		E_WeekDay m_eWeekDay;
		int m_iWeekDayValue;
	};

	struct ST_WeekDayAndValue st_aWeekDayAndValue[] = 
	{
		{e_Monday,		1},
		{e_Tuesday,		2},
		{e_Wednesday,	3},
		{e_Thursday,	4},
		{e_Friday,		5},
		{e_Saturday,	6},
		{e_Sunday,		7},
	};

	int iWeekDayAndValueArraySize = PARRAYSIZE(st_aWeekDayAndValue);

	int iWeekDayValue = 1;

	for (int i = 0; i < iWeekDayAndValueArraySize; i++)
	{
		struct ST_WeekDayAndValue &st_WeekDayAndValue = st_aWeekDayAndValue[i];
		if (eWeekDay == st_WeekDayAndValue.m_eWeekDay)
		{
			iWeekDayValue = st_WeekDayAndValue.m_iWeekDayValue;
			break;
		}
	}

	return iWeekDayValue;
}