#include <gtest/gtest.h>
#include <codex/function.hpp>

TEST( function  , usage ) {
    
    codex::method_calls<codex::function < void () >>& mc 
    = codex::singleton< codex::method_calls<codex::function < void () >>>::instance();
    
    ASSERT_EQ( mc.call_counts( codex::function_callable_ctor ) , 0 );

    codex::function < void () > function([]{});

    ASSERT_EQ( mc.call_counts( codex::function_callable_ctor ) , 1 );

    function();
    int test_value = 0;
    function = [&test_value] {
        test_value += 2;
    };

    ASSERT_EQ( mc.call_counts( codex::function_callable_assign ) , 1 );
    /*
    할당 연산자가 없으면 다음과 같이 동작
    codex::function<void ()> unnamed( [&test_value] {
        test_value += 2;
    });
    function = unamed;
    */
    function();
    ASSERT_EQ( test_value , 2 );
    function();
    ASSERT_EQ( test_value , 4 );
    /*
    function( function& ) 와 match
    해당 구현이 없을시
    template < typename HandlerT >
    function( HandlerT&& handler ) 로 match
    */
    const codex::function< void () > from_ctor_ref( function );

    ASSERT_EQ( mc.call_counts( codex::function_copy_ctor_l ) , 1 );
    from_ctor_ref();
    ASSERT_EQ( test_value , 6);

    codex::function< void () > from_ctor_cref( from_ctor_ref );
    
    ASSERT_EQ( mc.call_counts( codex::function_copy_ctor ) , 1 );

    from_ctor_cref();
    ASSERT_EQ( test_value , 8);


    codex::function< void () > assign;
    assign = from_ctor_ref;

    ASSERT_EQ( mc.call_counts( codex::function_assign ) , 1 );

    assign();
    ASSERT_EQ( test_value , 10);

    assign = from_ctor_cref;
    
    ASSERT_EQ( mc.call_counts( codex::function_assign_l ) , 1 );

    assign();
    ASSERT_EQ( test_value , 12);

    assign = std::move( from_ctor_cref );

    ASSERT_EQ( mc.call_counts( codex::function_move_assign ) , 1 );

    assign();
    ASSERT_EQ( test_value , 14);

    from_ctor_cref();
    EXPECT_EQ( test_value , 14);

    assign.swap(from_ctor_cref);
    
    from_ctor_cref();
    EXPECT_EQ( test_value , 16);
}