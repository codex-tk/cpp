#include "StdAfx.h"
#include <codex/util/win32/ini_key.hpp>
#include <codex/util/win32/ini_section.hpp>
#include <codex/util/win32/ini_file.hpp>

namespace codex {
namespace util {
namespace {

static const int k_ini_buffer_size = 1024;

}

ini_key::ini_key( const ini_section& section ,const codex::tstring& key )
    : _section( section ) , _key(key) {

}

ini_key::~ini_key(void) {

}

codex::tstring ini_key::to_string() {
    TCHAR buffer[k_ini_buffer_size];
    GetPrivateProfileString( _section.name().c_str() , 
                             _key.c_str() , 
                             _section.file().default<codex::tstring>().c_str() , 
                             buffer , 
                             k_ini_buffer_size , 
                             _section.file().name().c_str());
    return codex::tstring( buffer );
}

int ini_key::to_int() {
    return GetPrivateProfileInt( _section.name().c_str() , 
                                 _key.c_str() , 
                                 _section.file().default<int>()  , 
                                 _section.file().name().c_str());
}

void ini_key::operator=( const codex::tstring& value ) {
    WritePrivateProfileStringW(  _section.name().c_str() , 
                                _key.c_str()  , 
                                value.c_str() , 
                                _section.file().name().c_str() );
}

void ini_key::operator=( const int value ) {
	TCHAR buffer[k_ini_buffer_size];
    _stprintf_s( buffer , _T("%d") , value );
    ini_key::operator=( buffer );
}


}}