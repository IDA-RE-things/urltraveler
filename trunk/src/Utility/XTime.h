#pragma once

#include "XString.h"

class CTime
{
public:
	static CTime GetCurrentTime() throw();
	static BOOL IsValidFILETIME(const FILETIME& ft) throw();

	CTime() throw();
	CTime( __time64_t time ) throw();
	CTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
		int nDST = -1 );
	CTime( WORD wDosDate, WORD wDosTime, int nDST = -1 );
	CTime( const SYSTEMTIME& st, int nDST = -1 );
	CTime( const FILETIME& ft, int nDST = -1 );

	CTime& operator=( __time64_t time ) throw();

	bool operator==( CTime time ) const throw();
	bool operator!=( CTime time ) const throw();
	bool operator<( CTime time ) const throw();
	bool operator>( CTime time ) const throw();
	bool operator<=( CTime time ) const throw();
	bool operator>=( CTime time ) const throw();

	struct tm* GetGmtTm( struct tm* ptm ) const;
	struct tm* GetLocalTm( struct tm* ptm ) const;

	__time64_t GetTime() const throw();

	int GetYear() const throw();
	int GetMonth() const throw();
	int GetDay() const throw();
	int GetHour() const throw();
	int GetMinute() const throw();
	int GetSecond() const throw();
	int GetDayOfWeek() const throw();

	// formatting using "C" strftime
	String Format( LPCTSTR pszFormat ) const;
	String FormatGmt( LPCTSTR pszFormat ) const;
	String Format( UINT nFormatID ) const;
	String FormatGmt( UINT nFormatID ) const;

private:
	__time64_t m_time;
};