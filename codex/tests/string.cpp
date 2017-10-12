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
                .value()
                .c_str() 
                , "sample");
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

TEST_F( test_string , replace ){
    _message = "this is sparta!";
    _message = codex::string::replace( _message , std::string(" ") ,std::string(","));
    ASSERT_EQ( _message , std::string("this,is,sparta!") );
    _message = codex::string::replace( _message , std::string(",is") ,std::string("^^^"));
    ASSERT_EQ( _message , std::string("this^^^,sparta!") ); 
}