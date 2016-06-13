/**
 */
#ifndef __codex_io_ip_tcp_acceptor_h__
#define __codex_io_ip_tcp_acceptor_h__

#include <codex/reactor/poll_handler.hpp>
#include <codex/io/ip/address.hpp>

namespace codex {
  class loop;
}

namespace codex { namespace io { namespace ip { namespace tcp {

  class acceptor {
  public:
    acceptor( codex::loop& l );
    ~acceptor( void );

    int open( const int port );
    void close( void );

    template < class Handler >
    void set_on_accept( Handler&& h ) {
      _on_accept = std::function< void ( int fd , const codex::io::ip::tcp::address& addr ) >( 
          std::forward< Handler >( h ));
    }
  private:
    void handle_pollin( void );
    static void handle_event0( codex::reactor::poll_handler* poll , const int events );
  private:
    codex::loop& _loop;
    codex::reactor::poll_handler _poll_handler;
    int _fd;
    std::function< void ( int fd , const codex::io::ip::tcp::address& addr ) > _on_accept;
  };

}}}}


#endif
