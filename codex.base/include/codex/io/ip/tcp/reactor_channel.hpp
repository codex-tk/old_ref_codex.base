#ifndef __codex_io_ip_tcp_reactor_channel_h__
#define __codex_io_ip_tcp_reactor_channel_h__

#include <codex/reactor/poll_handler.hpp>

namespace codex {
  class loop;
}

namespace codex { namespace io { namespace ip { namespace tcp {

  class reactor_channel {
  public:
    reactor_channel( codex::loop& l );
    ~reactor_channel( void );

    codex::loop& loop( void );

    int bind( int fd );
  private:
    static void handle_event0( codex::reactor::poll_handler* poll , const int events );
    void handle_pollin( void );
    void handle_pollout( void );
  private:
    codex::loop& _loop;
    codex::reactor::poll_handler _poll_handler;
    int _fd;
  };

}}}}

#endif
