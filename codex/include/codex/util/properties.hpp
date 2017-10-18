#ifndef __codex_util_properties_h__
#define __codex_util_properties_h__

#include <iostream>
#include <map>

namespace codex {

class properties{
public:
    properties( void );
    ~properties( void );

    std::error_code load( std::istream& is );
    
    const std::string& operator[]( const std::string& key ) const ;
private:
    std::map< std::string , std::string > _prop_map;
};

}

#endif