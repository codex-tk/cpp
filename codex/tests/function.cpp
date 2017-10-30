#include <gtest/gtest.h>
#include <codex/function.hpp>

class function_test_fixture : public testing::Test {
public:
    void SetUp() override {
        method_calls = &codex::singleton< codex::method_calls<codex::function < void () >>>::instance();
        method_calls->reset();
        call_ids = {
            codex::function_ctor , 
            codex::function_copy_ctor ,
            codex::function_copy_ctor_l ,
            codex::function_move_ctor ,
            codex::function_callable_ctor ,
            codex::function_assign ,
            codex::function_assign_l ,
            codex::function_move_assign ,
            codex::function_callable_assign ,
        };
    }

    void TearDown() override {

    }
protected:
    codex::method_calls<codex::function < void () >>* method_calls;
    std::vector< int > call_ids;
};

typedef codex::function< void () > test_fn;

TEST_F( function_test_fixture , ctor ) {
    for ( auto id : call_ids ) {
        ASSERT_EQ( method_calls->call_counts(id) , 0 );
    }
    test_fn _function_ctor;
    ASSERT_EQ( method_calls->call_counts(codex::function_ctor) , 1 );

    const test_fn _function_copy_ctor_l(_function_ctor);
    ASSERT_EQ( method_calls->call_counts(codex::function_copy_ctor_l) , 1 );

    test_fn _function_copy_ctor( _function_copy_ctor_l );
    ASSERT_EQ( method_calls->call_counts(codex::function_copy_ctor) , 1 );

    test_fn _function_move_ctor( std::move(_function_copy_ctor));
    ASSERT_EQ( method_calls->call_counts(codex::function_move_ctor) , 1 );

    test_fn _function_callable_ctor([]{});
    ASSERT_EQ( method_calls->call_counts(codex::function_callable_ctor) , 1 );
}

TEST_F( function_test_fixture , assign ) {
    for ( auto id : call_ids ) {
        ASSERT_EQ( method_calls->call_counts(id) , 0 );
    }
    test_fn _function_ctor_rhs;
    const test_fn _function_ctor_const_rhs;
    ASSERT_EQ( method_calls->call_counts(codex::function_ctor) , 2 );

    test_fn _function_assign;
    ASSERT_EQ( method_calls->call_counts(codex::function_ctor) , 3 );

    _function_assign = _function_ctor_const_rhs;
    ASSERT_EQ( method_calls->call_counts(codex::function_assign) , 1 );
    
    _function_assign = _function_ctor_rhs;
    ASSERT_EQ( method_calls->call_counts(codex::function_assign_l) , 1 );

    _function_assign = std::move(_function_ctor_rhs);
    ASSERT_EQ( method_calls->call_counts(codex::function_move_assign) , 1 );

    _function_assign = [] {};
    ASSERT_EQ( method_calls->call_counts(codex::function_callable_assign) , 1 );
}

TEST( function  , owner_ship ) {
    int seed = 0;
    codex::function< int (void) > origin( [&seed] () -> int {
        ++seed;
        return seed;
    });

    origin();
    ASSERT_EQ( seed , 1 );

    codex::function< int (void) > copy( origin );
    copy();
    ASSERT_EQ( seed , 2 );

    codex::function< int (void) > move( std::move( copy));
    copy();
    ASSERT_EQ( seed , 2 );
    move();
    ASSERT_EQ( seed , 3 );

    codex::function< int (void) > assign;
    assign = origin;
    assign();
    ASSERT_EQ( seed , 4 );

    assign = std::move( origin );
    assign();
    ASSERT_EQ( seed , 5 );

    origin();
    ASSERT_EQ( seed , 5 );
}