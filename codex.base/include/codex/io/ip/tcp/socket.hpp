/**
 */
#ifndef __codex_io_ip_tcp_socket_h__
#define __codex_io_ip_tcp_socket_h__

#include <codex/codex.hpp>
#include <codex/loop.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  /**
   */
  class socket{
  public:
    socket( codex::loop& l );
    ~socket( void );

    template < class Handler >
    void connect( const codex::io::ip::tcp::address& addr 
        , Handler&& handler ){
      _loop.async_layer().connect( _descriptor 
          , addr 
          , std::forward< Handler >(handler));
    }

    template < class Handler >
    void accept( socket& fd , Handler&& handler ){
      //
    }

  private:
    codex::loop& _loop;
    codex::io::async_layer::descriptor_type _descriptor;
  };

}}}}

#endif

