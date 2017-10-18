#include "StdAfx.h"
#include <codex/util/win32/ini_section.hpp>
namespace codex {
namespace util {

ini_section::ini_section( const ini_file& file , const codex::tstring& section )
	: _ini_file( file ) , _section( section ) {

}

ini_section::~ini_section(void) {

}

ini_key ini_section::operator[]( const codex::tstring& key ) {
    return ini_key( *this , key );
}

const ini_file& ini_section::file( void ) const {
    return _ini_file;
}

const codex::tstring& ini_section::name( void ) const {
    return _section;
}

}}