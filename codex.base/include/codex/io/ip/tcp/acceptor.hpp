/**
 */
#ifndef __codex_io_ip_tcp_acceptor_h__
#define __codex_io_ip_tcp_acceptor_h__

#include <codex/codex.hpp>

#if defined( __codex_win32__)
#include <codex/proactor/proactor.hpp>
#else
#include <codex/reactor/poll_handler.hpp>
#endif
#include <codex/io/ip/address.hpp>
#include <codex/io/ip/socket_ops.hpp>

namespace codex {
  class loop;
}

namespace codex { namespace io { namespace ip { namespace tcp {

  class acceptor {
  public:
    typedef std::function< void ( codex::io::ip::socket_ops<>::socket_type fd 
        , const codex::io::ip::tcp::address& addr ) > on_accept_handler;
  public:
    acceptor( codex::loop& l );
    ~acceptor( void );

    int open( const int port );
    void close( void );

    template < class Handler >
    void set_on_accept( Handler&& h ) {
      _on_accept = on_accept_handler( 
          std::forward< Handler >( h ));
    }
  private:
#if defined( __codex_win32__)
    void do_accept(void);
    void handle_accept(const std::error_code& ec, const int io_bytes);
    static void handle_accept(void* ctx, const std::error_code& ec, const int io_bytes);
#else
    void handle_pollin( void );
    static void handle_event0( codex::reactor::poll_handler* poll , const int events );
#endif
  private:
    codex::loop& _loop;
#if defined( __codex_win32__)
    codex::proactor::impl::handler _handler;
    char _accept_address[sizeof(sockaddr_storage) * 2];
    codex::io::ip::socket_ops<>::socket_type _accepted;
#else
    codex::reactor::poll_handler _poll_handler;
#endif
    codex::io::ip::socket_ops<>::socket_type _fd;
    on_accept_handler _on_accept;
  };

}}}}


#endif
