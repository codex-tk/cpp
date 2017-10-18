#include <codex/util/string.hpp>
#include <stdarg.h>
#include <codex/codex.hpp>

namespace codex {
namespace string{

formatable::formatable( std::string& buf )
    : _string(buf)
{

}

formatable& formatable::append( const char* fmt , ... ) {
    if ( fmt == nullptr )
        return *this;

    constexpr int buffer_size = 2048;
    char fmt_buffer[buffer_size] = {0};
    va_list args;
    va_start( args , fmt );
    vsnprintf( fmt_buffer , buffer_size , fmt , args );
    va_end( args );
    _string += fmt_buffer;
    return *this;
}

std::string& formatable::value( void ) {
    return _string;
}

int atoi( const char* value ) {
    CODEX_ASSERT( value != nullptr 
        , "atoi value == nullptr" );
    if ( value == nullptr ) return 0;

    while( *value && *value == ' ') 
        ++value;

    int ans = 0;
    bool minus = false;
    if ( *value == '-') {
        minus = true;
        ++value;
    }
    while ( *value ) {
        if ( *value >= '0' && *value <= '9'){
            ans *= 10;
            ans += (*value - '0');
            ++value;
        }else{
            break;
        }
    }
    if ( minus ) ans *= -1;
    return ans;
}

}}