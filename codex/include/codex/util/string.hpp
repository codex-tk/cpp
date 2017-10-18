#ifndef __codex_string_h__
#define __codex_string_h__

#include <iostream>

namespace codex {
namespace string {
namespace detail {
    
    template < typename T_char >
    struct length;
    
    template <>
    struct length< char > {
        static std::size_t value( char* msg ) {
            return strlen(msg);
        }
    };
    
    template <>
    struct length< wchar_t > {
        static std::size_t value( wchar_t* msg ) {
            return wcslen(msg);
        }
    };
}

class formatable {
public:
    formatable( std::string& buf );
    formatable& append( const char* fmt , ... );
    std::string& value( void );
private:
    std::string& _string;
};

/*
template < typename string_type , typename out >
static void split( const string_type& message , const string_type& sep , out os ) {
	typename string_type::size_type msgEnd = message.length();
    typename string_type::size_type start  = 0;
    typename string_type::size_type stop   = 0;
        
    start = message.find_first_not_of( sep );
    while ( true ) {
        if ( start >= msgEnd ) break;

        stop = message.find_first_of(sep, start);
        if ( stop == string_type::npos ) 
            stop = msgEnd;

        *os = message.substr( start , stop - start );
        start = message.find_first_not_of( sep , stop + 1 );
		++os;
    }
}*/

template < typename StringType , typename Callback >
void split( const StringType& msg , const StringType& sep , Callback&& callback ) {
    typedef typename StringType::size_type size_type;
    size_type split_begin = msg.find_first_not_of(sep);
    while ( split_begin != StringType::npos ) {
        size_type split_end = msg.find_first_of( sep , split_begin );
        if ( split_end == StringType::npos ) split_end = msg.length();
        callback( msg.substr( split_begin , split_end - split_begin ) );
        split_begin = msg.find_first_not_of(sep,split_end+1);
    }
}

template < typename StringType  >
static StringType replace( const StringType& message
     , const StringType& pattern
      , const StringType& replace ) 
{
    StringType out = message;
    typename StringType::size_type pattern_begin  = out.find( pattern , 0 );
    while ( pattern_begin != StringType::npos ) {
        out.replace( pattern_begin , pattern.length() , replace );
        pattern_begin = out.find( pattern , pattern_begin + pattern.length() );
    }
    return out;
}


int atoi( const char* value );

/*
template < typename string_type >
static string_type ltrim( const string_type& msg , const string_type& trimStr ) {
	typename string_type::size_type start = msg.find_first_not_of( trimStr );
	if ( start != string_type::npos ) {
		return msg.substr( start , msg.length() - start );
	}
	return msg;
}

template < typename string_type >
static string_type rtrim( const string_type& msg , const string_type& trimStr ) {
	typename string_type::size_type end = msg.find_last_not_of( trimStr );
	if ( end != string_type::npos ) {
		return msg.substr( 0 , end + 1 );
	}
	return msg;
}
	
template < typename string_type >
static void trim( string_type& msg , const string_type& trimStr ) {
	typename string_type::size_type pos = msg.find_last_not_of( trimStr );
	if ( pos != string_type::npos ) {
		msg.erase( pos + 1 );
	}

	pos = msg.find_first_not_of( trimStr );
	if ( pos != string_type::npos ) {
		msg.erase( 0 , pos );
	}
}

template < typename string_type >
static string_type to_upper( const string_type& msg ) {
	string_type out;
	out.reserve( msg.length() );
	for ( typename string_type::size_type idx = 0 ; 
			idx < msg.length() ; 
			++idx ) {

		if ( msg[idx] < 0xff && isalpha( msg[idx] ) && islower( msg[idx] ) ) {
			out.push_back( _toupper(msg[idx]) );
		} else {
			out.push_back( msg[idx] );
		}
	}
	return out;
}

template < typename string_type >
static string_type to_lower( const string_type& msg ) {
	string_type out;
	out.reserve( msg.length() );
	for ( typename string_type::size_type idx = 0 ; 
			idx < msg.length() ; 
			++idx ) {
		if ( msg[idx] < 0xff && isalpha( msg[idx]) &&  isupper( msg[idx] ) ) {
			out.push_back( _tolower(msg[idx]) );
		} else {
			out.push_back( msg[idx] );
		}
	}
	return out;
}

template < typename T_char >
static T_char* ltrim( T_char* message , T_char ch =' ' ){
	if ( message != nullptr ) {
        T_char* start      = message;
        T_char* ltrimStart = message;

        while ( *ltrimStart == ch )
            ++ltrimStart;

        if ( ltrimStart == start )
            return message;
        
        while ( *ltrimStart ) {
            *(start++) = *(ltrimStart++);
        }
        *start = '\0';
    }
    return message;
}

template < typename T_char >
static T_char* rtrim( T_char* message , T_char ch = ' ' ){
	if ( message != nullptr ) {
		size_t len = detail::length<T_char>::value( message );
        T_char* rtrimEnd = message + len;
        while ( *(--rtrimEnd) == ch );
        *(++rtrimEnd) = '\0';
    }
    return message;
}
*/
}}

#endif