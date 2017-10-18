#ifndef __codex_base_rc_ptr_h__
#define __codex_base_rc_ptr_h__

#include <atomic>

namespace codex {

template < typename T_object , typename T_counter  = std::atomic< int > >
class rc_ptr_base {
protected:
	rc_ptr_base( void ) {
		_counter.store(0);
	}
	~rc_ptr_base( void ){
	}
public:
	int retain( void ) {
		return ++_counter;
	}
	
	int release( void ) {
		int val = --_counter;
		if ( val == 0 ) {
			T_object* type = static_cast< T_object* >( this );
			delete type;
		}
		return val;
	}
private:
	T_counter _counter;
};
	
	
template < 
    typename T_object
>
class rc_ptr {
public:
    explicit rc_ptr( T_object* p = nullptr ) 
        : _ptr( p ){
        retain();
    }

    rc_ptr( const rc_ptr& rhs ) 
        : _ptr( rhs.get()){
        retain();
    }

	template < class other_t >
    rc_ptr( const rc_ptr<other_t>& rhs ) 
        : _ptr(rhs.get()){
        retain();
    }

#if ( _MSC_VER >= 1600 ) 
	rc_ptr( rc_ptr&& rhs ) 
		: _ptr( rhs.get() ) {
		rhs._ptr = nullptr;
	}

	template < class other_t >
    rc_ptr( rc_ptr<other_t>&& rhs ) 
        : _ptr(rhs.get()){
        rhs._ptr = nullptr;
    }

#endif    
    ~rc_ptr( void ) {
        release();
    }

    void release( void ) {
        if ( _ptr ){
			if ( _ptr->release() == 0 ) {
				_ptr = nullptr;
			}
        }
    }

    void retain( void ) {
        if ( _ptr ) {
            _ptr->retain();
        }
    }

    T_object* operator->(void) const {
        //CODEX_ASSERT( _ptr != nullptr );
        return get();
    }

    T_object& operator*(void) const {
        //CODEX_ASSERT( _ptr != nullptr );
        return *get();
    }

    T_object* get(void) const {
        return _ptr;
    }

    void attach( T_object* t ) {
        release();
        _ptr = t;
    }

    T_object* detach( void ) {
        T_object* old = _ptr;
        _ptr = nullptr;
        return old;
    }

    void reset( T_object* t = nullptr ) {
        rc_ptr new_ptr( t );
        swap( new_ptr );
    }

    void swap( rc_ptr& other ) {
        T_object* t = other._ptr;
        other._ptr = _ptr;
        _ptr = t;
    }

    rc_ptr& operator=( T_object* t ) {
        rc_ptr newPtr( t );
        swap( newPtr );
        return *this;
    }

    rc_ptr& operator=( const rc_ptr& rhs ) {
        rc_ptr newPtr( rhs );
        swap( newPtr );
        return *this;
    }
#if ( _MSC_VER >= 1600 ) 
	// �ڱ� ������ std::move() �� �̿��Ҷ��� �����ϹǷ� , ����ڰ� �˾Ƽ� ó��~
	rc_ptr& operator=( rc_ptr&& rhs ) {
		release();
		_ptr = rhs.get();    
		rhs._ptr = nullptr;
		return *this;
    }
#endif	

    template < typename OtherT >
    rc_ptr& operator=( const rc_ptr<OtherT>& rhs ) {
        return this->operator=( rhs.get());
    }

    bool unique( void ) const {
        if ( _ptr ) {
            return _ptr->ref_count() == 1;
        }
        return false;
    }
private:
    T_object* _ptr;
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
