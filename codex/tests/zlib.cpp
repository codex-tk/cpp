#include <gtest/gtest.h>
#include <zlib.h>
#include <vector>

namespace detail {

    static const int ZLIB_CHUNK_SIZE = 16384;

    template < typename Service >
    class zlib {
    public:
        typedef Service service_type;

        template < typename Callback >
        bool service( uint8_t* ptr , int size , Callback&& cb , bool force = false ) {
            if ( ptr == nullptr ) {
                if ( _data.size() == 0 ) return true;
                return _service.service( _data , std::forward<Callback>(cb));
            } else {
                _data.insert(_data.end(), ptr, ptr+size);
                if ( _data.size() > ZLIB_CHUNK_SIZE || force ) {
                    return _service.service( _data , cb );
                }
            }
            return true;
        }
    private:
        std::vector<uint8_t> _data;
        service_type _service;
    };

    class inflate_service {
    public:
        inflate_service( void ) {
            memset( &_stream , 0x00 , sizeof(_stream));
            if ( inflateInit(&_stream) != Z_OK ) {
                throw std::runtime_error("zlib::inflateInit fail");
            }
        }

        ~inflate_service( void ) {
            inflateEnd( &_stream );
        }

        template < typename Callback >
        bool service( std::vector<uint8_t>& data , Callback&& callback ) {
            uint8_t outbuf[ZLIB_CHUNK_SIZE] = {0};
            
            _stream.avail_in = data.size();
            _stream.next_in = &data[0];
        
            do {
                _stream.avail_out = ZLIB_CHUNK_SIZE;
                _stream.next_out = outbuf;
                int ret = inflate( &_stream , Z_NO_FLUSH );
                if ( ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR ) {
                    return false;
                }
                callback( outbuf , ZLIB_CHUNK_SIZE - _stream.avail_out );
            } while ( _stream.avail_out == 0 );
            data.clear();
            return true;
        }
    private:
        z_stream _stream;
    };

    class deflate_service {
    public:
        deflate_service( void ) {
            memset( &_stream , 0x00 , sizeof(_stream));
            if ( deflateInit(&_stream , Z_BEST_COMPRESSION) != Z_OK ) {
                throw std::runtime_error("zlib::deflateInit fail");
            }
        }

        ~deflate_service( void ) {
            deflateEnd( &_stream );
        }

        template < typename Callback >
        bool service( std::vector<uint8_t>& data , Callback&& callback ) {
            uint8_t outbuf[ZLIB_CHUNK_SIZE] = {0};

            _stream.avail_in = data.size();
            _stream.next_in = &data[0];
        
            do {
                _stream.avail_out = ZLIB_CHUNK_SIZE;
                _stream.next_out = outbuf;
                int ret = deflate( &_stream , Z_FINISH );
                if ( ret == Z_STREAM_ERROR || ret == Z_BUF_ERROR ) {
                    return false;
                }
                callback( outbuf , ZLIB_CHUNK_SIZE - _stream.avail_out );
            } while ( _stream.avail_out == 0 );
            data.clear();
            return true;
        }
    private:
        z_stream _stream;
    };
    
    bool compress( void* gptr , int gsize , void* pptr , int& psize ) {
        zlib< deflate_service > compressor;
        int outsize = 0;
        auto callback = [&]( uint8_t* out , int size ) {
            int cpysize = psize - outsize;
            cpysize = std::min(cpysize , size);
            memcpy( static_cast<uint8_t*>(pptr) + outsize , out , size );
            outsize += size;
        };
        if ( compressor.service( static_cast<uint8_t*>(gptr) , gsize , callback, true )){
            psize = outsize;
            return true;
        }
        return false;
    }
    
    bool decompress( void* gptr , int gsize , void* pptr , int& psize ) {
        zlib< inflate_service  > decompressor;
        int outsize = 0;
        auto callback = [&]( uint8_t* out , int size ) {
            int cpysize = psize - outsize;
            cpysize = std::min(cpysize , size);
            memcpy( static_cast<uint8_t*>(pptr) + outsize , out , size );
            outsize += size;
        };
        if ( decompressor.service( static_cast<uint8_t*>(gptr) , gsize , callback , true )){
            psize = outsize;
            return true;
        }
        return false;
    }
}

TEST( zlib , compress ) {
    
    std::vector<int> orig;
    std::vector<int> compressed(1024);
    std::vector<int> decompressed;
    constexpr int buffer_size = sizeof(int)*1024;

    for ( int i = 0 ; i < 1024 ; ++i ) {
        orig.push_back(i);
        decompressed.push_back(0);
    }

    int sz = buffer_size;
    int desz = buffer_size ;

    ASSERT_TRUE(detail::compress( &orig[0] , buffer_size , &compressed[0] , sz ));
    ASSERT_NE( sz ,sizeof(int)*1024);

    ASSERT_TRUE(detail::decompress( &compressed[0] , sz ,  &decompressed[0] , desz ));
    ASSERT_EQ( desz , sizeof(int)*1024 );

    int value = 0;
    for ( int& i : decompressed ) {
        ASSERT_EQ( i , value++ );
    }
}

