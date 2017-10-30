#ifndef __codex_buffer_stream_h__
#define __codex_buffer_stream_h__

#include <codex/buffer/byte_buffer.hpp>

namespace codex { namespace buffer {

class stream{
public:
    explicit stream( const std::size_t size );
    ~stream( void );
private:
    codex::buffer::byte_buffer _buffer;
};

}}

#endif