#ifndef __codex_io_ip_tcp_reactor_channel_h__
#define __codex_io_ip_tcp_reactor_channel_h__

#include <memory>
#include <deque>

#include <codex/reactor/poll_handler.hpp>

#include <codex/buffer/packetizer.hpp>

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

    void write( codex::buffer::shared_blk blk );
  private:
    static void handle_event0( codex::reactor::poll_handler* poll , const int events );
    int handle_pollin( void );
    int handle_pollout( void );
    void handle_error( int ret );
    void write0( codex::buffer::shared_blk blk );
  private:
    codex::loop& _loop;
    codex::reactor::poll_handler _poll_handler;
    int _fd;
    std::shared_ptr< codex::buffer::packetizer > _packetizer;
    std::deque< codex::buffer::shared_blk > _write_packets;
  };

}}}}

#endif
