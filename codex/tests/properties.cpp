#include <gtest/gtest.h>
#include <codex/util/properties.hpp>
#include <fstream>

TEST( properties , usage ) {
    std::istringstream iss;
    iss.str( "key0=value0\nkey1=value1\n" );

    codex::properties prop;
    ASSERT_FALSE(prop.load( iss ));

    ASSERT_STREQ( prop["key0"].c_str() , "value0");
    ASSERT_STREQ( prop["key"].c_str() , "");
    ASSERT_STREQ( prop["key1"].c_str() , "value1");
}

TEST( properties , invalid_arg ) {
    codex::properties prop;
    std::istringstream iss;
    ASSERT_TRUE(prop.load( iss ));
}

TEST( properties , error_domain ) {
    codex::properties prop;
    std::istringstream iss;
    iss.str( "--" );
    ASSERT_TRUE(prop.load( iss ));
}

TEST( properties , fstream ) {
    std::ofstream os( "./sample.properties" );
    os <<  "key0=value0\nkey1=value1\n";
    os.close();

    std::ifstream is( "./sample.properties" );
    codex::properties prop;
    ASSERT_FALSE(prop.load( is ));
    ASSERT_STREQ( prop["key0"].c_str() , "value0");
    ASSERT_STREQ( prop["key"].c_str() , "");
    ASSERT_STREQ( prop["key1"].c_str() , "value1");
}