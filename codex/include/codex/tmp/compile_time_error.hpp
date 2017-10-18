#ifndef __codex_tmp_cte_h__
#define __codex_tmp_cte_h__

namespace codex { namespace tmp {

template < bool > 
class compile_time_error;

template <>
class compile_time_error<true>{
};

}}

#define STATIC_CHECK( expr , msg ) { codex::tmp::compile_time_error< (expr) != 0 > ERROR_##msg;(void)ERROR_##msg;}
//STATIC_CHECK( sizeof( object_type ) <= sizeof( void* ) , object_type_too_big );

#endif