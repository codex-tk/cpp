#include "stdafx.h"
#include <codex/time/ctime.hpp>

namespace codex {
namespace time {
namespace {

static const int DAY_PER_MONTH_TABLE [2][12] = {
	{ 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ,
	{ 31 , 29 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ,
};

}

/*
�׷������� ���� ��Ģ�� ��Ȯ�� 4�⸶�� ������ ���� �����콺���� ������ ���̴�. ��Ȯ�� ��Ģ�� ������ ����.
���� ��� ������ 4�� ������ �������� �ش� �켱 �������� �Ѵ�. (2004��, 2008��, ��)
100���� ������ �������� �ش� ������� �Ѵ�. (2100��, 2200��, ��)
400���� ������ �������� �ش� �ٽ� �������� �Ѵ�. (1600��, 2000��, ��)
�� 400�� �ֱ�� 97���� ������ �ݺ��Ǹ�, 1�⿡�� ��� 365.2425���� ��� ����. 
���� (������� �������� ��) �¾���� �� 365.242375���̱� ������, �� 8000�⸶�� 1���� ������ �߻��Ѵ�.
- ��Ű����
*/
bool ctime::isleapyear( const int year ) {
	return (( year % 4 == 0 ) && ( year % 100 != 0 )) || ( year % 400 == 0);
	/*
	if ( (year%4) == 0 ) {
		if ( (year%100) == 0 ) {
			if ( (year%400) == 0 ) {
				return true;
			}
			return false;
		}
		return true;
	}
	return false;*/
}

int  ctime::daycount( const int year ) {
	return ctime::isleapyear( year ) ? 366 : 365;
}

int  ctime::daycount( const int startYear , const int endYear ) {
	int stYear = startYear;
	int v = 0;
	while ( stYear <= endYear ) {
		v += ctime::daycount( stYear );
		++stYear;
	}
	return v;
}

int  ctime::daycount( const int year , const int month , const int day ) {
	int daycount = 0;
	int monthTableIndex = ctime::isleapyear( year ) ? 1 : 0 ;

	for ( int i = 0 ; i < month - 1 ; ++i ) {
		daycount += DAY_PER_MONTH_TABLE[ monthTableIndex ][i];
	}
	daycount += day;
	return daycount;
}
/*
void ctime::gmtime( const time_t aTime , tm& aTm ) {
	aTm.tm_sec  = ( aTime % 60 );
	aTm.tm_min  = ( aTime % ( 60 * 60 ) ) / 60;
	aTm.tm_hour = ( aTime % ( 60 * 60 * 24 )) / ( 60 * 60 );
	aTm.tm_wday = ((aTime / ( 60 * 60 * 24 )) + 4 ) % 7; // 0 day thursday

	int32_t daycount = static_cast< int32_t >( aTime / ( 60 * 60 * 24 ));
	int32_t year     = 1970;

	for(;;) {
		int32_t dayPerYear = ctime::daycount( year );
		if ( daycount < dayPerYear )
			break;
		++year;
		daycount -= dayPerYear;
	}

	aTm.tm_year = static_cast< int > (year - 1900);
	aTm.tm_yday = static_cast< int > (daycount);
	int32_t month = 0;
	int monthTableIndex =  ctime::isleapyear( year ) ? 1 : 0;
	for(;;) {
		if ( daycount < DAY_PER_MONTH_TABLE[ monthTableIndex ][ month ] ){
			break;
		}
		daycount -= DAY_PER_MONTH_TABLE[ monthTableIndex ][ month ];
		++month;
	}
	aTm.tm_mon  = static_cast< int > (month);
	aTm.tm_mday = static_cast< int > (daycount + 1);
	aTm.tm_isdst = 0;

}*/

time_t ctime::convert( const tm& aTm ) {
	time_t t = 0;
	t += aTm.tm_sec;
	t += aTm.tm_min  * 60;
	t += aTm.tm_hour * 60 * 60;
	t += aTm.tm_yday * 60 * 60 * 24;
	//[1970 , aT.tm_year + 1900) �̳��� ��¥�� �̹Ƿ� -1 �ؾ��Ѵ�.
	t += daycount( 1970 , ( aTm.tm_year + 1900 - 1 ) ) * (60 * 60 * 24);	
	//t -= ( 9 * 60 * 60 );
	return t;
}

struct tm ctime::convert( const time_t& aTime ) {
	struct tm aTm;
	aTm.tm_sec  = ( aTime % 60 );
	aTm.tm_min  = ( aTime % ( 60 * 60 ) ) / 60;
	aTm.tm_hour = ( aTime % ( 60 * 60 * 24 )) / ( 60 * 60 );
	aTm.tm_wday = ((aTime / ( 60 * 60 * 24 )) + 4 ) % 7; // 0 day thursday

	int32_t daycount = static_cast< int32_t >( aTime / ( 60 * 60 * 24 ));
	int32_t year     = 1970;

	for(;;) {
		int32_t dayPerYear = ctime::daycount( year );
		if ( daycount < dayPerYear )
			break;
		++year;
		daycount -= dayPerYear;
	}

	aTm.tm_year = static_cast< int > (year - 1900);
	aTm.tm_yday = static_cast< int > (daycount);
	int32_t month = 0;
	int monthTableIndex =  ctime::isleapyear( year ) ? 1 : 0;
	for(;;) {
		if ( daycount < DAY_PER_MONTH_TABLE[ monthTableIndex ][ month ] ){
			break;
		}
		daycount -= DAY_PER_MONTH_TABLE[ monthTableIndex ][ month ];
		++month;
	}
	aTm.tm_mon  = static_cast< int > (month);
	aTm.tm_mday = static_cast< int > (daycount + 1);
	aTm.tm_isdst = 0;
	return aTm;
}

/*
void ctime::localtime( const time_t aTime , tm& aTm ) {
	static time_t bias_val = ctime::bias() * 60;
	time_t lct = aTime - bias_val;
	ctime::gmtime( lct , aTm );
}

// input ��  localtime t
time_t ctime::mktime( const tm& aTm ) {
	time_t t = 0;
	t += aTm.tm_sec;
	t += aTm.tm_min  * 60;
	t += aTm.tm_hour * 60 * 60;
	t += aTm.tm_yday * 60 * 60 * 24;
	//[1970 , aT.tm_year + 1900) �̳��� ��¥�� �̹Ƿ� -1 �ؾ��Ѵ�.
	t += daycount( 1970 , ( aTm.tm_year + 1900 - 1 ) ) * (60 * 60 * 24);	
	//t -= ( 9 * 60 * 60 );
	return t;
}

int  ctime::bias() {
#if defined(_WIN32) || defined(_WIN64)
	TIME_ZONE_INFORMATION tz;
	GetTimeZoneInformation( &tz );
	return tz.Bias + tz.DaylightBias;
#else
	static int diff = 0;
	if ( diff == 0 ) {
		 time_t utc_time;
		 std::time( &utc_time );
		 struct tm* local = std::localtime( &utc_time );
		 time_t local_time = std::mktime( local );
		 diff = static_cast<int>(local_time - utc_time) / 60;
	}
	return diff;
#endif
}
*/
}}