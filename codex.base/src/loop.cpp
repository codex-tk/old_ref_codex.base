/**
 */
#include <codex/loop.hpp>

namespace codex{

  loop::loop( void )
  {
  }

  loop::~loop( void ) {
  }

  int loop::dispatch( void ) {
    return drain_ops();
  }
  
  void loop::post( operation_type* ptr ){
    codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
    _ops.add_tail( ptr );
  }

  int loop::drain_ops( void ) {
    int drains = 0;
    codex::slist< operation_type > drain_ops_list;
    do {
      codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
      _ops.swap( drain_ops_list );
    } while (0);
    while( drain_ops_list.head() ) {
      operation_type* op = drain_ops_list.remove_head();
      (*op)();
      ++drains;
    }
    return drains;
  }

}
