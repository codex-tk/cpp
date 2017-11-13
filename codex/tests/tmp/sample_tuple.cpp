#include <gtest/gtest.h>
#include <codex/tmp/tmp.hpp>

TEST( tuple , codex ) {
    //codex::sample::tuple< int , double > tup((int)1 , (double)1.0 );
    codex::sample::tuple< int , double > tup( 1 , 2.5 );
    ASSERT_EQ( codex::sample::get_value<0>(tup) , 1 );
    ASSERT_EQ( codex::sample::get_value<1>(tup) , 2.5 );
}