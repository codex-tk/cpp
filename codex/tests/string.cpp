#include <gtest/gtest.h>
#include <codex/codex.hpp>
#include <codex/util/string.hpp>

class test_string : public testing::Test {
public:
    void SetUp() override {
    }
    void TearDown() override {
    }
protected:
    std::string _message;
};

TEST_F( test_string , formatable ) {
    codex::string::formatable fs(_message);
    fs.append( "%d %d" , 1 , 2 );
    ASSERT_STREQ( _message.c_str() , "1 2");
    fs.append( "%d" , 1).append("%d" , 2);
    ASSERT_STREQ( _message.c_str() , "1 212");

    _message = "";

    ASSERT_STREQ( codex::string::formatable(_message)
                .append( "%s" , "sample" )
                .append( "%d" , 1 )
                .value()
                .c_str() 
                , "sample1");
}

TEST_F( test_string , split ) {
    _message = "1,2,3,4";
    std::vector< std::string > split;
    codex::string::split( _message , std::string(",") , [&] ( const std::string& s ) {
        split.push_back(s);
    });
    ASSERT_EQ( split.size() , 4 );
    ASSERT_EQ( split[0] , std::string("1"));
    ASSERT_EQ( split[1] , std::string("2"));
    ASSERT_EQ( split[2] , std::string("3"));
    ASSERT_EQ( split[3] , std::string("4"));
}

TEST_F( test_string , split0 ) {
    _message = "1,2,3,4";
    std::vector< std::string > split;
    auto bi = std::back_inserter(split);
    codex::string::split( _message , std::string(",") , [&] ( const std::string& s ) {
        *bi = s;
        ++bi;
    });
    ASSERT_EQ( split.size() , 4 );
    ASSERT_EQ( split[0] , std::string("1"));
    ASSERT_EQ( split[1] , std::string("2"));
    ASSERT_EQ( split[2] , std::string("3"));
    ASSERT_EQ( split[3] , std::string("4"));
}

TEST_F( test_string , split2 ) {
    _message = "1  2  3  4";
    std::vector< std::string > split;
    auto bi = std::back_inserter(split);
    codex::string::split( _message , std::string("  ") , [&] ( const std::string& s ) {
        *bi = s;
        ++bi;
    });
    ASSERT_EQ( split.size() , 4 );
    ASSERT_EQ( split[0] , std::string("1"));
    ASSERT_EQ( split[1] , std::string("2"));
    ASSERT_EQ( split[2] , std::string("3"));
    ASSERT_EQ( split[3] , std::string("4"));
}


TEST_F( test_string , replace ){
    _message = "this is sparta!";
    _message = codex::string::replace( _message , std::string(" ") ,std::string(","));
    ASSERT_EQ( _message , std::string("this,is,sparta!") );
    _message = codex::string::replace( _message , std::string("is") ,std::string("^^!"));
    ASSERT_EQ( _message , std::string("th^^!,^^!,sparta!") ); 
}

TEST_F( test_string , split_key_value ) {
    _message = "key0:value0,key1:value1,key2:value2";
    std::vector< std::string > items;
    auto items_bi = std::back_inserter(items);
    codex::string::split( _message , std::string(",") , [&] ( const std::string& s ) {
        *items_bi = s;
        ++items_bi;
    });
    std::map<std::string,std::string> kv_map;
    for ( std::string& kv_it : items ) {
        std::vector< std::string > key_value;
        auto kv_bi = std::back_inserter(key_value);
        codex::string::split( kv_it , std::string(":") , [&] ( const std::string& s ) {
            *kv_bi = s;
            ++kv_bi;
        });
        if (key_value.size() == 2) {
            kv_map[key_value[0]] = key_value[1];
        }
    }
    ASSERT_EQ( kv_map["key0"] , "value0");
    ASSERT_EQ( kv_map["key1"] , "value1");
    ASSERT_EQ( kv_map["key2"] , "value2");
}

TEST( atoi , atoi ) {
    std::vector< std::string > tests {
        "42" 
        ,"3.14159" 
        ,"31337 with words"
        ,"words and 2"
        ,"  2"
    };
    std::vector< int > ans {
        42 
        , 3 
        , 31337
        , 0 
        , 2
    };

    for ( std::size_t i = 0 ; i < tests.size() ; ++i ) {
        ASSERT_EQ(  codex::string::atoi(tests[i].c_str()) , ans[i] );
    }
}