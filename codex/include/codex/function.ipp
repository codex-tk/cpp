#define CODEX_FUNCTION_TEST

#include <codex/util/singleton.hpp>

#if defined( CODEX_FUNCTION_TEST )
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

template < typename TypeT >
class method_calls{
public:
    method_calls( void ) = default;
    ~method_calls( void ) = default;
    void call( int ptr ) {
        if ( _calls.find(ptr) == _calls.end() )
            _calls[ptr] = 0;
        _calls[ptr] = _calls[ptr] + 1;
    }
    int call_counts( int ptr ) { return _calls.find(ptr) == _calls.end() ? 0 : _calls[ptr]; }
    void reset( void ) { _calls.clear(); }
private:
    std::map< int , int > _calls;
};

}
#define CALL_RECORD( arg ) codex::singleton< method_calls< function > >::instance().call( arg );
#else
#define CALL_RECORD( arg )
#endif
namespace codex {
namespace {
    template < typename HandlerT , typename R , typename ... Args >
    class callable0 : public callable< R ( Args... ) > {
    public:
        callable0( HandlerT&& h ) 
            : _handler( std::forward< HandlerT >(h))
        {
        }

        virtual ~callable0( void ) override {
        }

        virtual R operator()( Args&& ... args ) override {
            return _handler( std::forward< Args >(args)...);
        }
    private:
        HandlerT _handler;
    };
}

template < typename R , typename ... Args >
function < R ( Args... ) >::function( void )
{
    CALL_RECORD( function_ctor );
}

template < typename R , typename ... Args >
function < R ( Args... ) >::function( const function& rhs )
    : _callable( rhs._callable )
{
    CALL_RECORD( function_copy_ctor );
}

template < typename R , typename ... Args >
function < R ( Args... ) >::function( function& rhs )
    : _callable( rhs._callable )
{
    CALL_RECORD( function_copy_ctor_l );
}

template < typename R , typename ... Args >
function < R ( Args... ) >::function( function&& rhs )
    : _callable( std::move(rhs._callable) )
{
    CALL_RECORD( function_move_ctor );
}



template < typename R , typename ... Args >
template < typename HandlerT >
function < R ( Args... ) >::function( HandlerT&& handler )
    : _callable( std::make_shared<callable0<HandlerT , R , Args... > >( 
        std::forward< HandlerT>(handler))) {
    CALL_RECORD( function_callable_ctor );
}

template < typename R , typename ... Args >
function < R ( Args... ) >& function < R ( Args... ) >::operator=( const function& rhs ){
    CALL_RECORD( function_assign );
    _callable = rhs._callable;
    return *this;
}

template < typename R , typename ... Args >
function < R ( Args... ) >& function < R ( Args... ) >::operator=( function& rhs ){
    CALL_RECORD( function_assign_l );
    _callable = rhs._callable;
    return *this;
}

template < typename R , typename ... Args >
function < R ( Args... ) >& function < R ( Args... ) >::operator=( function&& rhs ){
    CALL_RECORD( function_move_assign );
    _callable = std::move(rhs._callable);
    return *this;
}

template < typename R , typename ... Args >
template < typename HandlerT >
function < R ( Args... ) >& function < R ( Args... ) >::operator=( HandlerT&& handler ){
    CALL_RECORD( function_callable_assign );
    _callable = std::make_shared<callable0<HandlerT , R , Args... >  >( 
        std::forward< HandlerT>(handler));
    return *this;
}

template < typename R , typename ... Args >
R function < R ( Args... ) >::operator()( Args&& ... args ) const {
    if ( _callable ) {
        return (*_callable)( std::forward< Args >( args )... );
    }
    return R();
}

template < typename R , typename ... Args >
void function < R ( Args... ) >::swap( function& rhs ){
    _callable.swap(rhs._callable);
}

}