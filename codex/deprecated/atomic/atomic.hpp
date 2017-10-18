#ifndef __tck_threading_atomic_h__
#define __tck_threading_atomic_h__

#include <codex/codex.hpp>
#include <codex/threading/atomic/memory_order.hpp>
#include <atomic>

namespace codex {
namespace threading {

/*
	acquire / release sementics

	An operation has acquire semantics if other processors will 
		always see its effect before any subsequent operation's effect. 
	An operation has release semantics if other processors will 
		see every preceding operation's effect before the effect of the operation itself.

	acquire - guarantees that memory reads can't jump over the barrier

	InterlockedIncrementAcquire(&a);
	b++;
	c++;

	other processors would always see the increment of a before the increments of b and c.

	release - guarantees that memory writes can't jump over the barrier

	a++;
	b++;
	InterlockedIncrementRelease(&c);

	other processors would always see the increments of a and b before the increment of c.

*/

/*
	atomic operation
	windows assembly & Interlocked impl
	linux  gcc impl
	ios OSAtomic impl * check
*/


namespace detail {
/*
http://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
*/
	/*
template < int memory_model >
struct memory_model_impl {
	enum { value = memory_model_impl };
};

class atomic {
public:
#if defined( _WIN32 )
	#if defined( _M_X64 )
		typedef volatile int64_t value_type;
	#else
		typedef volatile int32_t value_type;
	#endif
#else
	#if defined(_M_X64) || defined(__x86_64__)
		typedef volatile int64_t value_type;
	#else
		typedef volatile int32_t value_type;
	#endif
#endif

	template < typename model >
	void barrier( void ) {

	}

	void load( void );
	void store( void );
	void exchange( void );
	bool compare_exchange( void );
	void add_fetch(void);
	void sub_fetch(void);

	void fetch_add( void );
	void fetch_sub( void );


	test_and_set();
	clear();
	thread_fence();
	signal_fence();
	always_lock_free();
	is_lock_free();
private:

	value_type _value;
};
*/
}

	
class atomic32 {
public:
	typedef volatile int32_t value_type;
	static void		  __stdcall barrier( void );
	
	static value_type __stdcall increment( value_type* value );
	static value_type __stdcall decrement( value_type* value );
	static value_type __stdcall exchange( value_type* value , value_type change );
	static value_type __stdcall compare_and_swap( value_type* value , value_type change , value_type compare );
	static bool		  __stdcall compare_and_swap_bool(  value_type* value , value_type change , value_type compare );
	static value_type __stdcall add( value_type* value , value_type add );
	/*
	static value_type __stdcall fetch_and_add (value_type *ptr, value_type value );
	static value_type __stdcall fetch_and_sub (value_type *ptr, value_type value );
	static value_type __stdcall add_and_fetch (value_type *ptr, value_type value );
	static value_type __stdcall sub_and_fetch (value_type *ptr, value_type value );
	static value_type __stdcall fetch_and_set(value_type *ptr, value_type value);

	
	static value_type __stdcall fetch_and_add (value_type *ptr, value_type value );
	static value_type __stdcall fetch_and_sub (value_type *ptr, value_type value );
	static value_type __stdcall fetch_and_or  (value_type *ptr, value_type value );
	static value_type __stdcall fetch_and_and (value_type *ptr, value_type value );
	static value_type __stdcall fetch_and_xor (value_type *ptr, value_type value );
	static value_type __stdcall fetch_and_nand(value_type *ptr, value_type value );
																				 
	static value_type __stdcall add_and_fetch (value_type *ptr, value_type value );
	static value_type __stdcall sub_and_fetch (value_type *ptr, value_type value );
	static value_type __stdcall or_and_fetch  (value_type *ptr, value_type value );
	static value_type __stdcall and_and_fetch (value_type *ptr, value_type value );
	static value_type __stdcall xor_and_fetch (value_type *ptr, value_type value );
	static value_type __stdcall nand_and_fetch(value_type *ptr, value_type value );

	static value_type __stdcall compare_and_swap( value_type* value , value_type change , value_type compare );
	static bool		  __stdcall compare_and_swap_bool(  value_type* value , value_type change , value_type compare );
	
	static value_type __stdcall fetch_and_set(value_type *ptr, value_type value); */
};

/*
	lock free container �뵵�� �ۼ�.
	����� ������ ���
	assembly �� ����
	cmpxchg8b �� ������ cas �Լ� ���̽��� �۵���
	�̿��� �÷����� ���� ��
*/

class atomic64 {
public:	
	typedef volatile int64_t value_type;
	static void		  __stdcall barrier( void );
	static value_type __stdcall increment( value_type* value );
	static value_type __stdcall decrement( value_type* value );
	static value_type __stdcall exchange( value_type* value , value_type change );
	static value_type __stdcall compare_and_swap( value_type* value , value_type change , value_type compare );
	static bool		  __stdcall compare_and_swap_bool(  value_type* value , value_type change , value_type compare );
	static value_type __stdcall add( value_type* value , value_type add );
};

template < typename T_type > 
struct atomic_impl;

template<> struct atomic_impl< atomic32::value_type > : public atomic32 {};
template<> struct atomic_impl< atomic64::value_type > : public atomic64 {};

template < int >
struct atomic_trait {
	typedef atomic32::value_type value_type;
};

template <> struct atomic_trait<5> {
	typedef atomic64::value_type value_type;
};

template <> struct atomic_trait<6> {
	typedef atomic64::value_type value_type;
};

template <> struct atomic_trait<7> {
	typedef atomic64::value_type value_type;
};

template <> struct atomic_trait<8> {
	typedef atomic64::value_type value_type;
};

template < typename T >
struct remove_volatile {
	typedef T type;
};

template < typename T >
struct remove_volatile< volatile T  >{
	typedef T type;
};

template < typename T_object >
class atomic {
public:
	typedef typename atomic_trait< sizeof(T_object) >::value_type value_type;

