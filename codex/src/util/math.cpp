#include <codex/util/math.hpp>

namespace codex { namespace math{
    double radian( double degree ) {
        return degree * M_PI / 180;
    }

    double degree( double radian ) {
        return radian * 180 / M_PI;
    }
}}