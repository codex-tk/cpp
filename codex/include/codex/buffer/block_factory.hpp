#ifndef __codex_buffer_block_factory_h__
#define __codex_buffer_block_factory_h__

#include <codex/buffer/block.hpp>
#include <codex/util/rc_ptr.hpp>

#if defined( ENABLE_LIB_BOOST )
#include <boost/pool/pool_alloc.hpp>
#endif

namespace codex { namespace buffer {

template < typename AllocatorT = 
#if defined( ENABLE_LIB_BOOST )
    boost::pool_allocator<uint8_t>
#else
    std::allocator<uint8_t> 
#endif
>
class block_factory {
public:
    typedef block<block_factory> block_type;
public:
    static block_type* create( const std::size_t size )
    {
        block_type* ptr = reinterpret_cast< block_type* >(
            _allocator.allocate( sizeof(block_type) + size ));
        new (ptr) block_type( size );
        return ptr;
    }
private:
    friend class block<block_factory>;
    static void release( block_type* ptr )
    {
        _allocator.deallocate( reinterpret_cast< uint8_t*>(ptr) , 
        sizeof( block<block_factory> ) + ptr->size() );
    }
private:
    static AllocatorT _allocator; 
};

template < typename AllocatorT >
AllocatorT block_factory<AllocatorT>::_allocator;

typedef block_factory<> default_block_factory_service; 

// todo ref block factory

}}

#endif