#ifndef __codex_component_container_h__
#define __codex_component_container_h__

#include <codex/component/id.hpp>
#include <unordered_map>
#include <memory>

namespace codex { namespace component {
namespace detail{

template < typename T >
struct component_trait {
    typedef typename T::ideitiifer_type::component_type component_type;
    typedef typename T::ideitiifer_type::parent_type parent_type;
    typedef typename T::ideitiifer_type::info_type info_type;
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
struct is_interface<codex::tmp::null_type>{
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
struct interface_component<codex::tmp::null_type>{
    typedef codex::tmp::null_type type;
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

}

class container{
public:
    template < typename ComponentT >
    void insert( const std::shared_ptr<ComponentT>& component ) {
        typedef typename detail::representation_component<ComponentT>::type component_type;
        _map[ component_type::component_id() ] = 
            std::static_pointer_cast<void>(component);
    }

    template < typename ComponentT > 
    std::shared_ptr<ComponentT> get( void ) {
        typedef typename detail::component_trait<ComponentT>::component_type component_type;
        typedef typename detail::component_trait<ComponentT>::info_type info_type;
        typedef typename detail::representation_component<ComponentT>::type representation_component_type;
        if ( detail::is_interface<component_type>::value 
            && !std::is_same< interface_type , info_type  >::value )
        {
            return std::shared_ptr<ComponentT>();
        }
        intptr_t cid = representation_component_type::component_id();
        if ( _map.find( cid ) != _map.end() ) {
            return std::static_pointer_cast<ComponentT>(_map[ cid ]);
        }
        return std::shared_ptr<ComponentT>();
    }
private:
    std::unordered_map< intptr_t , std::shared_ptr<void> > _map;
};

}}

#endif