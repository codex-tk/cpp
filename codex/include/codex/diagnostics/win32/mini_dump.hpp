#ifndef __codex_diagnostics_mini_dump_h__
#define __codex_diagnostics_mini_dump_h__

#if defined ( __codex_win32__ )

#include <codex.hpp>
#include <DbgHelp.h>

namespace codex { namespace diagnostics {
/*!
	mini_dump()
	.type( MiniDumpNormal )
	.output_path( "c:\\dumps\\" )
	.install();
*/
class mini_dump{
public:
	mini_dump(void);
	~mini_dump(void);

	mini_dump& type( MINIDUMP_TYPE dump_type );
	mini_dump& output_path( const std::wstring& path );
	bool install( void );
private:
	std::wstring _output_path;
	MINIDUMP_TYPE _dump_type;
};

}}

#endif

#endif