#include <gtest/gtest.h>
#include <chrono>
#include "helper.hpp"

//template < typename Clock , typename Duration > int64_t to_milliseconds( const std::chrono::time_point< Clock , Duration >& tp ) {
int64_t to_milliseconds( const std::chrono::system_clock::time_point& tp ) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
}

std::chrono::system_clock::time_point from_milliseconds( int64_t ms ) {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t( 
        ms / 1000
    ) + std::chrono::milliseconds( ms % 1000 );
    return tp;
}

TEST( chrono , basic ) {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point next_day = now + std::chrono::hours(24);

    ASSERT_TRUE( next_day > now );

    // std::chrono::system_clock::time_point =
    // std::chrono::time_point< std::chrono::system_clock , std::chrono::microseconds >
    std::chrono::microseconds diff_mic_secs = next_day - now;
    std::chrono::hours diff_hours = std::chrono::duration_cast<std::chrono::hours>(diff_mic_secs);

    // change Duration
    {
        using ms_tp = std::chrono::time_point< std::chrono::system_clock , std::chrono::milliseconds >;
        ms_tp now_ms = std::chrono::time_point_cast< std::chrono::milliseconds>(now); 
        ms_tp next_day_ms = std::chrono::time_point_cast< std::chrono::milliseconds>(next_day); 
        std::chrono::milliseconds diff_milli_sec_from_ms = next_day_ms - now_ms;
        ASSERT_EQ( diff_hours , std::chrono::duration_cast<std::chrono::hours>(diff_milli_sec_from_ms));
    }
    
    ASSERT_EQ( diff_hours.count() , 24 );

    int64_t milliseconds = to_milliseconds(now);
    std::chrono::system_clock::time_point from = from_milliseconds(milliseconds);

    ASSERT_EQ( milliseconds , std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
    ASSERT_NE( now , from );
    ASSERT_EQ( std::chrono::time_point_cast< std::chrono::milliseconds >(now) , from);

}

TEST( chrono , tm ) {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    time_t ts = std::chrono::system_clock::to_time_t(tp);
    struct tm tm = *localtime(&ts);
    PRINTF( "%d/%d/%d %02d:%02d:%02d\r\n" , tm.tm_year + 1900 , tm.tm_mon + 1 , tm.tm_mday 
                    , tm.tm_hour , tm.tm_min , tm.tm_sec );
    tm = *gmtime(&ts);
    PRINTF( "%d/%d/%d %02d:%02d:%02d\r\n" , tm.tm_year + 1900 , tm.tm_mon + 1 , tm.tm_mday 
                    , tm.tm_hour , tm.tm_min , tm.tm_sec );
}