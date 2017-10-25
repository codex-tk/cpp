#ifndef __codex_component_id_h__
#define __codex_component_id_h__

#include <codex/tmp/tmp.hpp>

namespace codex { namespace component {

template <  typename ComponentT , 
            typename ParentT ,
            typename InfoT >
struct id {
    typedef ComponentT component_type;
    typedef ParentT parent_type;
    typedef InfoT info_type; 
};

struct interface_type{};
struct normal_type{};

#define DECL_INTERFACE_COMPONENT( compnent , parent )\
    typedef codex::component::id< compnent , parent  , codex::component::interface_type > identifier_type;\
    static identifier_type _component_id;\
    static intptr_t component_id( void ) { return reinterpret_cast<intptr_t>(&_component_id);}

#define IMPL_INTERFACE_COMPONENT( compnent , parent )\
    codex::component::id< compnent , parent , codex::component::interface_type > compnent::_component_id;

#define DECL_NORMAL_COMPONENT( compnent , parent )\
    typedef codex::component::id< compnent , parent  , codex::component::normal_type > identifier_type;\
    static identifier_type _component_id;\
    static intptr_t component_id( void ) { return reinterpret_cast<intptr_t>(&_component_id);}

#define IMPL_NORMAL_COMPONENT( compnent , parent )\
    codex::component::id< compnent , parent , codex::component::normal_type > compnent::_component_id;

#define DECL_COMPONENT( compnent  )\
    DECL_NORMAL_COMPONENT( compnent , codex::tmp::null_type )

#define IMPL_COMPONENT( compnent  )\
    IMPL_NORMAL_COMPONENT( compnent , codex::tmp::null_type )

}}

#endif