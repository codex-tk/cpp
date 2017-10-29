#ifndef __codex_function_h__
#define __codex_function_h__

#include <memory>
#include <codex/callable.hpp>
#include <codex/util/method_calls.hpp>
#include <codex/util/singleton.hpp>

namespace codex {

constexpr int function_ctor = 0;
constexpr int function_copy_ctor = 1;
constexpr int function_copy_ctor_l = 2;
constexpr int function_move_ctor = 3;
constexpr int function_callable_ctor = 4;

constexpr int function_assign = 5;
constexpr int function_assign_l = 6;
constexpr int function_move_assign = 7;
constexpr int function_callable_assign = 8;

template < typename Signature >
class function;

template < typename R , typename ... Args >
class function < R ( Args... ) >{
public:
    /*! default construct */
    function( void );
    /*! copy construct */
    function( const function& rhs );
    /*! copy construct */
    function( function& rhs );
    /*! move construct */
    function( function&& rhs );

    /*! callable construct */
    template < typename HandlerT >
    function( HandlerT&& handler );
    
    /*! */
    function& operator=( const function& rhs );

    /*! */
    function& operator=( function& rhs );

    /*! */
    function& operator=( function&& rhs );

    /*! */
    template < typename HandlerT >
    function& operator=( HandlerT&& handler );
    
    /*! */
    R operator()( Args&& ... args ) const ;

    /*! */
    void swap( function& rhs );
private:
    std::shared_ptr< codex::callable< R ( Args... ) >> _callable;
};

}

#include <codex/function.ipp>

#endif