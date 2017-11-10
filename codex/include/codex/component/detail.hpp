#ifndef __codex_component_detail_h__
#define __codex_component_detail_h__

#include <codex/component/id.hpp>

namespace codex { namespace component {
namespace detail{

template < typename T >
struct component_trait {
    typedef typename T::identifier_type::component_type component_type;
    typedef typename T::identifier_type::parent_type parent_type;
    typedef typename T::identifier_type::info_type info_type;
};

template < typename T >
struct is_interface {
    enum {
        value = std::conditional< 
                    std::is_same< 
                        interface_type ,
                        typename component_trait<T>::info_type
                    >::value ,
                    std::true_type ,
                    is_interface< typename component_trait<T>::parent_type >
                >::type::value
    };
};

template <>
struct is_interface<codex::null_type>{
    enum { 
        value = false
    };
};

template < typename T >
struct interface_component {
    typedef typename std::conditional< 
        std::is_same< 
            interface_type ,
            typename component_trait<T>::info_type 
        >::value ,
        typename component_trait<T>::component_type ,
        typename interface_component<
                typename component_trait<T>::parent_type
        >::type 
    >::type type;
};

template <>
struct interface_component<codex::null_type>{
    typedef codex::null_type type;
};

template < typename T >
struct representation_component {
    typedef typename component_trait<T>::component_type component_type;
    typedef typename std::conditional< 
        is_interface<component_type>::value , 
        typename interface_component< component_type >::type , 
        component_type 
    >::type type;
};

}}}

#endif