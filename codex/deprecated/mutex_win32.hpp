#ifndef __codex_threading_mutex_h__
#define __codex_threading_mutex_h__
#if defined( _WIN32 )
#include <codex/time/time_span.hpp>

namespace codex {
namespace threading {

class mutex {
public:
	mutex( void );
	~mutex( void );

	bool create( const codex::tstring& name , bool initstate = false );
	bool open( const codex::tstring& name );
	void close( void );

	bool wait( const codex::time_span& ts );
	void release( void );
private:
	HANDLE _mutex;
};

}}
#endif
#endif
