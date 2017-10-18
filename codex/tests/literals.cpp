#include <gtest/gtest.h>
#include <string>

/**
 * http://en.cppreference.com/w/cpp/language/user_literal
 */

int operator "" _cdx( const char *  v) {
    return std::atoi(v);
}

std::string operator "" _cdx( long double v ) {
    return std::string("double") + std::to_string(v);
}

TEST( literals , usage ) {
    ASSERT_EQ( 5 , 5_cdx );
    ASSERT_EQ( std::string("double0.500000") , 0.5_cdx );
}