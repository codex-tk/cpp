#include <gtest/gtest.h>
#include <codex/tmp/tmp.hpp>
#include <codex/util/utility.hpp>

using test_type = codex::type_list< int , double , std::string >;
using tuple_type = std::tuple< int , double , std::string >;

TEST( type_list , at ) {
    static_assert( std::is_same< test_type::at<0> , int >::value );
    static_assert( std::is_same< test_type::at<1> , double >::value );
    static_assert( std::is_same< test_type::at<2> , std::string >::value );

    static_assert( std::is_same< test_type::front , int >::value );
    static_assert( std::is_same< test_type::back , std::string >::value );
}

TEST( type_list , modify ) {
    static_assert( std::is_same< test_type::push_front< char > 
                 , codex::type_list< char ,int , double , std::string > >::value );
    static_assert( std::is_same< test_type::push_back< char > 
                 , codex::type_list< int , double , std::string , char > >::value );
    static_assert( std::is_same< test_type::pop_front
                 , codex::type_list< double , std::string  > >::value );
    static_assert( std::is_same< test_type::pop_back
                 , codex::type_list< int , double  > >::value );

                    
    static_assert( std::is_same< 
                        test_type::template push_front< char >::template rebind< std::tuple >
                    ,   std::tuple< char ,int , double , std::string > 
                    >::value );
    static_assert( std::is_same< test_type::template push_back< char >::template rebind< std::tuple >
                 , std::tuple< int , double , std::string , char > >::value );
    static_assert( std::is_same< test_type::pop_front::template rebind< std::tuple >
                 , std::tuple< double , std::string  > >::value );
    static_assert( std::is_same< test_type::pop_back::template rebind< std::tuple >
                 , std::tuple< int , double  > >::value );
}

TEST( type_list , rebind ){ 
    static_assert( std::is_same<  test_type::rebind<std::tuple> , tuple_type >::value );
}