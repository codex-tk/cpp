#include "stdafx.h"
#include <codex/util/win32/mini_dump_seh_handler.hpp>
#include <codex/time/date_time.hpp>
#include <thread>
//#include <codex/threading/thread.hpp>

namespace codex {
namespace util {

mini_dump_seh_handler::mini_dump_seh_handler( MINIDUMP_TYPE type )
	: _dump( type ) {

}

mini_dump_seh_handler::~mini_dump_seh_handler(void) {

}

void mini_dump_seh_handler::handle_exception( EXCEPTION_POINTERS *pExp ) {
	if ( pExp->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW ) {
		std::thread thr([ this,pExp](){
			_write_dump(pExp);
		});
		thr.join();//( codex::time_span::infinite() );
	} else {
		_write_dump( pExp );
	}
}


void mini_dump_seh_handler::_write_dump( EXCEPTION_POINTERS* pExp ) {
	wchar_t path[MAX_PATH];

	GetModuleFileNameW( NULL, path , MAX_PATH * sizeof(wchar_t)); 
    wchar_t *ext = wcsrchr( path , '.' );

	SYSTEMTIME st = codex::time::tick::to_systemtime(codex::date_time::local().time());

    wsprintf( ext ? ext : path + wcslen( path ) , 
				L"_%04d%02d%02d_%02d%02d%02d_ProcessId_%d.dmp" , 
				st.wYear , st.wMonth , st.wDay ,
				st.wHour , st.wMinute, st.wMinute ,
				GetCurrentProcessId());

	_dump.dump( std::wstring( path ) , pExp );
}

}}
