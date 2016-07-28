#include <codex/reactor/poll_handler.hpp>

namespace codex { namespace reactor {

  poll_handler::poll_handler( handler cb )
    : _handler( cb )
  {
  }

  poll_handler::~poll_handler( void ){

  }

  void poll_handler::operator()( int polls ) {
    //cassert( _handler != nullptr );
    if ( _handler )
      _handler( this , polls );
  }

  int poll_handler::events( void ) {
    return _events;
  }

  void poll_handler::events( const int evt ) {
    _events = evt;
  }

  void poll_handler::set( poll_events e ){
    _events |= e;
  }

  void poll_handler::clear( poll_events e ){
    _events &= ~e;
  }

}}
