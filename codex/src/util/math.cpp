#include <codex/util/math.hpp>


namespace codex { namespace math{
    
    double radian( double degree ) {
        return degree * math::pi / 180;
    }

    double degree( double radian ) {
        return radian * 180 / math::pi;
    }
}}