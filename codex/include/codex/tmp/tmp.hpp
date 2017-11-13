#ifndef __codex_tmp_tmp_h__
#define __codex_tmp_tmp_h__

#include <cmath>

namespace codex { 

struct null_type{};
struct empty_type{};

//https://stackoverflow.com/questions/17178499/how-does-stdget-work
template < unsigned ... > struct seq{};
template < unsigned max , unsigned ...s > struct make_seq : make_seq< max - 1 , max - 1 , s ... >{};
template < unsigned ... s > struct make_seq< 0 , s ... > { using type = seq<s...>; };

template < typename ... Ts > struct type_list;

template < unsigned N , typename T > struct list_element;

template < unsigned N , typename Head , typename ... Tail > 
struct list_element< N , type_list< Head , Tail ... > >
    : list_element< N - 1 , type_list< Tail... > > {};

template < typename Head , typename ... Tail > 
struct list_element< 0 , type_list< Head , Tail ... > > { using type = Head; };

template < typename Seq , typename ... Ts > struct pop_back_helper;

template < unsigned ... N , typename ... Ts >
struct pop_back_helper< seq< N... > , Ts ... > {
    using type = type_list< typename type_list<Ts ... >::template type<N>...  >;
};

template < typename ... Ts >
struct type_list { 
    template < unsigned N > 
    using type = typename list_element< N , type_list< Ts ... > >::type; 
    using last = typename list_element< sizeof...(Ts) -1 , type_list< Ts ... > >::type;
    template < typename U >
    using push_back = type_list< Ts ... , U >;
    template < template < typename ... Args > class U >
    using rebind = U< Ts ... >;
    using pop_back = typename pop_back_helper< typename make_seq<sizeof...(Ts) - 1>::type , Ts... >::type;
};

template <  template < typename ... Ts > class U , typename ... Ts >
struct type_list<U< Ts... > >  : type_list< Ts ... >{};


namespace sample {
    //--------------------------------------------------------------------------------------------------
    template < unsigned N , typename T > struct storage { T data; };
    template < typename T , typename ... Ts > struct storage_helper{};
    template < unsigned s0 , unsigned ... s , typename T0 , typename ... Ts >
    struct storage_helper< codex::seq< s0 , s ... > , T0 , Ts... > 
        : storage< s0 , T0 > 
        , storage_helper< codex::seq< s... > , Ts ... > {};


    template < typename ... Args > 
    struct sample_tuple : storage_helper< typename codex::make_seq< sizeof...(Args) >::type , Args... >{};

    template < unsigned N  , typename T >
    T& sample_tuple_get( storage<N,T>& storage ) {
        return storage.data;
    }
    //--------------------------------------------------------------------------------------------------
    template < unsigned N , typename T > struct value { 
        T data; 
        value( void ) {}
        value( T&& data ) : data(data){}
    };

    template < typename N , typename ... Ts > struct basic_values;

    template < unsigned ... S , typename ... Ts > 
    struct basic_values< codex::seq< S... > , Ts ... > : value< S , Ts >... {
        basic_values( Ts&& ... ts ) 
            : value<S,Ts>( std::forward<Ts>(ts))... {}
    };

    template < typename ... Ts >
    struct tuple : basic_values< 
            typename codex::make_seq< sizeof...(Ts)>::type 
            , Ts ... > 
    {
        using basic_values< 
            typename codex::make_seq< sizeof...(Ts)>::type 
            , Ts ... >::basic_values;
    };

    template < unsigned N  , typename T >
    T& get_value( value<N,T>& value ) {
        return value.data;
    }
    //--------------------------------------------------------------------------------------------------
}

}

#endif