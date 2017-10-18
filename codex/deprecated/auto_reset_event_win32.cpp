#include "stdafx.h"
#include <codex/threading/auto_reset_event_win32.hpp>
#if defined( _WIN32 )
namespace codex {
namespace threading {

auto_reset_event::auto_reset_event( bool init_state )
	: _handle( CreateEvent( nullptr , false , init_state  , nullptr ) ) {
}

auto_reset_event::~auto_reset_event( void ) {
	CloseHandle( _handle );
	_handle = INVALID_HANDLE_VALUE;
}

bool auto_reset_event::wait( const codex::time_span& ts ) {
	if ( _handle == INVALID_HANDLE_VALUE ) {
		codex::set_last_error( codex::platform::error( ERROR_INVALID_HANDLE ));
        return false;
    }
	int ret = ::WaitForSingleObject( _handle , 
		static_cast< uint32_t >( ts.total_milli_seconds()));

	switch( ret ) {
    case WAIT_OBJECT_0:
		return true;
	case WAIT_FAILED:
        codex::set_last_error( codex::platform::error());
		return false;
	case WAIT_TIMEOUT:
		codex::set_last_error( codex::platform::timeout );
		return false;
	}
	codex::set_last_error( codex::platform::error(ret));
	return false;
}

void auto_reset_event::set( void ) {
	SetEvent( _handle );
}

void auto_reset_event::reset( void ) {
	ResetEvent( _handle );
}

}}
#endif
