#include <gtest/gtest.h>
#include <codex/buffer/basic_byte_buffer.hpp>

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
    ASSERT_EQ(buf.read_ptr(),buf.write_ptr());
    int i = 0;
    while ( buf.writable_size() >= sizeof(int)) {
        memcpy( buf.write_ptr() , &i , sizeof(int) );
        buf.write_skip( sizeof(int));
        ++i;
    }
    ASSERT_EQ( buf.writable_size() , 0 );
    i = 0;
    ASSERT_NE( buf.readable_size() , 0 );
    while ( buf.readable_size() >= sizeof(int)) {
        int value = 0;
        memcpy( &value , buf.read_ptr() , sizeof(int) );
        buf.read_skip( sizeof(int));
        ASSERT_EQ(  i , value );
        ++i;
    }
    
}

TEST( byte_buffer , owner ) {
    codex::buffer::byte_buffer buf(1024);
    ASSERT_EQ( buf.block_ptr()->refcount() , 1 ); 
    {
        codex::buffer::byte_buffer shared_buf( buf );
        ASSERT_EQ( buf.block_ptr()->refcount() , 2 );
    }
    ASSERT_EQ( buf.block_ptr()->refcount() , 1 ); 

    codex::buffer::byte_buffer move( std::move(buf));
    ASSERT_EQ( move.block_ptr()->refcount() , 1 ); 
}

TEST( byte_buffer , owner1 ) {
    codex::buffer::byte_buffer buf(1024);
    ASSERT_EQ( buf.block_ptr()->refcount() , 1 ); 
    {
        codex::buffer::byte_buffer shared_buf(0);
        shared_buf = buf;
        ASSERT_EQ( buf.block_ptr()->refcount() , 2 );
    }
    ASSERT_EQ( buf.block_ptr()->refcount() , 1 ); 

    codex::buffer::byte_buffer move(0);
    move = std::move( buf );
    ASSERT_EQ( move.block_ptr()->refcount() , 1 ); 
}
    