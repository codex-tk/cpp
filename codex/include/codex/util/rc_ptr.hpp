#ifndef __codex_rc_ptr_h__
#define __codex_rc_ptr_h__

#include <atomic>

namespace codex {
    
template < typename TypeT >
class rc_ptr {
public:
    explicit rc_ptr( TypeT* ptr = nullptr ) 
        : _ptr( ptr ){
        addref();
    }

    rc_ptr( const rc_ptr& rhs ) 
        : _ptr( rhs.get()){
        addref();
    }

    rc_ptr( rc_ptr&& rhs ) 
        : _ptr( rhs.get()){
        rhs._ptr = nullptr;
    }

	template < class OtherT >
    rc_ptr( const rc_ptr<OtherT>& rhs ) 
        : _ptr(rhs.get()){
        addref();
    }

    template < class OtherT >
    rc_ptr( rc_ptr<OtherT>&& rhs ) 
        : _ptr(rhs.get()){
        rhs._ptr = nullptr;
    }

    ~rc_ptr( void ) {
        release();
    }

    void release( void ) {
        if ( _ptr && _ptr->release() == 0 ){
			_ptr = nullptr;
        }
    }

    void addref( void ) {
        if ( _ptr ) {
            _ptr->addref();
        }
    }

    TypeT* operator->(void) const {
        return get();
    }

    TypeT& operator*(void) const {
        return *get();
    }

    TypeT* get(void) const {
        return _ptr;
    }
/*
    void attach( TypeT* t ) {
        release();
        _ptr = t;
    }*/

    TypeT* detach( void ) {
        TypeT* old = _ptr;
        _ptr = nullptr;
        return old;
    }

    void reset( TypeT* t = nullptr ) {
        rc_ptr new_ptr( t );
        swap( new_ptr );
    }

    void swap( rc_ptr& other ) {
        TypeT* t = other._ptr;
        other._ptr = _ptr;
        _ptr = t;
    }

    rc_ptr& operator=( TypeT* t ) {
        rc_ptr nptr( t );
        swap( nptr );
        return *this;
    }

    rc_ptr& operator=( const rc_ptr& rhs ) {
        rc_ptr nptr( rhs );
        swap( nptr );
        return *this;
    }

    rc_ptr& operator=( rc_ptr&& rhs ) {
        rc_ptr nptr( std::move(rhs));
        swap( nptr );
        rhs._ptr = nullptr;
        return *this;
    }

    template < typename OtherT >
    rc_ptr& operator=( const rc_ptr<OtherT>& rhs ) {
        return this->operator=( rhs.get());
    }

    template < typename OtherT >
    rc_ptr& operator=( rc_ptr<OtherT>&& rhs ) {
        rc_ptr nptr( std::move(rhs));
        swap( nptr );
        rhs._ptr = nullptr;
        return *this;
    }

    bool unique( void ) const {
        if ( _ptr ) {
            return _ptr->refcount() == 1;
        }
        return false;
    }
private:
    TypeT* _ptr;
};

template < typename T >
bool operator==( const rc_ptr<T>& lhs , const rc_ptr<T>& rhs ) {
    return lhs.get() == rhs.get();
}

template < typename T >
bool operator!=( const rc_ptr<T>& lhs , const rc_ptr<T>& rhs ) {
    return lhs.get() != rhs.get();
}

template < typename T >
bool operator<( const rc_ptr<T>& lhs , const rc_ptr<T>& rhs ) {
    return lhs.get() < rhs.get();
}

template < typename T >
bool operator>( const rc_ptr<T>& lhs , const rc_ptr<T>& rhs ) {
    return lhs.get() > rhs.get();
}

}

#endif
