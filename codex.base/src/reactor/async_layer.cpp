#include <codex/reactor/async_layer.hpp>
#include <codex/loop.hpp>
#include <codex/reactor/reactor.hpp>

namespace codex { namespace reactor {

  async_layer::async_layer( loop& l , engine& e )
    : _loop(l) , _engine(e)
  {
  }

  async_layer::~async_layer( void ) {

  }

}}
