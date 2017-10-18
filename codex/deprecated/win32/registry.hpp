#ifndef __codex_config_registry_h__
#define __codex_config_registry_h__

#include <winreg.h>

namespace codex {
namespace util {

class registry{
public:
	registry( void );
	~registry(void);
	bool open( HKEY root , const codex::tstring& subkey  );
	//bool open( HKEY root , const codex::tstring& subkey , REGSAM desired = KEY_ALL_ACCESS );
	void close( void );

	bool set_value( const codex::tstring& key , DWORD value );
	bool get_value( const codex::tstring& key , DWORD& value );

	bool set_value( const codex::tstring& key , const codex::tstring& value );
	bool get_value( const codex::tstring& key , codex::tstring& value ); 

	bool delete_value( const codex::tstring& key );

private:
	HKEY _key;
	HKEY _root;
	codex::tstring _subkey;
};

}}


#endif