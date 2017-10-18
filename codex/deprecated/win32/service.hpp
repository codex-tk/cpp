#ifndef __caffe_win32_service_h__
#define __caffe_win32_service_h__

#include <codex/task/handler.hpp>

namespace codex {
namespace util {
	
class service {
public:
	service( const codex::tstring& svc_name );
	~service( void );

	typedef codex::taskex::handler< void ( service* svc ) > service_main_handler;
	typedef codex::taskex::handler< DWORD ( service* svc , int ctrl , int type , void* data )> service_event_handler;

	// properties
	codex::tstring name() const { return _name; }
	codex::tstring display_name() const{ return _display_name; }
	codex::tstring description() const{ return _description; }
	int desire_access() const{ return _desire_access; }
	int service_type() const{ return _service_type; }
	int start_type() const{ return _start_type; }
	int error_control() const{ return _error_control; }
	codex::tstring path() const { return _path; }

	//codex::tstring name( const codex::tstring& value ) { _name = value; }
	void display_name(const codex::tstring& value ) { _display_name = value; }
	void description(const codex::tstring& value ) { _description = value; }
	void desire_access( int value ) { _desire_access = value; }
	void service_type(int value ) { _service_type = value; }
	void start_type(int value ) { _start_type = value; }
	void error_control(int value ) { _error_control = value; }
	void path(const codex::tstring& value ) { _path = value; }

	bool start( const service_main_handler& main_handler , const service_event_handler& event_hander );

	void run( void );
	DWORD handle_event( DWORD fdwControl, DWORD dwEventType, LPVOID lpEventData );
	/*
	SERVICE_STOPPED         
	SERVICE_START_PENDING   
	SERVICE_STOP_PENDING    
	SERVICE_RUNNING         
	SERVICE_CONTINUE_PENDING
	SERVICE_PAUSE_PENDING   
	SERVICE_PAUSED          
	*/
	void change_status( DWORD state );


private:
	static void  __stdcall _service_main(  int args , wchar_t** argv  );
	static DWORD __stdcall _service_handler(DWORD fdwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext);
private:
	SERVICE_STATUS_HANDLE _status_handle;

	DWORD _current_status;

	service_main_handler _main_handler;
	service_event_handler _event_handler;

// properties
	codex::tstring _name;
	codex::tstring _display_name;
	codex::tstring _description;
	// GetModuleFileName
	codex::tstring _path;
	// SERVICE_ALL_ACCESS
	int _desire_access;
	// SERVICE_WIN32_OWN_PROCESS
	int _service_type;
	// SERVICE_AUTO_START
	int _start_type;
	// SERVICE_ERROR_IGNORE
	int _error_control;


};

}}

#endif