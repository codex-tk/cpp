#ifndef __codex_buffer_block_h__
#define __codex_buffer_block_h__

#include <atomic>
#include <memory>
namespace codex { namespace buffer {

template < typename FactoryServiceT >
class block {
public:
    explicit block( const std::size_t size  ) 
        : _refcount(0) , _size(size) {
    }

    ~block( void ) {}

    block( const block& rhs ) = delete;
    block& operator=( const block& rhs ) = delete;
    
    block( block&& rhs ) = delete;
    block& operator=( block&& rhs ) = delete;
        
    int refcount( void ) { return _refcount.load(); }
    std::size_t size( void ) const noexcept { return _size; }
    void* data( void ) const noexcept { return const_cast<block*>(this) + 1; }

    int addref( void ) {
        int rc = _refcount.fetch_add(1);
        return rc + 1;
    }

    int release( void ) {
        int rc = _refcount.fetch_sub(1) - 1;
        if ( rc == 0 )
            FactoryServiceT::release( this );
        return rc;
    }
private:
    std::atomic<int> _refcount;
    std::size_t _size;
};

}}

#endif