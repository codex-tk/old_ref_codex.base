#include <codex/io/ip/tcp/channel.hpp>


namespace codex { namespace io { namespace ip { namespace tcp {
namespace {
  struct channel_deleter {
    void operator()( reactor_channel* chan ) {
      chan->release();
    }
  };
}

  reactor_channel_builder::reactor_channel_builder( void ) {

  }

  reactor_channel_builder::~reactor_channel_builder( void ) {

  }

  std::shared_ptr< codex::buffer::packetizer > reactor_channel_builder::packetizer( void ) {
    return std::make_shared< codex::buffer::random_packetizer >( 1500 );
  }

  channel_ptr reactor_channel_builder::build( void ) {
    static channel_deleter deleter;
    reactor_channel* chan = nullptr;
    do {
      codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
      if ( !_channels.empty() ) {
        chan = _channels.back();
        _channels.pop_back();
      }
    } while (0);
    if (chan== nullptr ) {
      chan = new reactor_channel();
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

  void reactor_channel_builder::on_end_reference( reactor_channel* ptr ) {
    codex::threading::lock_guard< codex::threading::mutex > guard(_lock);
    ptr->reset();
    _channels.push_back(ptr);
  }

}}}}



