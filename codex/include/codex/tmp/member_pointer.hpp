#ifndef __codex_tmp_member_pointer_h__
#define __codex_tmp_member_pointer_h__

namespace codex {

template < typename T , typename MemberT >
struct member_pointer{
    typedef MemberT T::* type; 
};

template <  typename T , typename SignatureT >
struct member_function_pointer;

template <  typename T , typename R , typename ... Args>
struct member_function_pointer<T , R ( Args ... ) >{
    typedef R (T::*type)( Args ... );
};

}

#endif