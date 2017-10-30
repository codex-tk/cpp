#include <codex/buffer/byte_buffer.hpp>
#include <algorithm>

namespace codex { namespace buffer {

byte_buffer::byte_buffer( const std::size_t size )    
    : _block_ptr( codex::buffer::default_block_factory::create(size))
    , _read_pos(0)
    , _write_pos(0)
{
}

byte_buffer::~byte_buffer( void ) {
}

int byte_buffer::size( void ) const noexcept {
    return _block_ptr->size();
}

int byte_buffer::readable_size( void ) const noexcept{
    return _write_pos - _read_pos;
}

int byte_buffer::read_skip( int seek ) noexcept{
    if ( seek >= 0 ) {
        seek = std::min( seek , readable_size() );
    } else {
        seek *= -1;
        if ( seek > _read_pos ) {
            seek = _read_pos;
        } 
        seek *= -1;
    }
    _read_pos += seek;
    return seek;
}

void* byte_buffer::read_ptr( void ) const noexcept {
    return static_cast< uint8_t*>(_block_ptr->data()) + _read_pos;
}

int byte_buffer::writable_size( void ) const noexcept{
    return size() - _write_pos;
}

int byte_buffer::write_skip( int seek ) noexcept{
    if ( seek >= 0 ) {
        seek = std::min( seek , writable_size() );
    } else {
        seek *= -1;
        if ( seek > readable_size() ) {
            seek = readable_size();
        } 
        seek *= -1;
    }
    _write_pos += seek;
    return seek;
}

void* byte_buffer::write_ptr( void ) const noexcept {
    return static_cast< uint8_t*>(_block_ptr->data()) + _write_pos;
}

}}