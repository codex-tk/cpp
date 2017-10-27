#include <gtest/gtest.h>
#include <json.hpp>
#include <boost/any.hpp>
#include "helper.hpp"

namespace {

class any_value {
public:
    any_value( int val ) //noexcept( noexcept( std::declval< boost::any >())) 
    {
        _type = type::integer;
        _value = val;
    }
    any_value( const char* str ) {
        _type = type::string;
        _value = std::string(str);
    }
    any_value( std::initializer_list<any_value>&& l ) {
        _type = type::list;
        _value = std::forward<std::initializer_list<any_value>>(l);
    }

    void print() const {
        switch( _type ){
            case type::integer:
                PRINTF( "%d\n" , boost::any_cast<int>(_value));
                break;
            case type::string:
                PRINTF( "%s\n" , boost::any_cast<std::string>(_value).c_str());
                break;
            case type::list:
                PRINTF( "List Begin\n" );
                {
                    std::initializer_list<any_value> l = boost::any_cast<std::initializer_list<any_value>>(_value);
                    for ( const any_value& nr : l ) {
                        nr.print();
                    }
                }
                PRINTF( "List End\n" );
                break;
        }
    }
private:
    enum class type {
        integer , 
        string , 
        list ,
    };
    type _type;
    boost::any _value;
};

void initialize_list_retrieve( std::initializer_list<any_value>&& l  ) {
    for ( const any_value& nr : l ) {
        nr.print();
    }
}

}

TEST( multiple_types_initialize_list , usage ){
    initialize_list_retrieve( { 0 , "str" , { 3 , "1" }} );
}