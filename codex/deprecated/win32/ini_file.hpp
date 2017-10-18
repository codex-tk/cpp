#ifndef __codex_util_ini_file_h__
#define __codex_util_ini_file_h__

#include <codex/util/win32/ini_section.hpp>
namespace codex {
namespace util {

class ini_file{
public:
    ini_file( const codex::tstring& filename , const codex::tstring& defstring = _T("")  , const int defint = -1);
    ~ini_file(void);

    ini_section operator[]( const codex::tstring& section );

    const codex::tstring& name( void ) const;

    bool is_exist( void ) const;

	template < typename T >
	T default(void) const ;

	template <>
	int default<int>( void ) const {
		return _def_int;
	}

	template <>
	codex::tstring default<codex::tstring>() const {
		return _def_string;
	}

private:
	codex::tstring _file_name;
    codex::tstring _def_string;
    int          _def_int;
};

}}

#endif
