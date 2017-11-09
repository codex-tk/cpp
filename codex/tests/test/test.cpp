#include <gtest/gtest.h>

#include <vector>
#include <iostream>

namespace detail{
    class testing {
    public:
        testing() { _tests.push_back(this); }
        virtual ~testing() {}
        virtual void run() = 0;
    private:
        static std::vector< testing* > _tests;
    public:
        static void run_all_tests(){
            for( testing* test : _tests ) {
                test->run();
            }
        }
        template < typename T , typename ... Args  >
        static void print( T&& t , Args&& ... args ) {
            std::cout << t ;
            print( std::forward<Args>(args)...);
        }
    
        template < typename T >
        static void print( T&& t ) {
            std::cout << t << std::endl;
        }
    };

    std::vector< testing* > testing::_tests;
}

#ifndef ASSERT_EQUAL
#define ASSERT_EQUAL( a  ,b ) do { if ( a != b ) detail::testing::print( "[          ] " ,  a , " is not equals " , b ); } while(0)
#endif 
#ifndef CODEX_TEST
#define CODEX_TEST( test_name ) class test_##test_name : public detail::testing { virtual void run(); }; test_##test_name s_test_##test_name; void test_##test_name::run()
#endif

CODEX_TEST( sample ) {
    ASSERT_EQUAL( 1 , 2 );
}

TEST( unit_test , usage ) {
    detail::testing::run_all_tests();
}