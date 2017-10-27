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