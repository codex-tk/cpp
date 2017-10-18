#include <gtest/gtest.h>
#include <codex/util/time.hpp>

TEST( time , usage ) {
    ASSERT_TRUE( codex::time::is_leap_year(2000));
    ASSERT_TRUE( codex::time::is_leap_year(2004));
    ASSERT_FALSE(codex::time::is_leap_year(2100));
    
    ASSERT_EQ( codex::time::days_in_month( 2100 , 2 ) , 28 );
    ASSERT_EQ( codex::time::days_in_month( 2004 , 2 ) , 29 );
}