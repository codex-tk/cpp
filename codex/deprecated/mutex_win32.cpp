#include "stdafx.h"
#include <codex/threading/mutex_win32.hpp>
#if defined( _WIN32 )
namespace codex {
namespace threading {

mutex::mutex( void )
	: _mutex(INVALID_HANDLE_VALUE){
}

mutex::~mutex( void ) {
	close();
}

bool mutex::create( const codex::tstring& name , bool initstate ) {
	_mutex = CreateMutex( NULL , initstate ? TRUE : FALSE , name.c_str() );
	if ( _mutex == NULL ) {
		codex::set_last_error( codex::platform::error());
		return false;
	}
	return true;
}

bool mutex::open( const codex::tstring& name ) {
	_mutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, name.c_str() );
	if ( _mutex == NULL ) {
		codex::set_last_error( codex::platform::error());
		return false;
	}
	return true;
}

void mutex::close( void ) {
	if ( _mutex != INVALID_HANDLE_VALUE ) {
		CloseHandle( _mutex );
	}
	_mutex = INVALID_HANDLE_VALUE;
}


bool mutex::wait( const codex::time_span& ts ) {
	if ( _mutex == INVALID_HANDLE_VALUE ) {
		codex::set_last_error( codex::platform::error( ERROR_INVALID_HANDLE ));
        return false;
    }
	int ret = ::WaitForSingleObject( _mutex , 
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

void mutex::release( void ) {
	ReleaseMutex( _mutex );
}

}}
#endif
