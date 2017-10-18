#ifndef __codex_ssl_h__
#define __codex_ssl_h__

namespace codex {
namespace ssl {

#if defined ( _CODEX_OPENSSL_ENABLED_)

class open_ssl {
public:
	static bool init( void );
private:

};

#endif
}}

#endif

