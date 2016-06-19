#include <codex/io/ip/tcp/proactor_channel_builder.hpp>
#include <codex/io/ip/tcp/proactor_channel.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {
namespace {
  struct channel_deleter {
    void operator()( proactor_channel* chan ) {
      chan->release();
    }
  };
}

  proactor_channel_builder::proactor_channel_builder( void ) {

  }

  proactor_channel_builder::~proactor_channel_builder( void ) {

  }

  std::shared_ptr< codex::buffer::packetizer > proactor_channel_builder::packetizer( void ) {
    return std::make_shared< codex::buffer::random_packetizer >( 1500 );
  }

  channel_ptr proactor_channel_builder::build( void ) {
    static channel_deleter deleter;
    proactor_channel* chan = nullptr;
    do {
      codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
      if ( !_channels.empty() ) {
        chan = _channels.back();
        _channels.pop_back();
      }
    } while (0);
    if (chan== nullptr ) {
      chan = new proactor_channel();
    }
    chan->add_ref();
    channel_ptr ptr( chan , deleter );
    ptr->set_builder( this );
    ptr->set_loop( &loop());
    ptr->set_packetizer( packetizer());
    auto handler0 = handler();
    handler0->channel_ptr(ptr);
    ptr->set_handler( handler0 );
    return ptr;
  }

  void proactor_channel_builder::on_end_reference( proactor_channel* ptr ) {
    codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
    ptr->reset();
    _channels.push_back(ptr);
  }

}}}}



