#ifndef __codex_config_IniKey_h__
#define __codex_config_IniKey_h__

namespace codex {
namespace util {

class ini_section;

class ini_key{
public:
    ini_key( const ini_section& section ,const codex::tstring& key );
    ~ini_key(void);

	codex::tstring to_string();
    int          to_int();

    void operator=( const codex::tstring& value );
    void operator=( const int value );
private:
	const ini_section& _section;
    codex::tstring _key;    
};

}}


#endif