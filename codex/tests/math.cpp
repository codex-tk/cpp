#include <gtest/gtest.h>
#include <codex/util/math.hpp>
#include "helper.hpp"

TEST( math , rad_deg ) {
    ASSERT_EQ( 180 , codex::math::degree( M_PI ));
    ASSERT_EQ( M_PI , codex::math::radian( 180 ));
    ASSERT_EQ( round(57.2958) , round(codex::math::degree(1)));
}