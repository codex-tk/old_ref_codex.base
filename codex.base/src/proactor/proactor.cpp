#include <codex/proactor/proactor.hpp>

namespace codex { namespace proactor {

  engine::engine( void ) {
  }

  engine::~engine( void ) {
  }

  impl& engine::impl( void ) {
    return _impl;
  }

  int engine::wait( const int waitms ) {
    return _impl.wait( waitms );
  }

  void engine::wakeup( void ) {
    _impl.wakeup();
  }

}}
