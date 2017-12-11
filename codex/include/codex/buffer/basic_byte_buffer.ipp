#include <algorithm>

namespace codex { namespace buffer {

template < typename BlockServiceT >
basic_byte_buffer<BlockServiceT>::basic_byte_buffer( const std::size_t size )    
    : _block( block_service_type::create(size) )
    , _read_pos(0)
    , _write_pos(0)
{
}

template < typename BlockServiceT >
basic_byte_buffer<BlockServiceT>::basic_byte_buffer( const basic_byte_buffer& rhs )
    : _block( rhs._block )
    , _read_pos( rhs._read_pos )
    , _write_pos( rhs._write_pos )
{

}

template < typename BlockServiceT >
basic_byte_buffer<BlockServiceT>::basic_byte_buffer( basic_byte_buffer&& rhs )    
    : _block( std::move(rhs._block) )
    , _read_pos( rhs._read_pos )
    , _write_pos( rhs._write_pos )
{
    rhs._read_pos = rhs._write_pos = 0;
}

template < typename BlockServiceT >
basic_byte_buffer<BlockServiceT>& basic_byte_buffer<BlockServiceT>::operator=( const basic_byte_buffer& rhs ){
    basic_byte_buffer nbb( rhs );  
    swap( nbb );
    return *this;
}

template < typename BlockServiceT >
basic_byte_buffer<BlockServiceT>& basic_byte_buffer<BlockServiceT>::operator=( basic_byte_buffer&& rhs ){
    basic_byte_buffer nbb( std::move(rhs) );  
    swap( nbb );
    return *this;
}

template < typename BlockServiceT >
basic_byte_buffer<BlockServiceT>::~basic_byte_buffer( void ) {
}

template < typename BlockServiceT >
int basic_byte_buffer<BlockServiceT>::size( void ) const noexcept {
    return block_service_type::size(_block);
}

template < typename BlockServiceT >
int basic_byte_buffer<BlockServiceT>::readable_size( void ) const noexcept{
    return _write_pos - _read_pos;
}

template < typename BlockServiceT >
int basic_byte_buffer<BlockServiceT>::read_seek( int seek ) noexcept{
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

template < typename BlockServiceT >
void* basic_byte_buffer<BlockServiceT>::read_ptr( void ) const noexcept {
    return static_cast< uint8_t*>( block_service_type::data(_block)) + _read_pos;
}

template < typename BlockServiceT >
int basic_byte_buffer<BlockServiceT>::writable_size( void ) const noexcept{
    return size() - _write_pos;
}

template < typename BlockServiceT >
int basic_byte_buffer<BlockServiceT>::write_seek( int seek ) noexcept{
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

template < typename BlockServiceT >
void* basic_byte_buffer<BlockServiceT>::write_ptr( void ) const noexcept {
    return static_cast< uint8_t*>(block_service_type::data(_block)) + _write_pos;
}

template < typename BlockServiceT >
void basic_byte_buffer<BlockServiceT>::swap( basic_byte_buffer& rhs ) noexcept{
    _block.swap(rhs._block);
    std::swap( _read_pos , rhs._read_pos );
    std::swap( _write_pos , rhs._write_pos );
}

template < typename BlockServiceT >
typename basic_byte_buffer<BlockServiceT>::block_type& 
    basic_byte_buffer<BlockServiceT>::block( void ) 
{
    return _block;
}

template < typename BlockServiceT >
void basic_byte_buffer<BlockServiceT>::reserve( const std::size_t size ){
    if ( writable_size() >= static_cast<int>(size) ) {
        return;
    }
    if ( this->size() - readable_size() >= static_cast<int>(size) 
        && block_service_type::use_count(_block) == 1 )
    {
        std::memmove( block_service_type::data(_block) , read_ptr() , readable_size());
        _write_pos = readable_size();
        _read_pos = 0;
        return;
    }
    block_type nb(block_service_type::create(readable_size() + size ));
    memcpy( block_service_type::data(nb) , read_ptr() , readable_size() );
    block_service_type::swap(_block,nb);
    _write_pos = readable_size();
    _read_pos = 0;
}
template < typename BlockServiceT >
std::size_t basic_byte_buffer<BlockServiceT>::consume( const std::size_t n ){
    return read_seek(n);
}

template < typename BlockServiceT >
std::size_t basic_byte_buffer<BlockServiceT>::commit( const std::size_t n ){
    return write_seek(n);
}

template < typename BlockServiceT >
void* basic_byte_buffer<BlockServiceT>::prepare( const std::size_t n ){
    reserve(n);
    return write_ptr();
}

}}