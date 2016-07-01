/**
 */
#ifndef __codex_io_ip_tcp_proactor_channel_builder_h__
#define __codex_io_ip_tcp_proactor_channel_builder_h__

#include <codex/codex.hpp>
#include <codex/buffer/packetizer.hpp>
#include <codex/io/ip/tcp/proactor_channel.hpp>

namespace codex {
  class loop;
}

namespace codex {  namespace io { namespace ip { namespace tcp {

  class proactor_channel_builder {
  public:
    proactor_channel_builder( void );
    virtual ~proactor_channel_builder( void );

    virtual codex::loop& loop( void ) = 0;
    virtual std::shared_ptr< event_handler > handler() = 0;

    virtual std::shared_ptr< codex::buffer::packetizer > packetizer( void ); 

    std::shared_ptr< proactor_channel > build( void );
  public:
    void on_end_reference(proactor_channel* ptr );
  private:
    codex::threading::mutex _lock;
    std::vector< proactor_channel* > _channels;
  };


}}}}


#endif