namespace detail{
bool raw_compress( void* gptr , int gsize , void* pptr , int& psize ) {
    z_stream strm;
    uint8_t out_chunk[ZLIB_CHUNK_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    int ret = deflateInit(&strm , Z_BEST_COMPRESSION);
    if (ret != Z_OK)
        return false;
    
    strm.avail_in = gsize;
    strm.next_in = static_cast<uint8_t*>(gptr);

    int proc = 0;
    do {
        strm.avail_out = ZLIB_CHUNK_SIZE;
        strm.next_out = out_chunk;
        ret = deflate( &strm , Z_FINISH );
        if ( ret == Z_STREAM_ERROR || ret == Z_BUF_ERROR ) {
            deflateEnd(&strm);
            return false;
        }
        memcpy( static_cast<uint8_t*>(pptr) + proc , out_chunk , ZLIB_CHUNK_SIZE - strm.avail_out );
        proc += ZLIB_CHUNK_SIZE - strm.avail_out;
    } while ( strm.avail_out == 0 );
    psize = proc;
    deflateEnd( &strm );
    return true;
}

bool raw_decompress( void* gptr , int gsize , void* pptr , int& psize ) {
    z_stream strm;
    uint8_t out_chunk[ZLIB_CHUNK_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = 0;

    int ret = inflateInit(&strm);
    if (ret != Z_OK)
        return false;
    
    strm.avail_in = gsize;
    strm.next_in = static_cast<uint8_t*>(gptr);

    int proc = 0;
    do {
        strm.avail_out = ZLIB_CHUNK_SIZE;
        strm.next_out = out_chunk;
        ret = inflate( &strm , Z_NO_FLUSH );
        switch( ret ) {
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd( &strm );
                return false;
        }
        memcpy( static_cast<uint8_t*>(pptr) + proc , out_chunk , ZLIB_CHUNK_SIZE - strm.avail_out );
        proc += ZLIB_CHUNK_SIZE - strm.avail_out;
    } while ( strm.avail_out == 0 );
    inflateEnd( &strm );
    psize = proc;
    return true;
}
}

/*

// original string len = 36
char a[50] = "Hello Hello Hello Hello Hello Hello!"; 

// placeholder for the compressed (deflated) version of "a" 
char b[50];

// placeholder for the UNcompressed (inflated) version of "b"
char c[50];
    

printf("Uncompressed size is: %lu\n", strlen(a));
printf("Uncompressed string is: %s\n", a);


printf("\n----------\n\n");

// STEP 1.
// deflate a into b. (that is, compress a into b)

// zlib struct
z_stream defstream;
defstream.zalloc = Z_NULL;
defstream.zfree = Z_NULL;
defstream.opaque = Z_NULL;
// setup "a" as the input and "b" as the compressed output
defstream.avail_in = (uInt)strlen(a)+1; // size of input, string + terminator
defstream.next_in = (Bytef *)a; // input char array
defstream.avail_out = (uInt)sizeof(b); // size of output
defstream.next_out = (Bytef *)b; // output char array

// the actual compression work.
deflateInit(&defstream, Z_BEST_COMPRESSION);
deflate(&defstream, Z_FINISH);
deflateEnd(&defstream);
    
// This is one way of getting the size of the output
printf("Compressed size is: %lu %lu %lu\n"
, strlen(b) 
, (uInt)sizeof(b) - defstream.avail_out
, defstream.total_out );
printf("Compressed string is: %s\n", b);


printf("\n----------\n\n");


// STEP 2.
// inflate b into c
// zlib struct
z_stream infstream;
infstream.zalloc = Z_NULL;
infstream.zfree = Z_NULL;
infstream.opaque = Z_NULL;
// setup "b" as the input and "c" as the compressed output
infstream.avail_in = (uInt)((char*)defstream.next_out - b); // size of input
infstream.next_in = (Bytef *)b; // input char array
infstream.avail_out = (uInt)sizeof(c); // size of output
infstream.next_out = (Bytef *)c; // output char array
    
// the actual DE-compression work.
inflateInit(&infstream);
inflate(&infstream, Z_NO_FLUSH);
inflateEnd(&infstream);
    
printf("Uncompressed size is: %lu\n", strlen(c));
printf("Uncompressed string is: %s\n", c);


// make sure uncompressed is exactly equal to original.
ASSERT_STREQ(a,c);*/