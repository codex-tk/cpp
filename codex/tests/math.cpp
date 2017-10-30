#include <gtest/gtest.h>
#include <codex/util/math.hpp>
#include "helper.hpp"
#include <cmath>

TEST( math , rad_deg ) {
    ASSERT_EQ( 180 , codex::math::degree( codex::math::pi ));
    ASSERT_EQ( codex::math::pi , codex::math::radian( 180 ));
    ASSERT_EQ( round(57.2958) , round(codex::math::degree(1)));
}