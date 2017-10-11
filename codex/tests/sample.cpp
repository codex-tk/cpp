#include <gtest/gtest.h>
#include <codex/codex.hpp>

TEST( hello , test ) {
    ASSERT_EQ( codex::universe() ,82 );
    ASSERT_STREQ( "Hello" , "Hello" );
}