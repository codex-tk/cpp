#ifndef __codex_util_utility_h__
#define __codex_util_utility_h__

#include <string>
#include <typeinfo>

#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>
namespace codex {
    static inline std::string demangle(const char* name) {
        int status = -4; // some arbitrary value to eliminate the compiler warning
        // enable c++11 by passing the flag -std=c++11 to g++
        std::unique_ptr<char, void(*)(void*)> res {
            abi::__cxa_demangle(name, NULL, NULL, &status),
            std::free
        };
        return (status==0) ? res.get() : name ;
    }
}
#else 
namespace codex {
    static inline std::string demangle(const char* name) {
        return name;
    }
}
#endif

namespace codex {
    template <class T>
    std::string pretty_type_name(const T& t) {
        return demangle(typeid(t).name());
    }
}

#endif