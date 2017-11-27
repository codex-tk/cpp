#include <algorithm>

namespace codex { namespace buffer {

template < typename BlockFactoryServiceT >
basic_byte_buffer<BlockFactoryServiceT>::basic_byte_buffer( const std::size_t size )    
    : _block_ptr( BlockFactoryServiceT::create(size))
    , _read_pos(0)
    , _write_pos(0)
{
}

template < typename BlockFactoryServiceT >
basic_byte_buffer<BlockFactoryServiceT>::basic_byte_buffer( const basic_byte_buffer& rhs )
    : _block_ptr( rhs._block_ptr )
    , _read_pos( rhs._read_pos )
    , _write_pos( rhs._write_pos )
{

}

template < typename BlockFactoryServiceT >
basic_byte_buffer<BlockFactoryServiceT>::basic_byte_buffer( basic_byte_buffer&& rhs )    
    : _block_ptr( std::move(rhs._block_ptr) )
    , _read_pos( rhs._read_pos )
    , _write_pos( rhs._write_pos )
{
    rhs._read_pos = rhs._write_pos = 0;
}

template < typename BlockFactoryServiceT >
basic_byte_buffer<BlockFactoryServiceT>& basic_byte_buffer<BlockFactoryServiceT>::operator=( const basic_byte_buffer& rhs ){
    basic_byte_buffer nbb( rhs );  
    swap( nbb );
    return *this;
}

template < typename BlockFactoryServiceT >
basic_byte_buffer<BlockFactoryServiceT>& basic_byte_buffer<BlockFactoryServiceT>::operator=( basic_byte_buffer&& rhs ){
    basic_byte_buffer nbb( std::move(rhs) );  
    swap( nbb );
    return *this;
}

template < typename BlockFactoryServiceT >
basic_byte_buffer<BlockFactoryServiceT>::~basic_byte_buffer( void ) {
}

template < typename BlockFactoryServiceT >
int basic_byte_buffer<BlockFactoryServiceT>::size( void ) const noexcept {
    return _block_ptr->size();
}

template < typename BlockFactoryServiceT >
int basic_byte_buffer<BlockFactoryServiceT>::readable_size( void ) const noexcept{
    return _write_pos - _read_pos;
}

template < typename BlockFactoryServiceT >
int basic_byte_buffer<BlockFactoryServiceT>::read_seek( int seek ) noexcept{
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

template < typename BlockFactoryServiceT >
void* basic_byte_buffer<BlockFactoryServiceT>::read_ptr( void ) const noexcept {
    return static_cast< uint8_t*>(_block_ptr->data()) + _read_pos;
}

template < typename BlockFactoryServiceT >
int basic_byte_buffer<BlockFactoryServiceT>::writable_size( void ) const noexcept{
    return size() - _write_pos;
}

template < typename BlockFactoryServiceT >
int basic_byte_buffer<BlockFactoryServiceT>::write_seek( int seek ) noexcept{
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

template < typename BlockFactoryServiceT >
void* basic_byte_buffer<BlockFactoryServiceT>::write_ptr( void ) const noexcept {
    return static_cast< uint8_t*>(_block_ptr->data()) + _write_pos;
}

template < typename BlockFactoryServiceT >
void basic_byte_buffer<BlockFactoryServiceT>::swap( basic_byte_buffer& rhs ) noexcept{
    _block_ptr.swap(rhs._block_ptr);
    std::swap( _read_pos , rhs._read_pos );
    std::swap( _write_pos , rhs._write_pos );
}

template < typename BlockFactoryServiceT >
typename basic_byte_buffer<BlockFactoryServiceT>::block_ptr_type& 
    basic_byte_buffer<BlockFactoryServiceT>::block_ptr( void ) 
{
    return _block_ptr;
}

template < typename BlockFactoryServiceT >
void basic_byte_buffer<BlockFactoryServiceT>::reserve( const std::size_t size ){
    if ( writable_size() >= static_cast<int>(size) ) {
        return;
    }
    if ( this->size() - readable_size() >= static_cast<int>(size) 
        && _block_ptr->refcount() == 1 )
    {
        std::memmove( _block_ptr->data() , read_ptr() , readable_size());
        _write_pos = readable_size();
        _read_pos = 0;
        return;
    }
    block_ptr_type nbptr(BlockFactoryServiceT::create(readable_size() + size ));
    memcpy( nbptr->data() , read_ptr() , readable_size() );
    _block_ptr.swap( nbptr );
    _write_pos = readable_size();
    _read_pos = 0;
}
template < typename BlockFactoryServiceT >
std::size_t basic_byte_buffer<BlockFactoryServiceT>::consume( const std::size_t n ){
    return read_seek(n);
}

template < typename BlockFactoryServiceT >
std::size_t basic_byte_buffer<BlockFactoryServiceT>::commit( const std::size_t n ){
    return write_seek(n);
}

template < typename BlockFactoryServiceT >
void* basic_byte_buffer<BlockFactoryServiceT>::prepare( const std::size_t n ){
    reserve(n);
    return write_ptr();
}

}}