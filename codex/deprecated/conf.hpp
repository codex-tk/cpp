#ifndef __codex_util_conf_h__
#define __codex_util_conf_h__

#include <codex.hpp>
#include <map>

namespace codex { 
namespace util {

class conf{
public:
	conf( const codex::tstring& file );
	~conf(void);

	bool load( void );

	codex::tstring operator[]( const codex::tstring& str );
private:
	codex::tstring _filename;
	std::map< codex::tstring , codex::tstring > _value;
};

}}

#endif
