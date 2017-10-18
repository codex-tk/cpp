#include <codex/util/properties.hpp>
#include <codex/util/string.hpp>
#include <vector>

namespace codex {

properties::properties( void ) {

}

properties::~properties( void ) {

}

std::error_code properties::load( std::istream& is )  {
    if ( !is.good()) {
        return std::make_error_code(std::errc::invalid_argument);
    }
    std::string line;
    while ( std::getline( is , line ) ) {
        std::vector< std::string > kv;
        codex::string::split( line , std::string("=") 
            , [&] ( const std::string& value ) {
                kv.push_back(value);
            });
        if ( kv.size() == 2 ) {
            _prop_map[kv[0]] = kv[1];
        }
    }
    if ( _prop_map.size() == 0 ) {
        return std::make_error_code(std::errc::argument_out_of_domain);
    }
    return std::error_code();
}


const std::string& properties::operator[]( const std::string& key ) const  {
    auto it = _prop_map.find( key );
    if ( it != _prop_map.end() ) {
        return it->second;
    }
    static const std::string invalid("");
    return invalid;
}

}