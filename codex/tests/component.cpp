#include <gtest/gtest.h>
#include <codex/component/container.hpp>

class component_life_check {
public:
    component_life_check( bool& is_live )  
        : _is_live(is_live)
    {
        _is_live = true;
    }
    ~component_life_check( void ) {
        _is_live = false;
    }
    DECL_COMPONENT( component_life_check  )  
private:
    bool& _is_live;
};
IMPL_COMPONENT(component_life_check )

class null_component{
public:
    DECL_COMPONENT( null_component )
};
IMPL_COMPONENT( null_component )

TEST( component , usage ) {
    bool is_live = false; 
    {
        codex::component::container container;
        container.insert( std::make_shared<component_life_check>( is_live ) );
        std::shared_ptr< component_life_check > bcptr = container.get<component_life_check>();
        ASSERT_TRUE(bcptr);
        ASSERT_TRUE(is_live);
        ASSERT_FALSE( container.get<null_component>());
    }
    ASSERT_FALSE(is_live);
}

class A {
public:
    A( void ) {}
    virtual ~A(void) {}
    DECL_COMPONENT( A );
};
IMPL_COMPONENT(A);

class B : public A {
public:
    virtual ~B(void){}
    DECL_INTERFACE_COMPONENT( B , A );  
};
IMPL_INTERFACE_COMPONENT( B , A );  

class C : public B {
public:
    virtual ~C(void){}
    DECL_NORMAL_COMPONENT( C , B );  
};
IMPL_NORMAL_COMPONENT( C , B );  

class D : public C {
public:
    virtual ~D(void){}
    DECL_INTERFACE_COMPONENT( D , C );  
};
IMPL_INTERFACE_COMPONENT( D , C );  

class E0 : public D {
public:
    virtual ~E0(void){}
    DECL_NORMAL_COMPONENT( E0 , D );  
};
IMPL_NORMAL_COMPONENT( E0 , D );  

class E1 : public D {
public:
    virtual ~E1(void){}
    DECL_NORMAL_COMPONENT( E1 , D );  
};
IMPL_NORMAL_COMPONENT( E1 , D );  

TEST( compnent , tmp ) {
    namespace ccd = codex::component::detail;
    ASSERT_FALSE( ccd::is_interface<A>::value );
    ASSERT_TRUE(  ccd::is_interface<B>::value );
    ASSERT_TRUE(  ccd::is_interface<C>::value );

    ASSERT_EQ( 
        ccd::representation_component<B>::type::component_id() ,
        ccd::representation_component<C>::type::component_id());

    ASSERT_EQ( 
        ccd::representation_component<D>::type::component_id() ,
        ccd::representation_component<E0>::type::component_id());

    ASSERT_EQ( 
        ccd::representation_component<D>::type::component_id() ,
        ccd::representation_component<E1>::type::component_id());
}

class service{
public:
    virtual ~service(void){}

    virtual std::string operator()( void ){      
        return "service";
    }
public:
    DECL_INTERFACE_COMPONENT( service , codex::null_type )
};
IMPL_INTERFACE_COMPONENT( service , codex::null_type )

class sample_service : public service{
public:
    virtual ~sample_service(void){}

    virtual std::string operator()( void ){      
        return "sample_service";
    }
public:
    DECL_NORMAL_COMPONENT( sample_service , service )
};
IMPL_NORMAL_COMPONENT( sample_service , service )  


class real_service: public service{
public:
    virtual ~real_service(void){}

    virtual std::string operator()( void ){      
        return "real_service";
    }
public:
    DECL_NORMAL_COMPONENT( real_service , service )
};
IMPL_NORMAL_COMPONENT( real_service , service )

TEST( compnent , interface_base ){
    std::shared_ptr<sample_service> scptr = std::make_shared<sample_service>();
    std::shared_ptr<real_service> rcptr = std::make_shared<real_service>();

    codex::component::container container;
    container.insert(scptr);
    ASSERT_EQ( (*container.get<service>())() , "sample_service" );
    ASSERT_TRUE(container.get<sample_service>());
    ASSERT_FALSE(container.get<real_service>());
    container.insert(rcptr);
    ASSERT_EQ( (*container.get<service>())() , "real_service" );
    ASSERT_TRUE(container.get<real_service>());
    ASSERT_TRUE(container.get<sample_service>());
}

namespace {
    class svc {
    public:
        DECL_COMPONENT( svc )
    };
    IMPL_COMPONENT( svc )

    class svc0 : public svc{
    public:
        DECL_NORMAL_COMPONENT( svc0 , svc  )
    };
    IMPL_NORMAL_COMPONENT(  svc0 , svc )

    class svc1 : public svc{
    public:
        DECL_NORMAL_COMPONENT( svc1 , svc  )
    };
    IMPL_NORMAL_COMPONENT(  svc1 , svc )
}

TEST( compnent , normal_base ){
    std::shared_ptr<svc0> s0 = std::make_shared<svc0>();
    std::shared_ptr<svc1> s1 = std::make_shared<svc1>();

    codex::component::container container;
    container.insert(s0);
    container.insert(s1);
    ASSERT_FALSE(container.get<svc>());
    ASSERT_TRUE( container.get<svc0>());
    ASSERT_TRUE( container.get<svc1>());
    ASSERT_NE( 
        reinterpret_cast<intptr_t>(container.get<svc0>().get())
     ,  reinterpret_cast<intptr_t>(container.get<svc1>().get())
    );
}