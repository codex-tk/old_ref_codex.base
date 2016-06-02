/**
 */
#include <codex/loop.hpp>

namespace codex{

  loop::loop( void )
    : _ops( new std::vector< operation_ptr >())
    , _ops_reserve( new std::vector< operation_ptr >())
  {
    // pre allocate ops vector
    _ops->reserve(256);
    _ops_reserve->reserve(256);
  }

  loop::~loop( void ) {
  }

  int loop::dispatch( void ) {
    drain_ops();
    return 0;
  }
  
  void loop::post( const loop::operation_ptr& ptr ){
    codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
    _ops->push_back(ptr);
  }

  void loop::drain_ops( void ) {
    bool run_ops = false;
    do {
      codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
      run_ops = !_ops->empty();
      if ( run_ops ) {
        // swap for prevent allocation
        _ops.swap( _ops_reserve );
      }
    } while (0);
    if ( run_ops ) {
      for ( auto it : (*_ops_reserve) ) {
        (*it)();
      }
      _ops_reserve->clear();
    }

  }

}
