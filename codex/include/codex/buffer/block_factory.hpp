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
    typedef codex::rc_ptr<block<AllocatorT>> block_ptr;
    static block_ptr create( const std::size_t size ){
        AllocatorT allocator;
        block<AllocatorT>* ptr = reinterpret_cast< block<AllocatorT>* >(
            allocator.allocate( sizeof(block<AllocatorT>) + size ));
        new (ptr) block<AllocatorT>( size );
        return block_ptr(ptr);
    }
};

typedef block_factory<> default_block_factory; 

}}

#endif