#include <codex/io/ip/tcp/socket.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  socket::socket( codex::loop& l )
    : _loop(l)
  {
  }

  socket::~socket( void ) {

  }


}}}}
