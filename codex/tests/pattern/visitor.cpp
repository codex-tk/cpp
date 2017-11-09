#include <gtest/gtest.h>
#include "../helper.hpp"
#include <memory>

class apple;
class water;
class fruit;
class cart;

class visitor {
public:
    virtual void visit( const std::shared_ptr<apple>& apple ) = 0;
    virtual void visit( const std::shared_ptr<water>& apple ) = 0;
    virtual void visit( const std::shared_ptr<fruit>& apple ) = 0;
    virtual void visit( const std::shared_ptr<cart>& apple ) = 0;
};

class element  {
public:
    virtual void accept( const std::shared_ptr<visitor>& visitor ) = 0;
};

class apple : public element , public std::enable_shared_from_this<apple>{
public:
    virtual void accept( const std::shared_ptr<visitor>& visitor ){
        std::shared_ptr<apple> ptr = shared_from_this();
        visitor->visit(ptr);
    }
};

class water : public element , public std::enable_shared_from_this<water>{
public:
    virtual void accept( const std::shared_ptr<visitor>& visitor ){
        std::shared_ptr<water> ptr = shared_from_this();
        visitor->visit(ptr);
    }
};

class fruit : public element , public std::enable_shared_from_this<fruit>{
public:
    virtual void accept( const std::shared_ptr<visitor>& visitor ){
        std::shared_ptr<fruit> ptr = shared_from_this();
        visitor->visit(ptr);
    }
};

class cart : public element , public std::enable_shared_from_this<cart>{
public:
    cart( void ) {
        _ptrs.emplace_back( std::make_shared<apple>());
        _ptrs.emplace_back( std::make_shared<water>());
        _ptrs.emplace_back( std::make_shared<fruit>());
    }
    virtual void accept( const std::shared_ptr<visitor>& visitor ){
        std::shared_ptr<cart> ptr = shared_from_this();
        visitor->visit(ptr);
        for ( auto p : _ptrs ) {
            p->accept(visitor);
        }
    }
private:
    std::vector< std::shared_ptr<element> > _ptrs;
};

class visitor_impl : public visitor {
public:
    virtual void visit( const std::shared_ptr<apple>& apple ){
        _messages.push_back("visit apple\n");
    }
    virtual void visit( const std::shared_ptr<water>& water ){
        _messages.push_back("visit water\n");
    }
    virtual void visit( const std::shared_ptr<fruit>& fruit ){
        _messages.push_back("visit fruit\n");
    }
    virtual void visit( const std::shared_ptr<cart>& cart ) {
        _messages.push_back("visit cart\n");
    }

    void print(){
        for ( auto m : _messages ) {
            PRINTF( "%s" , m.c_str() );
        }
    }
private:
    std::vector< std::string > _messages;
};

TEST( dp , 0 ){
    auto cart_ptr = std::make_shared<cart>();
    auto visitor = std::make_shared<visitor_impl>();
    cart_ptr->accept(visitor);
    visitor->print();
}