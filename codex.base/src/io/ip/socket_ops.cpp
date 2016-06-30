#include <codex/io/ip/socket_ops.hpp>

namespace codex{ namespace io{ namespace ip{
  namespace {
    ::linger linger_value( int on_off , int liv ){
      ::linger li;
      li.l_onoff = on_off;
      li.l_linger = liv;
      return li;
    }
  }
  template < class SocketType >
  typename socket_ops< SocketType >::option::linger 
  socket_ops< SocketType >::option::linger_remove_time_wait( linger_value(1,0));

}}}
