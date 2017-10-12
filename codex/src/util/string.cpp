#include <codex/util/string.hpp>
#include <stdarg.h>

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

}}