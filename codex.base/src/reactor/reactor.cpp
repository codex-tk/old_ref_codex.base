#include <codex/reactor/reactor.hpp>

namespace codex { namespace reactor {

  event_handler::event_handler( callback cb )
    : _callback( cb )
  {
  }

  event_handler::~event_handler( void ){

  }

  void event_handler::operator()( int events ) {
    //cassert( _callback != nullptr );
    if ( _callback )
      _callback( this , events );
  }

}}
