#include <gtest/gtest.h>
#include <codex/tmp/member_pointer.hpp>
#include <codex/container_of.hpp>
#include "helper.hpp"

struct sample {
    int ivalue;
    double dvalue;

    int dosome( int i ) {
        ivalue = i;
        return ivalue;
    }
    void dosome( void ) {
        ivalue = 0;
    }
};

class testTmp : public testing::Test {
public:
    void SetUp() override {
        _ps = &_s;
    }
    void TearDown() override {
    }
protected:
    sample _s;
    sample* _ps;
};

TEST_F( testTmp , member_pointer ) {
    int sample::* piv = &sample::ivalue;
    double sample::* pdv = &sample::dvalue;
    codex::tmp::member_pointer<sample,int>::type piv0 = &sample::ivalue;
    codex::tmp::member_pointer<sample,double>::type pdv0 = &sample::dvalue;

    _s.*piv = 1;
    _s.*pdv = 0.1;

    ASSERT_EQ( _s.ivalue , 1 );
    ASSERT_EQ( _s.dvalue , 0.1 );
    ASSERT_EQ( _ps->*piv , 1 );
    ASSERT_EQ( _ps->*pdv , 0.1 );

    _s.*piv0 = 2;
    _s.*pdv0 = 0.2;

    ASSERT_EQ( _s.ivalue , 2 );
    ASSERT_EQ( _s.dvalue , 0.2 );
    ASSERT_EQ( _ps->*piv0 , 2 );
    ASSERT_EQ( _ps->*pdv0 , 0.2 );
}

TEST_F( testTmp ,  member_function_pointer ){
    codex::tmp::member_function_pointer< sample , int ( int ) >::type mptr = &sample::dosome;
    codex::tmp::member_function_pointer< sample , void ( void ) >::type mptr_overloaded = &sample::dosome;
    (_s.*mptr_overloaded)();
    ASSERT_EQ( _s.ivalue , 0 );
    ASSERT_EQ((_s.*mptr)(32) , 32 );
    ASSERT_EQ( _s.ivalue , 32 );
    (_ps->*mptr_overloaded)();
    ASSERT_EQ( _ps->ivalue , 0 );
    ASSERT_EQ((_ps->*mptr)(33) , 33 );
    ASSERT_EQ( _ps->ivalue , 33 );
}

TEST_F( testTmp , container_of ) {
    PRINTF( "%d %d\n"  , codex::offset_of( &sample::ivalue ) 
        , codex::offset_of( &sample::dvalue )); 
    ASSERT_EQ( codex::offset_of( &sample::ivalue ) , 0 );
    EXPECT_EQ( codex::offset_of( &sample::dvalue ) , 8 );

    sample* ps = codex::container_of( &_s.ivalue , &sample::ivalue );
    ASSERT_EQ( _ps , ps );
}
