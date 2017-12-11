#include <gtest/gtest.h>
#include <codex/buffer/basic_byte_buffer.hpp>
/*
class custom_block_factory {
public:
    struct block {
        block( const std::size_t sz  ) 
            : data( sz , 0 )
        {
        }
        std::vector< char > data;
    };

    typedef std::shared_ptr< block > block_type;

    static block_type create( const std::size_t size )
    {
        return std::make_shared<block>(size);
    }

    static int size( const block_type& value ) {
        return value->data.size();
    }
    static int use_count( block_type& value ) {
        return value.use_count();
    }
    static void* data( const block_type& value ) {
        return &(value->data[0]);
    }
    static void swap( block_type& a , block_type& b ) {
        a.swap(b);
    }
};*/

using byte_buffer = codex::buffer::basic_byte_buffer<codex::buffer::custom_block_service>;
using block_service_type = byte_buffer::block_service_type;

TEST( byte_buffer , ctor ) {
    
    byte_buffer buf(1024);
    ASSERT_EQ( buf.size() , 1024 );
    ASSERT_EQ( buf.writable_size() , 1024 );
    ASSERT_EQ( buf.readable_size() , 0 );
}

TEST( byte_buffer , skip ) {
    byte_buffer buf(1024);
    ASSERT_EQ( buf.read_seek(24) , 0 );
    ASSERT_EQ( buf.write_seek(24) , 24 );

    ASSERT_EQ( buf.writable_size() , 1024 - 24 );
    ASSERT_EQ( buf.readable_size() , 24 );

    ASSERT_EQ( buf.read_seek(4) , 4 );
    ASSERT_EQ( buf.readable_size() , 20 );

    ASSERT_EQ( buf.read_seek(-2) , -2 );
    ASSERT_EQ( buf.readable_size() , 22 );

    ASSERT_EQ( buf.read_seek(-10) , -2 );
    ASSERT_EQ( buf.readable_size() , 24 );

    ASSERT_EQ( buf.write_seek(-2) , -2 );
    ASSERT_EQ( buf.readable_size() , 22 );
    
    ASSERT_EQ( buf.write_seek(10000) , 1002 );
    ASSERT_EQ( buf.readable_size() , 1024 );

    ASSERT_EQ( buf.write_seek(-10000) , -1024 );
    ASSERT_EQ( buf.readable_size() , 0 );

    ASSERT_EQ( buf.write_seek(24) , 24 );
    ASSERT_EQ( buf.read_seek(4) , 4 );

    ASSERT_EQ( buf.readable_size() , 20 );

    ASSERT_EQ( buf.write_seek(-24) , -20 );
}

TEST( byte_buffer , ptr ) {
    byte_buffer buf(1024);
    ASSERT_EQ(buf.read_ptr(),buf.write_ptr());
    int i = 0;
    while ( buf.writable_size() >= sizeof(int)) {
        memcpy( buf.write_ptr() , &i , sizeof(int) );
        buf.write_seek( sizeof(int));
        ++i;
    }
    ASSERT_EQ( buf.writable_size() , 0 );
    i = 0;
    ASSERT_NE( buf.readable_size() , 0 );
    while ( buf.readable_size() >= sizeof(int)) {
        int value = 0;
        memcpy( &value , buf.read_ptr() , sizeof(int) );
        buf.read_seek( sizeof(int));
        ASSERT_EQ(  i , value );
        ++i;
    }
}


TEST( byte_buffer , owner ) {
    byte_buffer buf(1024);
    ASSERT_EQ( block_service_type::use_count(buf.block()) , 1 ); 
    {
        byte_buffer shared_buf( buf );
        ASSERT_EQ( block_service_type::use_count(buf.block()) , 2 );
    }
    ASSERT_EQ( block_service_type::use_count(buf.block()) , 1 ); 

    byte_buffer move( std::move(buf));
    ASSERT_EQ( block_service_type::use_count(move.block()) , 1 ); 
}

TEST( byte_buffer , owner1 ) {
    byte_buffer buf(1024);
    ASSERT_EQ( block_service_type::use_count(buf.block()) , 1 ); 
    {
        byte_buffer shared_buf(0);
        shared_buf = buf;
        ASSERT_EQ( block_service_type::use_count(buf.block()) , 2 );
    }
    ASSERT_EQ( block_service_type::use_count(buf.block()) , 1 ); 

    byte_buffer move(0);
    move = std::move( buf );
    ASSERT_EQ( block_service_type::use_count(move.block()) , 1 ); 
}

TEST( byte_buffer , reserve ) {
    byte_buffer buf(4);
    void* orig_ptr = block_service_type::data(buf.block());
    // 01--
    buf.write_seek(2);
    ASSERT_EQ( block_service_type::data(buf.block()) , buf.read_ptr() );
    //-1--
    buf.read_seek(1);
    ASSERT_NE( block_service_type::data(buf.block()) , buf.read_ptr() );
    buf.reserve(3);
    // 1---
    ASSERT_EQ( block_service_type::data(buf.block()) , orig_ptr );
    ASSERT_EQ( buf.writable_size() , 3 );
    ASSERT_EQ( block_service_type::data(buf.block()) , buf.read_ptr() );
}

TEST( byte_buffer , consume_commit_prepare ) {
    byte_buffer buf(4);  
    memset( buf.prepare(1) , 0x01 , 1 );
    ASSERT_EQ(buf.commit(1) , 1 );
    ASSERT_EQ(buf.readable_size() , 1 );
    ASSERT_EQ(buf.writable_size() , 3 );
    ASSERT_EQ(buf.consume(1) , 1 );
}