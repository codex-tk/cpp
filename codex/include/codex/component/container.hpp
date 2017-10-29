#ifndef __codex_component_container_h__
#define __codex_component_container_h__

#include <codex/component/id.hpp>
#include <codex/component/detail.hpp>
#include <unordered_map>
#include <memory>

namespace codex { namespace component {

class container{
public:
    template < typename ComponentT >
    void insert( const std::shared_ptr<ComponentT>& component ) {
        typedef typename detail::representation_component<ComponentT>::type component_type;
        _type_map[ component_type::component_id() ] = std::static_pointer_cast<void>(component);
        // 인터페이스 타입이면서 인터페이스 이외의 타입으로 추가시
        if ( detail::is_interface<component_type>::value and 
            not std::is_same< component_type , ComponentT >::value )
        {
            _type_map[ComponentT::component_id()] = std::static_pointer_cast<void>(component);
        }
    }

    template < typename ComponentT > 
    std::shared_ptr<ComponentT> get( void ) {
        typedef typename detail::component_trait<ComponentT>::component_type component_type;
        typedef typename detail::component_trait<ComponentT>::info_type info_type;
        typedef typename detail::representation_component<ComponentT>::type representation_component_type;
        intptr_t cid = ComponentT::component_id();
        // interface_type 의 하위 타입으로 요청시
        if ( detail::is_interface<component_type>::value and 
            std::is_same< normal_type , info_type >::value )
        {
            if ( _type_map.find(cid) != _type_map.end() ) {
                return std::static_pointer_cast<ComponentT>(_type_map[ cid ]);
            }
            return std::shared_ptr<ComponentT>();
        }
        cid = representation_component_type::component_id();
        if ( _type_map.find( cid ) != _type_map.end() ) {
            return std::static_pointer_cast<ComponentT>(_type_map[ cid ]);
        }
        return std::shared_ptr<ComponentT>();
    }
private:
    std::unordered_map< intptr_t , std::shared_ptr<void> > _type_map;
};

}}

#endif