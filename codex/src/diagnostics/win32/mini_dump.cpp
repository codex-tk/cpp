
#include <codex/diagnostics/win32/mini_dump.hpp>
#if defined ( __codex_win32__ )
#include <thread>
#include <codex/util/singleton.hpp>

namespace codex { namespace diagnostics {
namespace detail {

struct context {
	MINIDUMP_TYPE type;
	std::wstring output_path;
};

typedef BOOL (WINAPI *pf_write_dump)(
	HANDLE hProcess, 
	DWORD dwPid, 
	HANDLE hFile, 
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION	CallbackParam
);

pf_write_dump wite_dump_impl = nullptr;

static void __stdcall write_dump( EXCEPTION_POINTERS *pExp ){
	if ( wite_dump_impl == nullptr ) {
		return;
	}

    time_t ts = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm tm = *localtime(&ts);
	
	wchar_t output[1024] = {0};
    wsprintf( output , 
				L"%s\\%04d%02d%02d_%02d%02d%02d_ProcessId_%d.dmp" , 
				codex::singleton< detail::context >::instance().output_path.c_str() ,
				tm.tm_year + 1900 , tm.tm_mon + 1 , tm.tm_mday , 
				tm.tm_hour , tm.tm_min , tm.tm_sec
				GetCurrentProcessId());

	HANDLE file = CreateFileW(  output , 
								GENERIC_READ | GENERIC_WRITE, 
								0, 
								NULL, 
								CREATE_ALWAYS, 
								FILE_ATTRIBUTE_NORMAL, 
								NULL );

	if ( file == INVALID_HANDLE_VALUE ) {
		return false;
	}
	
	MINIDUMP_EXCEPTION_INFORMATION ExInfo;

	ExInfo.ThreadId           = GetCurrentThreadId(); 
	ExInfo.ExceptionPointers  = pExp; 
	ExInfo.ClientPointers     = FALSE; 

	BOOL ret = wite_dump_impl(GetCurrentProcess(), 
				GetCurrentProcessId(), 
				file, 
				codex::singleton< detail::context >::instance().dump_type , 
				&ExInfo , 
				NULL , 
				NULL );

	CloseHandle( file );
	return ret == TRUE;
}

static LONG __stdcall handle_exception( EXCEPTION_POINTERS *pExp ) {
	if ( pExp->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW ) {
		std::thread thr([pExp](){
			write_dump(pExp);
		});
		thr.join();
	} else {
		write_dump(pExp);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

}

mini_dump::mini_dump( void )
	: _dump_type(MiniDumpNormal)
	, _output_path( "" )
{
	if ( detail::wite_dump_impl == nullptr ) {
		HMODULE dbg_help_dll = LoadLibrary( _T("DBGHELP.DLL"));
		if ( dbg_help_dll != nullptr ) {
			detail::wite_dump_impl = reinterpret_cast< pf_write_dump  >( 
										GetProcAddress( dbg_help_dll , "MiniDumpWriteDump" ));
		}
	}
	wchar_t path[MAX_PATH];
	GetModuleFileNameW( NULL, path , MAX_PATH * sizeof(wchar_t)); 
	wchar_t *module_path = wcsrchr( path , '\\' );
	*module_path = 0;
	_output_path = path;
}

mini_dump::~mini_dump(void)
{
}

mini_dump& mini_dump::type( MINIDUMP_TYPE dump_type ) {
	_dump_type = dump_type;
	return *this;
}

mini_dump& mini_dump::output_path( const std::wstring& path ){
	_output_path = path;
	return *this;
}

bool mini_dump::install( void ){
	if ( detail::wite_dump_impl == nullptr ) 
		return false;
	codex::singleton< detail::context >::instance().dump_type = _dump_type;
	codex::singleton< detail::context >::instance().output_path = _output_path;
	SetUnhandledExceptionFilter( &detail::handle_exception );
	return true;
}

}}

#endif