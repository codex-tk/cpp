#include <boost/asio.hpp>
#include <iostream>
#include <list>

using tcp = boost::asio::ip::tcp;

template < typename StreamTransportT >
class session 
    : public std::enable_shared_from_this< session<StreamTransportT> > {
public:
    typedef typename StreamTransportT::socket socket_type;

    explicit session( boost::asio::io_service& ios )
        : _socket( ios )
    {}

    socket_type& socket( void ) {
        return _socket;
    }

    void read( void ) {
        auto self = this->shared_from_this();
        auto buf_ptr = std::make_shared<boost::asio::streambuf >();
        boost::asio::async_read_until( _socket 
            , *buf_ptr 
            , std::string( "\r\n") 
            , [this,self,buf_ptr]( boost::system::error_code ec , std::size_t read_sz ){
                if( ec || read_sz == 0 ) return;
                std::istream is( buf_ptr.get());
                char cmd;
                is >> cmd;
                switch( cmd ) {
                    case '0': 
                        _write_bufs.push_back(buf_ptr);
                         break;
                    case '1': this->write_remains(); break;
                    case '2': this->write_remains_cont();break;
                    case '3': return;
                }
                read();
            });
    }

    void write_remains_cont(){
        std::vector< boost::asio::const_buffer > bufs( _write_bufs.size() );
        for ( auto& i : _write_bufs ) {
            bufs.push_back( i->data());
        }
        bufs.back() = boost::asio::buffer( 
            boost::asio::buffer_cast<const char*>(bufs.back())
            , boost::asio::buffer_size(bufs.back()) - 2 );
        auto self = this->shared_from_this();
        boost::asio::async_write( _socket , bufs , [this,self] ( boost::system::error_code ec , std::size_t sz ){
            if ( ec ) return;
            while ( !_write_bufs.empty() ) {
                if ( sz >= _write_bufs.front()->size() ) {
                    sz -= _write_bufs.front()->size();
                    _write_bufs.pop_front();
                } else {
                    _write_bufs.front()->consume(sz);
                    break;
                }
            }
            if ( !_write_bufs.empty() ) {
                write_remains();
            }
        });
    }

    void write_remains() {
        std::vector< boost::asio::const_buffer > bufs( _write_bufs.size() );
        for ( auto& i : _write_bufs ) {
            bufs.push_back( i->data());
        }
        auto self = this->shared_from_this();
        boost::asio::async_write( _socket , bufs , [this,self] ( boost::system::error_code ec , std::size_t sz ){
            if ( ec ) return;
            while ( !_write_bufs.empty() ) {
                if ( sz >= _write_bufs.front()->size() ) {
                    sz -= _write_bufs.front()->size();
                    _write_bufs.pop_front();
                } else {
                    _write_bufs.front()->consume(sz);
                    break;
                }
            }
            if ( !_write_bufs.empty() ) {
                write_remains();
            }
        });
    }
private:
    socket_type _socket;
    std::list< std::shared_ptr<boost::asio::streambuf > > _write_bufs;
};

void start_accept( tcp::acceptor& acceptor ){
    auto s = std::make_shared< session<tcp> >(acceptor.get_io_service());
    acceptor.async_accept( s->socket() , [s,&acceptor](boost::system::error_code ec) {
        if ( !ec ) {
            s->read();
        }
        start_accept( acceptor );
    });
}

int main() {
    try {
        boost::asio::io_service ios;
        
        tcp::acceptor acceptor(ios);
        tcp::endpoint ep( boost::asio::ip::address::from_string("127.0.0.1") , 7543 );
        acceptor.open( ep.protocol() );
        acceptor.set_option( tcp::acceptor::reuse_address(true));
        acceptor.bind(ep);
        acceptor.listen();

        start_accept( acceptor );

        ios.run();
    } catch( std::exception& e ) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}