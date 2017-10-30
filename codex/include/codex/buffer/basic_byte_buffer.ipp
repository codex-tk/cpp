#include <algorithm>

namespace codex { namespace buffer {

template < typename BlockFatoryT >
basic_byte_buffer<BlockFatoryT>::basic_byte_buffer( const std::size_t size )    
    : _block_ptr( BlockFatoryT::create(size))
    , _read_pos(0)
    , _write_pos(0)
{
}

template < typename BlockFatoryT >
basic_byte_buffer<BlockFatoryT>::basic_byte_buffer( const basic_byte_buffer& rhs )
    : _block_ptr( rhs._block_ptr )
    , _read_pos( rhs._read_pos )
    , _write_pos( rhs._write_pos )
{

}

template < typename BlockFatoryT >
basic_byte_buffer<BlockFatoryT>::basic_byte_buffer( basic_byte_buffer&& rhs )    
    : _block_ptr( std::move(rhs._block_ptr) )
    , _read_pos( rhs._read_pos )
    , _write_pos( rhs._write_pos )
{
    rhs._read_pos = rhs._write_pos = 0;
}

template < typename BlockFatoryT >
basic_byte_buffer<BlockFatoryT>& basic_byte_buffer<BlockFatoryT>::operator=( const basic_byte_buffer& rhs ){
    basic_byte_buffer nbb( rhs );  
    swap( nbb );
    return *this;
}

template < typename BlockFatoryT >
basic_byte_buffer<BlockFatoryT>& basic_byte_buffer<BlockFatoryT>::operator=( basic_byte_buffer&& rhs ){
    basic_byte_buffer nbb( std::move(rhs) );  
    swap( nbb );
    return *this;
}

template < typename BlockFatoryT >
basic_byte_buffer<BlockFatoryT>::~basic_byte_buffer( void ) {
}

template < typename BlockFatoryT >
int basic_byte_buffer<BlockFatoryT>::size( void ) const noexcept {
    return _block_ptr->size();
}

template < typename BlockFatoryT >
int basic_byte_buffer<BlockFatoryT>::readable_size( void ) const noexcept{
    return _write_pos - _read_pos;
}

template < typename BlockFatoryT >
int basic_byte_buffer<BlockFatoryT>::read_skip( int seek ) noexcept{
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

template < typename BlockFatoryT >
void* basic_byte_buffer<BlockFatoryT>::read_ptr( void ) const noexcept {
    return static_cast< uint8_t*>(_block_ptr->data()) + _read_pos;
}

template < typename BlockFatoryT >
int basic_byte_buffer<BlockFatoryT>::writable_size( void ) const noexcept{
    return size() - _write_pos;
}

template < typename BlockFatoryT >
int basic_byte_buffer<BlockFatoryT>::write_skip( int seek ) noexcept{
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

template < typename BlockFatoryT >
void* basic_byte_buffer<BlockFatoryT>::write_ptr( void ) const noexcept {
    return static_cast< uint8_t*>(_block_ptr->data()) + _write_pos;
}

template < typename BlockFatoryT >
void basic_byte_buffer<BlockFatoryT>::swap( basic_byte_buffer& rhs ) noexcept{
    _block_ptr.swap(rhs._block_ptr);
    std::swap( _read_pos , rhs._read_pos );
    std::swap( _write_pos , rhs._write_pos );
}

template < typename BlockFatoryT >
typename basic_byte_buffer<BlockFatoryT>::block_ptr_type& 
    basic_byte_buffer<BlockFatoryT>::block_ptr( void ) 
{
    return _block_ptr;
}

}}