	atomic( void ) : _value( 0 ){
		STATIC_CHECK( sizeof( T_object ) <= sizeof( int64_t ) , atomic_type_too_big );
	}

	explicit atomic( const T_object& val ): _value(val) {
		STATIC_CHECK( sizeof( T_object ) <= sizeof( int64_t ) , atomic_type_too_big );
	}

	T_object load( void ) const {
		return *static_cast< value_type* >( const_cast< value_type* >(&_value ));
	}

	template < typename T_memory_order >
	T_object load( T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return load();
	}

	void store( const T_object& obj ) {
		_value = obj;
	}

	template < typename T_memory_order >
	void store( const T_object& obj , T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		store( obj );
	}
	
	T_object increment( void ) {
		return atomic_impl< value_type >::increment( &_value );
	}

	template < typename T_memory_order >
	T_object increment( T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return increment();
	}

	T_object decrement( void ) {
		return atomic_impl< value_type >::decrement( &_value );
	}

	template < typename T_memory_order >
	T_object decrement( T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return decrement();
	}

	T_object exchange( T_object change ) {
		return atomic_impl< value_type >::exchange( &_value , change );
	}

	T_object compare_and_swap( T_object change , T_object compare ) {
		return atomic_impl< value_type >::compare_and_swap( &_value , change , compare );
	}

	T_object add( T_object a ) {
		return atomic_impl< value_type >::add( &_value , a );
	}

	T_object _or( T_object a ) {
		T_object old;
		do {
			old = load();
		} while ( compare_and_swap( old | a , old ) != old );
		return old;
	}
	
	T_object _and( T_object a ) {
		T_object old;
		do {
			old = load();
		} while ( compare_and_swap( old & a , old ) != old );
		return old;
	}
	

	template < typename T_memory_order >
	T_object exchange( T_object change , T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return exchange( change );
	}

	template < typename T_memory_order >
	T_object compare_and_swap( T_object change , T_object compare , T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return compare_and_swap( change , compare );
	}

	template < typename T_memory_order >
	T_object add( T_object a , T_memory_order& ord ) {
		auto_barrier< T_memory_order > barrier( ord );
		return add( a );
	}

	void barrier( void ) {
		atomic_impl< value_type >::barrier();
	}

#if defined ( _MSC_VER )
	__declspec(align(8)) value_type _value;
#else
	value_type _value __attribute__((aligned(8)));
#endif
};

}
}

#endif
