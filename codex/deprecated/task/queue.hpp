#ifndef __codex_task_queue_h__
#define __codex_task_queue_h__

#include <codex/util/list_node.hpp>
#include <codex/threading/spin_lock.hpp>

namespace codex{
namespace taskex {

class queue {
public:
	class operation 
		: public codex::slist_node< operation >{
	public:
		typedef void (*handler)( operation* op );
		operation( queue* q );
		~operation( void );

		void set_handler( handler h );
		void  context( void* ctx );
		void* context( void );

		void dispatch( void );

		void operator()( void );
	private:
		queue* _queue;
		handler _handler;
		void* _context;
	};

	queue( void );
	~queue( void );

	void execute( operation* op );
	bool fetch( void );
private:
	codex::threading::spin_lock _lock;
	codex::slist_queue< operation > _op_queue;
	operation* _running;
};

}}

#endif