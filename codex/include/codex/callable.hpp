#ifndef __codex_callable_h__
#define __codex_callable_h__

namespace codex {

/*!
*/
template < typename Signature >
class callable;

/*!
*/
template < typename R , typename ... Args >
class callable < R ( Args... ) >{
public:
    callable( void ) noexcept {
    }
    virtual ~callable( void ) noexcept {
    }
    virtual R operator()( Args&& ... arg ) = 0;
};

}

#endif