#ifndef __codex_function_h__
#define __codex_function_h__

#include <memory>
#include <codex/callable.hpp>

namespace codex {

template < typename Signature >
class function;

template < typename R , typename ... Args >
class function < R ( Args... ) >{
public:
    /*! default construct */
    function( void );
    /*! copy construct */
    function( const function& rhs );
    /*! 
        function< R (Args...) > lhs( ... );
        if ( not exist  function( function& rhs ) ) 
            // match -> template < typename HandlerT > function( HandlerT&& handler );
            function< R (Args...) > rhs(lhs); 
    */
    function( function& rhs );
    /*! move construct */
    function( function&& rhs );

    /*! ctor from callable */
    template < typename HandlerT >
    function( HandlerT&& handler );
    
    /*! typically assign operation */
    function& operator=( const function& rhs );

    /*! 
        function< R (Args...) > lhs( ... );
        function< R (Args...) > rhs( ... );
        if ( not exist  operator=( function& rhs ) ) 
            // match -> template < typename HandlerT > function& operator=( HandlerT&& handler );
            lhs = rhs; 
    */
    function& operator=( function& rhs );

    /*! move assign */
    function& operator=( function&& rhs );

    /*! assign from callable */
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