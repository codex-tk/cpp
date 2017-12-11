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
    typedef codex::rc_ptr< block<block_factory> > block_type;
public:
    static block_type create( const std::size_t size )
    {
        block<block_factory>* ptr = reinterpret_cast< block<block_factory>* >(
            _allocator.allocate( sizeof(block<block_factory>) + size ));
        new (ptr) block<block_factory>( size );
        return block_type(ptr);
    }

    static int size( const block_type& value ) {
        return value->size();
    }
    static int use_count( block_type& value ) {
        return value->refcount();
    }
    static void* data( const block_type& value ) {
        return value->data();
    }
    static void swap( block_type& a , block_type& b ) {
        a.swap(b);
    }
private:
    friend class block<block_factory>;
    static void release( block<block_factory>* ptr )
    {
        _allocator.deallocate( reinterpret_cast< uint8_t*>(ptr) , 
        sizeof( block<block_factory> ) + ptr->size() );
    }
private:
    static AllocatorT _allocator; 
};

template < typename AllocatorT >
AllocatorT block_factory<AllocatorT>::_allocator;

template < typename AllocatorT = 
#if defined( ENABLE_LIB_BOOST )
    boost::pool_allocator<uint8_t>
#else
    std::allocator<uint8_t> 
#endif
>
class shared_ptr_block_service {
public:
    typedef std::shared_ptr< std::vector<uint8_t,AllocatorT>> block_type;
public:
    static block_type create( const std::size_t size )
    {
        return std::allocate_shared< std::vector<uint8_t,AllocatorT> >(
            _allocator 
            , size , static_cast< uint8_t >(0));
    }

    static int size( const block_type& value ) {
        return value->size();
    }
    static int use_count( block_type& value ) {
        return value.use_count();
    }
    static void* data( const block_type& value ) {
        return &((*value)[0]);
    }
    static void swap( block_type& a , block_type& b ) {
        a.swap(b);
    }
private:
    static AllocatorT _allocator; 
};

template < typename AllocatorT >
AllocatorT shared_ptr_block_service<AllocatorT>::_allocator;

typedef block_factory<> default_block_factory_service; 
typedef shared_ptr_block_service<> custom_block_service;

// todo ref block factory

}}

#endif