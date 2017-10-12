#include <gtest/gtest.h>
#include <codex/util/singleton.hpp>

class singleton_target{
public:
    int value( void ) { return _value; }
    int value( int v ) { std::swap(v , _value); return v; }
private:
    int _value;
};

TEST( singleton , base ) {
    codex::singleton<singleton_target>::instance().value(1);
    ASSERT_EQ( codex::singleton<singleton_target>::instance().value() , 1 );
}