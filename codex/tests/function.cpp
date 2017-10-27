#include <gtest/gtest.h>
#include <codex/function.hpp>

TEST( function  , usage ) {
    codex::function < void () > function([]{});
    function();
    int test_value = 0;
    function = [&test_value] {
        test_value += 2;
    };
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
    from_ctor_ref();
    ASSERT_EQ( test_value , 6);

    codex::function< void () > from_ctor_cref( from_ctor_ref );
    from_ctor_cref();
    ASSERT_EQ( test_value , 8);


    codex::function< void () > assign;
    assign = from_ctor_ref;
    assign();
    ASSERT_EQ( test_value , 10);

    assign = from_ctor_cref;
    assign();
    ASSERT_EQ( test_value , 12);

    assign = std::move( from_ctor_cref );
    assign();
    ASSERT_EQ( test_value , 14);

    from_ctor_cref();
    EXPECT_EQ( test_value , 14);

    assign.swap(from_ctor_cref);
    
    from_ctor_cref();
    EXPECT_EQ( test_value , 16);
}