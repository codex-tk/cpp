#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/tmp/compile_time_error.hpp>

TEST( hello , test ) {
    ASSERT_EQ( codex::universe() ,82 );
    ASSERT_STREQ( "Hello" , "Hello" );
}

TEST( assertion , base ) {
    int value = 1;
    CODEX_ASSERT( value == 1 , "value == 1" );
}

TEST( cte , base ) {
    STATIC_CHECK( sizeof(int64_t) > sizeof(short) , old_passion_static_assert );
    static_assert( sizeof(int64_t) > sizeof(short) );
}