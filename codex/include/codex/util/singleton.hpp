#ifndef __codex_singleton_h__
#define __codex_singleton_h__

#include <codex/codex.hpp>

namespace codex {
    
template < typename T >
class singleton{
public:
    DELETE_MOVE_AND_COPY(singleton);

    typedef T object_type;
    /*!
        c++11 works!
        If control enters the declaration concurrently while the variable is being initialized, 
        the concurrent execution shall wait for completion of the initialization.
    */
    static object_type& instance(void) {
        static object_type _instance;
        return _instance;
    }
protected:
    singleton(void) {}
    ~singleton(void){}
};

}
#endif