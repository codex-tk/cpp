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
}

template < typename R , typename ... Args >
function < R ( Args... ) >::function( const function& rhs )
    : _callable( rhs._callable )
{
}

template < typename R , typename ... Args >
function < R ( Args... ) >::function( function&& rhs )
    : _callable( std::move(rhs._callable) )
{
}

template < typename R , typename ... Args >
function < R ( Args... ) >::function( function& rhs )
    : _callable( rhs._callable )
{
}

template < typename R , typename ... Args >
template < typename HandlerT >
function < R ( Args... ) >::function( HandlerT&& handler )
    : _callable( std::make_shared<callable0<HandlerT , R , Args... > >( 
        std::forward< HandlerT>(handler))) {
}

template < typename R , typename ... Args >
function < R ( Args... ) >& function < R ( Args... ) >::operator=( const function& rhs ){
    _callable = rhs._callable;
    return *this;
}

template < typename R , typename ... Args >
function < R ( Args... ) >& function < R ( Args... ) >::operator=( function& rhs ){
    _callable = rhs._callable;
    return *this;
}

template < typename R , typename ... Args >
function < R ( Args... ) >& function < R ( Args... ) >::operator=( function&& rhs ){
    _callable = std::move(rhs._callable);
    return *this;
}

template < typename R , typename ... Args >
template < typename HandlerT >
function < R ( Args... ) >& function < R ( Args... ) >::operator=( HandlerT&& handler ){
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