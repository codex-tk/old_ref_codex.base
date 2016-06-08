#include <codex/reactor/reactor.hpp>

namespace codex { namespace reactor {

  engine::engine( void ) {
    _poller.bind( _wakeup.handle() , _wakeup.handler() );
  }

  engine::~engine( void ) {
    _poller.unbind( _wakeup.handle());
  }

  poller& engine::reactor( void ) {
    return _poller;
  }

  int engine::wait( const int waitms ) {
    return _poller.wait( waitms );
  }

  void engine::wakeup( void ) {
    _wakeup.set();
  }

}}
