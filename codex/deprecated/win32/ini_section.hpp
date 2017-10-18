#ifndef __codex_util_ini_section_h__
#define __codex_util_ini_section_h__

#include <codex/util/win32/ini_key.hpp>

namespace codex {
namespace util {
class ini_file;
class ini_section{
public:    
    ini_section( const ini_file& file , const codex::tstring& section );
    ~ini_section(void);

	ini_key operator[]( const codex::tstring& key );

    const ini_file& file( void ) const;
    const codex::tstring& name( void ) const;    	
private:
    const ini_file& _ini_file;
    codex::tstring _section;
};


}}

#endif