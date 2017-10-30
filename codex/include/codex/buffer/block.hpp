#ifndef __codex_buffer_block_h__
#define __codex_buffer_block_h__

#include <atomic>
#include <memory>

namespace codex { namespace buffer {

template < typename AllocatorT  = std::allocator<uint8_t> >
class block {
public:
    explicit block( const std::size_t size  ) 
        : _refcount(0) , _size(size) {
    }

    ~block( void ) {
    }

    int addref( void ) {
        int rc = _refcount.fetch_add(1);
        return rc + 1;
    }

    int release( void ) {
        int rc = _refcount.fetch_sub(1);
        if ( rc == 1 ){
            AllocatorT allocator;
            allocator.deallocate( reinterpret_cast< uint8_t*>(this)
                 , sizeof( block ) + _size );
        }
        return rc - 1;
    }
    
    int refcount( void ) {
        return _refcount.load();
    }

    std::size_t size( void ) const noexcept {
        return _size;
    }

    void* data( void ) {
        return this + 1;
    }
private:
    std::atomic<int> _refcount;
    std::size_t _size;
};

}}

#endif