/**
 */
#ifndef __codex_io_ip_tcp_reactor_channel_builder_h__
#define __codex_io_ip_tcp_reactor_channel_builder_h__

#include <codex/codex.hpp>
#include <codex/buffer/packetizer.hpp>
#include <codex/io/ip/tcp/reactor_channel.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  class reactor_channel_builder {
  public:
    reactor_channel_builder( void );
    virtual ~reactor_channel_builder( void );

    virtual codex::loop& loop( void ) = 0;
    virtual std::shared_ptr< event_handler > handler() = 0;

    virtual std::shared_ptr< codex::buffer::packetizer > packetizer( void ); 

    channel_ptr build( void );
  public:
    void on_end_reference( reactor_channel* ptr );
  private:
    codex::threading::mutex _lock;
    std::vector< reactor_channel* > _channels;
  };


}}}}


#endif
