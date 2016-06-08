#include <codex/io/ip/tcp/reactor_channel.hpp>

#include <codex/loop.hpp>

namespace codex { namespace io { namespace ip { namespace tcp {

  reactor_channel::reactor_channel( codex::loop& l ) 
    : _loop( l )
    , _poll_handler( &reactor_channel::handle_event0 )
    , _fd(-1)
  {
  }

  reactor_channel::~reactor_channel( void ) {

  }

  codex::loop& reactor_channel::loop( void ) {
    return _loop;
  }

  int reactor_channel::bind( int fd ) {
    if ( _fd != -1 ) 
      _loop.engine().reactor().unbind( _fd );
    _fd = fd;
    _poll_handler.events(codex::reactor::pollin);
    return _loop.engine().reactor().bind( _fd , &_poll_handler );
  }

  void reactor_channel::handle_pollin( void ) {
    
  }

  void reactor_channel::handle_pollout( void ) {

  }

  void reactor_channel::handle_event0( codex::reactor::poll_handler* p 
      , const int events )
  {
    reactor_channel* chan = codex::container_of( p , &reactor_channel::_poll_handler );
    if ( chan ) {
      if ( events & codex::reactor::pollin ) 
        chan->handle_pollin();
      if ( events & codex::reactor::pollout ) 
        chan->handle_pollout();
    }
  }

}}}}
