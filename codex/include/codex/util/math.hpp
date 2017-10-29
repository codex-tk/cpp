#ifndef __codex_util_math_h__
#define __codex_util_math_h__

namespace codex { namespace math {

    constexpr double pi = 3.14159265358979323846;
    /*
    180 degree = pi radian
    1 degree = pi / 180 radian
    x degree = x * pi / 180 radian

    pi radian = 180 degree
    1 radian  = 180 / pi degree
    x radian  = x * 180 / pi degree
    */

    double radian( double degree );
    double degree( double radian );


}}


#endif