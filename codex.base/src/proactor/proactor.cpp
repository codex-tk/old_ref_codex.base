#include <codex/proactor/proactor.hpp>

namespace codex { namespace proactor {

  engine::engine( void ) {
  }

  engine::~engine( void ) {
  }

  proactor::implementation_type& engine::implementation( void ) {
    return _implementation;
  }

  int engine::wait( const int waitms ) {
    return _implementation.wait( waitms );
  }

  void engine::wakeup( void ) {
    _implementation.wakeup();
  }

}}
