#ifndef __codex_buffer_basic_byte_buffer_h__
#define __codex_buffer_basic_byte_buffer_h__

#include <codex/buffer/block_factory.hpp>

namespace codex { namespace buffer {

template < typename BlockFactoryService >
class basic_byte_buffer {
public:
    typedef typename BlockFactoryService::block_ptr_type block_ptr_type;
public:
    explicit basic_byte_buffer( const std::size_t size );

    basic_byte_buffer( const basic_byte_buffer& rhs );
    basic_byte_buffer( basic_byte_buffer&& rhs );

    basic_byte_buffer& operator=( const basic_byte_buffer& rhs );
    basic_byte_buffer& operator=( basic_byte_buffer&& rhs );

    ~basic_byte_buffer( void );

    int size( void ) const noexcept ;

    int readable_size( void ) const noexcept;
    int read_seek( int seek ) noexcept;
    void* read_ptr( void ) const noexcept;

    int writable_size( void ) const noexcept;
    int write_seek( int seek ) noexcept;
    void* write_ptr( void ) const noexcept;

    void swap( basic_byte_buffer& rhs ) noexcept ;

    block_ptr_type& block_ptr( void );

    void reserve( const std::size_t size );
    
    std::size_t consume( const std::size_t n );
    std::size_t commit( const std::size_t n );
    void* prepare( const std::size_t n );
private:
    block_ptr_type _block_ptr;
    int _read_pos;
    int _write_pos;
};

typedef basic_byte_buffer< codex::buffer::default_block_factory_service > byte_buffer;

}}

#include <codex/buffer/basic_byte_buffer.ipp>



#endif