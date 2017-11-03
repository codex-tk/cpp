#include <gtest/gtest.h>
#include <memory>

TEST( shared_ptr , cast ) {
    std::shared_ptr<int> sp( std::make_shared<int>(0) );
    ASSERT_EQ( sp.use_count() , 1 );
    std::shared_ptr<void> sp0;
    sp0 = std::static_pointer_cast<void>(sp);
    ASSERT_EQ( sp.use_count() , 2 );
    ASSERT_EQ( sp0.use_count() , 2 );
}