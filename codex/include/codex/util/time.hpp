#ifndef __codex_util_time_h__
#define __codex_util_time_h__

#include <chrono>

namespace codex { 
namespace time{

bool is_leap_year( const int year );
int days_in_month( const int year, const int month );


int64_t to_milliseconds( const std::chrono::system_clock::time_point& tp );
std::chrono::system_clock::time_point from_milliseconds( int64_t ms );

}}


#endif