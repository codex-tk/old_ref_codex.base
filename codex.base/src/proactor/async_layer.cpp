#include <codex/codex.hpp>
#include <codex/slist.hpp>
#include <codex/operation.hpp>
#include <codex/loop.hpp>
#include <codex/proactor/async_layer.hpp>

namespace codex { namespace proactor {

  async_layer::async_layer( codex::loop& l )
    : _loop(l)
  {
  }

  async_layer::~async_layer( void ) {

  }

  async_layer::descriptor_type async_layer::wrap( SOCKET native_fd ) {
    return native_fd;
  }

  codex::loop& async_layer::loop( void ) {
    return _loop;
  }
}}
