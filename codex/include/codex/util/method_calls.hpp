#ifndef __codex_method_calls_h__
#define __codex_method_calls_h__

namespace codex {

    template < typename TypeT >
    class method_calls{
    public:
        method_calls( void ){   
        }
        void call( int ptr ) {
            if ( _calls.find(ptr) == _calls.end() ){
                _calls[ptr] = 0;
            }
            _calls[ptr] = _calls[ptr] + 1;
        }
        int call_counts( int ptr ) {
            if ( _calls.find(ptr) == _calls.end() ){
                return 0;
            }
            return _calls[ptr];
        }
    private:
        std::map< int , int > _calls;
    };

}

#endif