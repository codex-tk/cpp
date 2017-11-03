#ifndef __codex_buffer_stream_h__
#define __codex_buffer_stream_h__

#include <codex/buffer/basic_byte_buffer.hpp>

namespace codex { namespace buffer {

template < typename BufferT >
class stream{
public:
    explicit stream( BufferT& buffer );
    ~stream( void );
private:
    BufferT& _buffer;
};

}}

#include <codex/buffer/stream.ipp>

#endif