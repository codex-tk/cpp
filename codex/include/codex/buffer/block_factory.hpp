#ifndef __codex_buffer_block_factory_h__
#define __codex_buffer_block_factory_h__

#include <codex/buffer/block.hpp>
#include <codex/util/rc_ptr.hpp>

namespace codex { namespace buffer {
    
template < typename AllocatorT  = std::allocator<uint8_t> >
class block_factory {
public:
    static codex::rc_ptr<block<AllocatorT>> create( const std::size_t size ){
        AllocatorT allocator;
        block<AllocatorT>* ptr = reinterpret_cast< block<AllocatorT>* >(
            allocator.allocate( sizeof(block<AllocatorT>) + size ));
        new (ptr) block<AllocatorT>( size );
        return codex::rc_ptr<block<AllocatorT>>(ptr);
    }
};

}}



#endif