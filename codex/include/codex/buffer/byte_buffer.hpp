#ifndef __codex_buffer_byte_buffer_h__
#define __codex_buffer_byte_buffer_h__

#include <codex/buffer/block_factory.hpp>

namespace codex { namespace buffer {

class byte_buffer{
public:
    explicit byte_buffer( const std::size_t size );

    ~byte_buffer( void );


    int size( void ) const noexcept ;

    int readable_size( void ) const noexcept;
    int read_skip( int seek ) noexcept;
    void* read_ptr( void ) const noexcept;

    int writable_size( void ) const noexcept;
    int write_skip( int seek ) noexcept;
    void* write_ptr( void ) const noexcept;
private:
    codex::buffer::default_block_factory::block_ptr _block_ptr;
    int _read_pos;
    int _write_pos;
};

}}

#endif