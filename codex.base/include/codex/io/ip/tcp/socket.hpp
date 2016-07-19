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

    // todo
    void connect( );

    // todo
    void accept( socket& fd );

  private:
    codex::loop& _loop;
    codex::io::async_layer::descriptor_type _descriptor;
  };

}}}}

#endif

