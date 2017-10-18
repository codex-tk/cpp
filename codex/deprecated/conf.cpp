#include "stdafx.h"
#include <codex/util/conf.hpp>
#include <string>
#include <iostream>
#include <codex/util/string.hpp>
#include <codex/error/error_platform.hpp>
#include <stdlib.h>
#include <vector>

namespace codex { 
namespace util {
conf::conf( const codex::tstring& file ) 
	: _filename( file ) {

}

conf::~conf(void) {
}

bool conf::load( void ) {
/*
	std::ifstream fs(
#if defned( __WIN32 )
			codex::string::wcs_to_mbs( _filename )
#else
		_filename
#endif
	, std::ios::in );
	if ( !fs ) {
#if defned( __WIN32 )
		codex::set_last_error( codex::platform::error( ERROR_FILE_INVALID ));
#endif
		return false;
	}*/
#if defined(_WIN32)
	FILE* fp = nullptr;
	_wfopen_s( &fp , _filename.c_str() , L"r");
#else
	FILE* fp = fopen( _filename.c_str() , "r");

	char * read_buffer = nullptr;
	int read_size = 0;
	size_t len = 0;
	while ( ( read_size = getline( &read_buffer , &len , fp)) != 0 ) {
		std::string line(read_buffer);
		if ( line.length() == 0 || line[0] == '#' || line.length() < 3 ) {
			continue;
		}		
		if ( line.length() > 3 ) {
			std::vector< std::string > out;
			codex::string::split( line , std::string("=") , std::back_inserter(out) );
			if ( out.size() == 2 ) {
				_value.insert( std::map< codex::tstring , codex::tstring >::value_type(
						out[0] , out[1] ));
/*
					codex::string::mbs_to_wcs( out[0] ) 
					, codex::string::mbs_to_wcs( out[1] )));
					*/
			}
		}
	}	
#endif
	
	return true;
}

codex::tstring conf::operator[]( const codex::tstring& str ) {
	std::map< codex::tstring , codex::tstring >::iterator it = _value.find( str  );
	if ( it == _value.end() ) {
		return nullptr;
	}
	return it->second;
}
}}
