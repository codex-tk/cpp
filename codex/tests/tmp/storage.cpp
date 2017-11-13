#include <gtest/gtest.h>

template < typename T > struct storage{ T data; };
template < typename U > U& get( storage<U>& s ){ return s.data; };
template < typename ... Ts > struct combined_storage : storage<Ts>... {
    template < typename U > U& _get( storage<U>& s ){ return s.data; }
    template < typename U > U& data() { return combined_storage::template _get<U>(*this);}
};

TEST( storage , combine ) {
    combined_storage< int , double , std::string > storage;
    
    get<int>(storage) = 1;
    get<double>(storage)=0.1;
    get<std::string>(storage)="Sample";
    
    ASSERT_EQ( get<int>(storage) , 1 );
    ASSERT_EQ( get<double>(storage),0.1);
    ASSERT_EQ( get<std::string>(storage),"Sample");

    ASSERT_EQ( storage.template data<int>() , 1 );
    ASSERT_EQ( storage.template data<double>(),0.1);
    ASSERT_EQ( storage.template data<std::string>(),"Sample");

    storage.template data<int>() = 1;
    storage.template data<double>() = 0.1;
    storage.template data<std::string>() = "Sample";

    ASSERT_EQ( storage.template data<int>() , 1 );
    ASSERT_EQ( storage.template data<double>(),0.1);
    ASSERT_EQ( storage.template data<std::string>(),"Sample");
}
