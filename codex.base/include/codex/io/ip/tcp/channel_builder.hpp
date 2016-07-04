/**
 */
#ifndef __codex_io_ip_tcp_channel_builder_h__
#define __codex_io_ip_tcp_channel_builder_h__

#include <codex/codex.hpp>
#include <codex/buffer/packetizer.hpp>
#include <codex/io/ip/tcp/channel.hpp>

namespace codex {
  class loop;
}

namespace codex {  namespace io { namespace ip { namespace tcp {

  class channel_builder {
  public:
    channel_builder( void );
    virtual ~channel_builder( void );

    virtual codex::loop& loop( void ) = 0;
    virtual std::shared_ptr< event_handler > handler() = 0;

    virtual std::shared_ptr< codex::buffer::packetizer > packetizer( void ); 

    std::shared_ptr< channel > build( void );
  public:
    void on_end_reference( channel* ptr );
  private:
    codex::threading::mutex _lock;
    std::vector< channel* > _channels;
  };


}}}}


#endif
