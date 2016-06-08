/**
 */
#include <codex/loop.hpp>

namespace codex{

  loop::loop( void )
  {
  }

  loop::~loop( void ) {
  }

  int loop::dispatch( const int waitms ) {
    struct _scope_loop {
      _scope_loop( loop& l ) : obj(l){ obj._loop_id = std::this_thread::get_id(); }
      ~_scope_loop(void){ obj._loop_id = std::thread::id(); }
      loop& obj;
    };
    int dispatched = 0;
    do {
      _scope_loop s(*this);
      dispatched = _engine.wait( waitms );
      if ( dispatched < 0 )
        return dispatched;
      dispatched += drain_ops();
    } while (0);
    return drain_in_loop_ops() + dispatched; 
  }
  
  void loop::post( operation_type* ptr ){
    if ( in_loop() ) {
      _in_loop_ops.add_tail(ptr);
    } else {
      codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
      _ops.add_tail( ptr );
      _engine.wakeup();
    }
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

  int loop::drain_in_loop_ops( void ) {
    int drains = 0;
    codex::slist< operation_type > drain_ops_list;
    _in_loop_ops.swap( drain_ops_list );
    while( drain_ops_list.head() ) {
      operation_type* op = drain_ops_list.remove_head();
      (*op)();
      ++drains;
    }
    return drains;
  }

  codex::loop::engine_type& loop::engine( void ) {
    return _engine;
  }

  bool loop::in_loop( void ) {
    return _loop_id == std::this_thread::get_id();
  }
}
