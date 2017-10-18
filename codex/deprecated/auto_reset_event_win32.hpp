#ifndef __codex_threading_auto_reset_event_h__
#define __codex_threading_auto_reset_event_h__
#if defined( _WIN32 )
#include <codex/time/time_span.hpp>

namespace codex {
namespace threading {

class auto_reset_event {
public:
	explicit auto_reset_event( bool init_state = false );
	~auto_reset_event( void ) ;

	bool wait( const codex::time_span& w );
	void set( void );
	void reset( void );
private:
	HANDLE _handle;
};

}}
#endif

#endif
