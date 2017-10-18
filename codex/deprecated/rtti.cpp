#include "stdafx.h"
#include <codex/util/rtti.hpp>

namespace codex {
namespace util {

rtti::rtti( const std::string& name , const rtti* base )
	: _name( name ) , _base( base ) {
}

rtti::~rtti( void ) {

}

rtti::get_handler::get_handler( void )
	: _handler( nullptr ){
}

rtti::get_handler::~get_handler( void ) {

}

const codex::util::rtti* rtti::get_handler::operator()( void ) const {
	return  _handler != nullptr ?  (*_handler)() : nullptr;
}

void rtti::get_handler::set_handler( handler_function h ) {
	_handler = h;
}


}}


