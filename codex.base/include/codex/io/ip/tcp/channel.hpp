/**
*/
#ifndef __codex_io_ip_tcp_channel_h__
#define __codex_io_ip_tcp_channel_h__

#include <codex/codex.hpp>
#if defined( __codex_win32__ )
#include <codex/io/ip/tcp/proactor_channel.hpp>
#else
#include <codex/io/ip/tcp/reactor_channel.hpp>
#endif

namespace codex { namespace io { namespace ip { namespace tcp {


#if defined( __codex_win32__ )
typedef proactor_channel channel;
#else
typedef reactor_channel channel;
#endif

class event_handler {
public:
  event_handler( void );
  virtual ~event_handler( void );
  virtual void on_read( codex::buffer::shared_blk blk ) = 0 ;
  virtual void on_write( const int write_bytes , bool flushed ) = 0;
  virtual void on_error( const std::error_code& ec ) = 0;
  
  void channel_ptr( tcp::channel_ptr ptr );
  tcp::channel_ptr& channel_ptr( void );
  void on_error0( const std::error_code& ec );
private:
  tcp::channel_ptr _channel_ptr;
};

}}}}

#endif
