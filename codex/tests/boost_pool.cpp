#include <gtest/gtest.h>
#if defined( ENABLE_LIB_BOOST )
#include <boost/pool/pool.hpp>

TEST( boost ,  pool ) {
    boost::pool<> p(sizeof(int));
    for (int i = 0; i < 10000; ++i)
    {
        int* t = static_cast<int*>(p.malloc());
        (*t) = 32;
    }
}

#endif