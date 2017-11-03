namespace codex { namespace buffer {

template < typename BufferT >
stream<BufferT>::stream( const std::size_t size )    
    : _buffer( size )
{
    
}

}}