#include <gtest/gtest.h>
#include <codex/util/utility.hpp>
#include <codex/tmp/tmp.hpp>

template< unsigned ... > struct seq{};

template < unsigned max , unsigned ...s > struct make_seq0 { typedef int type; };
template < unsigned ... s > struct make_seq0< 0 , s ... > { typedef char type; };

TEST( make_seq , view ) {
    std::cout << codex::pretty_type_name( make_seq0<0>::type{} ) << std::endl; // char
    std::cout << codex::pretty_type_name( make_seq0<1>::type{} ) << std::endl; // int
    std::cout << codex::pretty_type_name( make_seq0<2>::type{} ) << std::endl; // int
    std::cout << codex::pretty_type_name( make_seq0<3>::type{} ) << std::endl; // int
    std::cout << codex::pretty_type_name( make_seq0<4>::type{} ) << std::endl; // int
    std::cout << codex::pretty_type_name( make_seq0<5>::type{} ) << std::endl; // int
}

template < unsigned max , unsigned ...s > struct make_seq1 { typedef seq<max - 1, max - 1, s... > type;  };
template < unsigned ... s > struct make_seq1< 0 , s ... > { typedef seq<s...> type;  };

TEST( make_seq , view2 ) {
    std::cout << codex::pretty_type_name( make_seq1<0>::type{} ) << std::endl; // char
    // make_seq1< 0 , s ... > - > make_seq1< 0 , none > -> seq<>

    std::cout << codex::pretty_type_name( make_seq1<1>::type{} ) << std::endl; // int
    // make_seq1< max = 1 , s ... = none  > ->  seq< 1 -1 , 1 -1 , none > -> seq< 1 -1 , 1 -1  > 

    std::cout << codex::pretty_type_name( make_seq1<2>::type{} ) << std::endl; // int
    std::cout << codex::pretty_type_name( make_seq1<3>::type{} ) << std::endl; // int
    std::cout << codex::pretty_type_name( make_seq1<4>::type{} ) << std::endl; // int
    std::cout << codex::pretty_type_name( make_seq1<5>::type{} ) << std::endl; // int
}


TEST( make_seq , view3 ) {
    std::cout << codex::pretty_type_name( codex::make_seq<0>::type{} ) << std::endl; // char
    // make_seq< 0 , s ... > - > make_seq1< 0 , none > -> seq<>

    std::cout << codex::pretty_type_name( codex::make_seq<1>::type{} ) << std::endl; // int
    // make_seq< max = 1 , s ... = none  > ->  make_seq< 0 , 0 , none > -> seq<0>

    std::cout << codex::pretty_type_name( codex::make_seq<2>::type{} ) << std::endl; // int
    // make_seq< max = 2 , s ... = none  > -> : make_seq< 1 , 1 > -> : make_seq< 0 , 1  > -> seq< 0 , 1 >

    std::cout << codex::pretty_type_name( codex::make_seq<3>::type{} ) << std::endl; // int
    // make_seq< 3 , ... > - > make_seq< 2 , 2 , ... > -> make_seq< 1 , 1 , 2 , ... > 
    //-> make_seq< 0 , 0 , 1, 2 , ... > -> seq< 0 , 1 , 2 >
    
    std::cout << codex::pretty_type_name( codex::make_seq<4>::type{} ) << std::endl; // int
    std::cout << codex::pretty_type_name( codex::make_seq<5>::type{} ) << std::endl; // int

    std::cout << codex::pretty_type_name( codex::make_seq<6>::type{} ) << std::endl;
}

TEST( tmp , type_list ) {
    codex::sample_tuple< int , double , char > tuple;
    codex::sample_tuple_get< 0 > (tuple) = 1;
    codex::sample_tuple_get< 1 > (tuple) = 1.1;
    codex::sample_tuple_get< 2 > (tuple) = 'c';

    ASSERT_EQ( codex::sample_tuple_get< 0 > (tuple) , 1  );
    ASSERT_EQ( codex::sample_tuple_get< 1 > (tuple) , 1.1  );
    ASSERT_EQ( codex::sample_tuple_get< 2 > (tuple) , 'c'  );

    std::cout << codex::pretty_type_name( 
        codex::type_list< codex::sample_tuple< int , double , char >>::type<0> {}  ) << std::endl;

    static_assert(std::is_same< codex::type_list< codex::sample_tuple< int , double , char >>::type<0> , int >::value);
    
    std::cout << codex::pretty_type_name( 
        codex::type_list< codex::sample_tuple< int , double , char , std::string >>::last {}  ) << std::endl;
    std::cout << codex::pretty_type_name( 
        codex::type_list< int , double , char >::type<0> {}  ) << std::endl;
    std::cout << codex::pretty_type_name( 
        codex::type_list< int , double , char >::last {}  ) << std::endl;
    std::cout << codex::pretty_type_name( 
        codex::type_list< std::tuple< int , double , char >>::type<0> {}  ) << std::endl;
    std::cout << codex::pretty_type_name( 
        codex::type_list< std::tuple< int , double , char >>::last {}  ) << std::endl;

    std::cout << codex::pretty_type_name( 
        codex::type_list< int ,char >::
                template push_back< double >::
                    template rebind< std::tuple >{} 
    ) << std::endl;

    std::cout << codex::pretty_type_name( 
        codex::type_list< std::tuple< int ,char > >::
                template push_back< double >::
                    template rebind< std::tuple >{} 
    ) << std::endl;

    std::cout << codex::pretty_type_name( 
        codex::type_list< std::tuple< int ,char > >::pop_back::
                    template rebind< std::tuple >{} 
    ) << std::endl;
}