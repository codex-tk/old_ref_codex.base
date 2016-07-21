#include <codex/reactor/reactor.hpp>

namespace codex { namespace reactor {

  engine::engine( void ) {
    _implementation.bind( _wakeup.handle() , _wakeup.handler() );
  }

  engine::~engine( void ) {
    _implementation.unbind( _wakeup.handle());
  }

  reactor::implementation_type& engine::implementation( void ) {
    return _implementation;
  }

  int engine::wait( const int waitms ) {
    return _implementation.wait( waitms );
  }

  void engine::wakeup( void ) {
    _wakeup.set();
  }

}}
