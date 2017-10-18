#include "stdafx.h"
#include <codex/threading/atomic/memory_order.hpp>

namespace codex {
namespace threading {


void acquire::acquire_barrier( void ) {
}


void no_acquire::acquire_barrier( void ) {

}


void release::release_barrier( void ) {

}

void no_release::release_barrier( void ) {

}

}}