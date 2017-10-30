#include <gtest/gtest.h>
#include <codex/buffer/byte_buffer.hpp>

TEST( byte_buffer , ctor ) {
    codex::buffer::byte_buffer buf(1024);
    ASSERT_EQ( buf.size() , 1024 );
    ASSERT_EQ( buf.writable_size() , 1024 );
    ASSERT_EQ( buf.readable_size() , 0 );
}

TEST( byte_buffer , skip ) {
    codex::buffer::byte_buffer buf(1024);
    ASSERT_EQ( buf.read_skip(24) , 0 );
    ASSERT_EQ( buf.write_skip(24) , 24 );

    ASSERT_EQ( buf.writable_size() , 1024 - 24 );
    ASSERT_EQ( buf.readable_size() , 24 );

    ASSERT_EQ( buf.read_skip(4) , 4 );
    ASSERT_EQ( buf.readable_size() , 20 );

    ASSERT_EQ( buf.read_skip(-2) , -2 );
    ASSERT_EQ( buf.readable_size() , 22 );

    ASSERT_EQ( buf.read_skip(-10) , -2 );
    ASSERT_EQ( buf.readable_size() , 24 );

    ASSERT_EQ( buf.write_skip(-2) , -2 );
    ASSERT_EQ( buf.readable_size() , 22 );
    
    ASSERT_EQ( buf.write_skip(10000) , 1002 );
    ASSERT_EQ( buf.readable_size() , 1024 );

    ASSERT_EQ( buf.write_skip(-10000) , -1024 );
    ASSERT_EQ( buf.readable_size() , 0 );

    ASSERT_EQ( buf.write_skip(24) , 24 );
    ASSERT_EQ( buf.read_skip(4) , 4 );

    ASSERT_EQ( buf.readable_size() , 20 );

    ASSERT_EQ( buf.write_skip(-24) , -20 );
}
    


TEST( byte_buffer , ptr ) {
    codex::buffer::byte_buffer buf(1024);
}
    